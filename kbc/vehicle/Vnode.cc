//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Vnode.h"

#include <openssl/rand.h>

#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/applications/kbc/Kbc_m.h"
#include "inet/networklayer/common/L3AddressTag_m.h"

namespace inet {

Define_Module(Vnode);

Vnode::~Vnode()
{
    cancelAndDelete(selfMsg);
}

void Vnode::initialize(int stage)
{
    ClockUserModuleMixin::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        numSent = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numReceived);

        localPort = par("localPort");
        destPort = par("destPort");
        startTime = par("startTime");
        stopTime = par("stopTime");
        packetName = par("packetName");
        dontFragment = par("dontFragment");
        if (stopTime >= CLOCKTIME_ZERO && stopTime < startTime)
            throw cRuntimeError("Invalid startTime/stopTime parameters");
        selfMsg = new ClockEvent("sendTimer");

        RegisterCar();
    }
}

void Vnode::RegisterCar()
{
    int numRsus = par("numRSUS");
    for (int i = 1; i <= numRsus; i++) {
        std::string rsu = "RSU" + std::to_string(i);
        rsus.push_back(rsu);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, rsus.size() - 1);
    int randomIndex = dist(gen);

    vehicleId = std::string(this->getParentModule()->getFullName());
    userId = random();
    password = generatePassword(6);
    src = "Okayama";  // implement a way to assign source and destination address of a given car
    dest = "Okadai";
    service = rsus[randomIndex];
    roles = { "read", "write", "modify" };
}

std::string Vnode::generatePassword(int length)
{
    unsigned char buffer[length];
    if (!RAND_bytes(buffer, length)) {
        EV_ERROR << "Random key generation failed." << endl;
    }
    // Convert to hexadecimal string
    std::stringstream ss;
    for (int i = 0; i < length; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)buffer[i];
    }

    return ss.str();
}

void Vnode::finish()
{
    recordScalar("packets sent", numSent);
    recordScalar("packets received", numReceived);
    ApplicationBase::finish();
}

void Vnode::setSocketOptions()
{
    int timeToLive = par("timeToLive");
    if (timeToLive != -1)
        socket.setTimeToLive(timeToLive);

    int dscp = par("dscp");
    if (dscp != -1)
        socket.setDscp(dscp);

    int tos = par("tos");
    if (tos != -1)
        socket.setTos(tos);

    const char *multicastInterface = par("multicastInterface");
    if (multicastInterface[0]) {
        IInterfaceTable *ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
        NetworkInterface *ie = ift->findInterfaceByName(multicastInterface);
        if (!ie)
            throw cRuntimeError("Wrong multicastInterface setting: no interface named \"%s\"", multicastInterface);
        socket.setMulticastOutputInterface(ie->getInterfaceId());
    }

    bool receiveBroadcast = par("receiveBroadcast");
    if (receiveBroadcast)
        socket.setBroadcast(true);

    bool joinLocalMulticastGroups = par("joinLocalMulticastGroups");
    if (joinLocalMulticastGroups) {
        MulticastGroupList mgl = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this)->collectMulticastGroups();
        socket.joinLocalMulticastGroups(mgl);
    }
    socket.setCallback(this);
}

L3Address Vnode::chooseDestAddr()
{
    int k = intrand(destAddresses.size());
    if (destAddresses[k].isUnspecified() || destAddresses[k].isLinkLocal()) {
        L3AddressResolver().tryResolve(destAddressStr[k].c_str(), destAddresses[k]);
    }
    return destAddresses[k];
}

void Vnode::sendRAPacket()
{
    strt = simTime();
    startT = std::chrono::high_resolution_clock::now();
    Packet *packet = new Packet();
    if (dontFragment)
        packet->addTag<FragmentationReq>()->setDontFragment(true);
    const auto& payload = makeShared<RAPacket>();
    payload->setVID(vehicleId.c_str());
    payload->setService(service.c_str());
    payload->setVIP(vehicleIp.str().c_str());
    payload->setLifetime(2.0);
    payload->setChunkLength(B(par("messageLength")));
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    packet->insertAtBack(payload);
    packet->setName("RAPacket");
    L3Address destAddr = chooseDestAddr();
    emit(packetSentSignal, packet);
    socket.sendTo(packet, destAddr, destPort);
    numSent++;
}

void Vnode::processStart()
{
    socket.setOutputGate(gate("socketOut"));
    const char *localAddress = par("localAddress");
    socket.bind(*localAddress ? L3AddressResolver().resolve(localAddress) : L3Address(), localPort);
    setSocketOptions();

    const char *destAddrs = par("destAddresses");
    cStringTokenizer tokenizer(destAddrs);
    const char *token;

    while ((token = tokenizer.nextToken()) != nullptr) {
        destAddressStr.push_back(token);
        L3Address result;
        L3AddressResolver().tryResolve(token, result);
        if (result.isUnspecified())
            EV_ERROR << "cannot resolve destination address: " << token << endl;
        destAddresses.push_back(result);
    }

    if (!destAddresses.empty()) {
        selfMsg->setKind(SEND);
        processSend();
    }
    else {
        if (stopTime >= CLOCKTIME_ZERO) {
            selfMsg->setKind(STOP);
            scheduleClockEventAt(stopTime, selfMsg);
        }
    }
    vehicleIp = L3AddressResolver().resolve(this->getParentModule()->getFullName());
}

void Vnode::processSend()
{
    sendRAPacket();
}

void Vnode::processStop()
{
    socket.close();
}

void Vnode::handleMessageWhenUp(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        ASSERT(msg == selfMsg);
        switch (selfMsg->getKind()) {
            case START:
                processStart();
                break;

            case SEND:
                processSend();
                break;

            case STOP:
                processStop();
                break;
            case AUTHREQ:
                processRequest();
                break;
            default:
                throw cRuntimeError("Invalid kind %d in self message", (int)selfMsg->getKind());
        }
    }
    else
        socket.processMessage(msg);
}

void Vnode::socketDataArrived(UdpSocket *socket, Packet *packet)
{
    std::string pkname = packet->getName();
    if (pkname == "AStV+TGT")
        handleAttributeFromServer(packet);
    else if (pkname == "Ticket")
        handleServerTicket(packet);
    else if (pkname == "Authenticator")
        handleRSUAuthentication(packet);
    else if (pkname == "Handover-Success")
        handleHandoverSuccess(packet);
    else
        delete packet;
}

void Vnode::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void Vnode::socketClosed(UdpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION)
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void Vnode::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

    char buf[100];
    sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
    getDisplayString().setTagArg("t", 0, buf);
}

void Vnode::handleAttributeFromServer(Packet *pkt)
{
    numReceived++;
    rcvdBytes += pkt->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);
    if (file.is_open()) {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "Vehicle" << "\n";
        file.close();
    }

    auto data = pkt->removeAtFront<TGTPacket>();
    if (data != nullptr)
    {
        // decrypt
        std::string ATencrypted = data->getAT();
        std::vector<unsigned char> binaryKey = hex2bin(data->getKey());
        std::copy(binaryKey.begin(), binaryKey.end(), secret);
        std::string ATdecrypted = decrypt(ATencrypted, secret);
        std::string TGT = data->getTGT();

        // split the string to individual attributes
        std::string TGSname, timestamp, lifetime, sessionKeyStr;
        std::stringstream ss(ATdecrypted);
        std::getline(ss, TGSname, '|');
        std::getline(ss, timestamp, '|');
        std::getline(ss, lifetime, '|');
        std::getline(ss, sessionKeyStr, '|');

        // valid session
        if (std::stod(lifetime) > (simTime().dbl() - std::stod(timestamp))) {
            // encrypt
            std::string AUplaintext = vehicleId + "|" + timestamp;
            std::vector<unsigned char> sessionKeyBinary = hex2bin(sessionKeyStr);
            std::copy(sessionKeyBinary.begin(), sessionKeyBinary.end(), sessionKey);
            std::string AUencrypted = encrypt(AUplaintext, sessionKey);

            // send message
            Packet *packet = new Packet();
            if (dontFragment)
                packet->addTag<FragmentationReq>()->setDontFragment(true);
            const auto& payload = makeShared<AttributePacket>();
            payload->setService(service.c_str());
            payload->setLifetime(lifetime.c_str());
            payload->setAU(AUencrypted.c_str());
            payload->setTGT(TGT.c_str());
            payload->setChunkLength(B(par("messageLength")));
            payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
            packet->insertAtBack(payload);
            packet->setName("TGT+ATvtgs+AUvtgs");
            L3Address destAddr = chooseDestAddr();
            emit(packetSentSignal, packet);
            socket.sendTo(packet, destAddr, destPort);
            numSent++;
        }
    }
    delete pkt;
}

void Vnode::handleServerTicket(Packet *pkt)
{
    numReceived++;
    rcvdBytes += pkt->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);
    if (file.is_open()) {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "Vehicle" << "\n";
        file.close();
    }

    auto data = pkt->removeAtFront<TicketPacket>();
    if (data != nullptr) {
        std::string ATencrypted = data->getAT();
        std::string ST = data->getST();
        std::string ATdecrypted = decrypt(ATencrypted, sessionKey);
        std::string serviceId, timestamp, lt, serviceSessionKeyStr;
        std::stringstream sss(ATdecrypted);
        std::getline(sss, serviceId, '|');
        std::getline(sss, timestamp, '|');
        std::getline(sss, lt, '|');
        std::getline(sss, serviceSessionKeyStr, '|');

        // encrypt
        std::vector<unsigned char> sessionKeyBinary = hex2bin(serviceSessionKeyStr);
        std::copy(sessionKeyBinary.begin(), sessionKeyBinary.end(), serviceKey);
        std::string AUplaintext = vehicleId + "|" + timestamp;
        std::string AUencrypted = encrypt(AUplaintext, serviceKey);
        validateVehicle(serviceId, ST, AUencrypted);

        simtime_t start = data->getTag<CreationTimeTag>()->getCreationTime();
        file.open("results/delay.csv", std::ios::app); // Open file in append mode
        if (file.is_open()) {
            file << (simTime() - strt).dbl() << "," << (simTime() - start).dbl() << "\n";
            file.close();
        }
    }
    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open()) {
        file << numSent << "," << numReceived << "," << "Vehicle" << std::endl;
        file.close();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - startT).count();
    file.open("results/ComputationCost.csv", std::ios::app);
    if (file.is_open()) {
        file << duration << "," << "vehicle" << std::endl;
        file.close();
    }

    delete pkt;
}

void Vnode::handleRSUAuthentication(Packet *pkt)
{
    numReceived++;
    rcvdBytes += pkt->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);
    if (file.is_open()) {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "Vehicle" << "\n";
        file.close();
    }

    L3Address destAddr = pkt->getTag<L3AddressInd>()->getSrcAddress();
    auto data = pkt->removeAtFront<AuthenticatorPacket>();
    if (data != nullptr) {
        std::string AUencrypted = data->getAU();
        tempAU = AUencrypted;
        std::string AUdecrypted = decrypt(AUencrypted, serviceKey);
        std::string serviceName, timestamp;
        std::stringstream sss(AUdecrypted);
        std::getline(sss, serviceName, '|');
        std::getline(sss, timestamp, '|');

        std::random_device rd;
        std::mt19937 gen(rd());
        nextService = getService(gen);
        if (serviceName == service) {
            if (2.0 > (simTime().dbl() - std::stod(timestamp))) {
                Packet *packet = new Packet("Success");
                if (dontFragment)
                    packet->addTag<FragmentationReq>()->setDontFragment(true);
                const auto& payload = makeShared<HandoverPacket>();
                payload->setChunkLength(B(par("messageLength")));
                payload->setService(nextService.c_str());
                payload->setIp(vehicleIp.str().c_str());
                payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
                packet->insertAtBack(payload);
                emit(packetSentSignal, packet);
                socket.sendTo(packet, destAddr, destPort);
                numSent++;

                selfMsg->setKind(AUTHREQ);
                scheduleClockEventAfter(10.0, selfMsg);
                EV_ERROR << vehicleIp << endl;
            }
        }
    }

    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open()) {
        file << numSent << "," << numReceived << "," << "Vehicle" << std::endl;
        file.close();
    }

    delete pkt;
}

void Vnode::validateVehicle(std::string rsu, std::string ST, std::string AU)
{
    L3Address destAddr = L3AddressResolver().resolve(rsu.c_str());
    Packet *packet = new Packet("Authenticator");
    if (dontFragment)
        packet->addTag<FragmentationReq>()->setDontFragment(true);
    const auto& payload = makeShared<AuthPacket>();
    payload->setST(ST.c_str());
    payload->setAU(AU.c_str());
    payload->setChunkLength(B(par("messageLength")));
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    packet->insertAtBack(payload);
    emit(packetSentSignal, packet);
    socket.sendTo(packet, destAddr, destPort);
    numSent++;
}

std::string Vnode::getService(std::mt19937& gen)
{
    if (rsus.empty()) {
        throw std::runtime_error("The vector is empty!");
    }

    std::uniform_int_distribution<> dist(0, rsus.size() - 1);
    int randomIndex = dist(gen);
    return rsus[randomIndex];
}

void Vnode::processRequest()
{
    L3Address destAddr = L3AddressResolver().resolve(nextService.c_str());
    Packet *packet = new Packet("AuthReq");
    if (dontFragment)
        packet->addTag<FragmentationReq>()->setDontFragment(true);
    const auto& payload = makeShared<AuthenticatorPacket>();
    payload->setAU(tempAU.c_str());
    payload->setChunkLength(B(par("messageLength")));
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    packet->insertAtBack(payload);
    emit(packetSentSignal, packet);
    socket.sendTo(packet, destAddr, destPort);
    numSent++;
}

void Vnode::handleHandoverSuccess(Packet *pk)
{
    numReceived++;
    rcvdBytes += pk->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);
    if (file.is_open()) {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "Vehicle" << "\n";
        file.close();
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    nextService = getService(gen);
    selfMsg->setKind(AUTHREQ);
    scheduleClockEventAfter(10.0, selfMsg);

    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open()) {
        file << numSent << "," << numReceived << "," << "Vehicle" << std::endl;
        file.close();
    }
}

void Vnode::processPacket(Packet *pk)
{
    emit(packetReceivedSignal, pk);
    EV_INFO << "Received packet: " << UdpSocket::getReceivedPacketInfo(pk) << endl;
    delete pk;
    numReceived++;
}

void Vnode::handleStartOperation(LifecycleOperation *operation)
{
    clocktime_t start = std::max(startTime, getClockTime());
    if ((stopTime < CLOCKTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime)) {
        selfMsg->setKind(START);
        scheduleClockEventAt(start, selfMsg);
    }
}

void Vnode::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(selfMsg);
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void Vnode::handleCrashOperation(LifecycleOperation *operation)
{
    cancelClockEvent(selfMsg);
    socket.destroy(); // TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
}

} // namespace inet
