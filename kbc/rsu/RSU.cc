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

#include "RSU.h"

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

Define_Module(RSU);

RSU::~RSU()
{
    cancelAndDelete(selfMsg);
}

void RSU::initialize(int stage)
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
    }
}

void RSU::finish()
{
    recordScalar("packets sent", numSent);
    recordScalar("packets received", numReceived);
    ApplicationBase::finish();
}

void RSU::setSocketOptions()
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

L3Address RSU::chooseDestAddr()
{
    int k = intrand(destAddresses.size());
    if (destAddresses[k].isUnspecified() || destAddresses[k].isLinkLocal()) {
        L3AddressResolver().tryResolve(destAddressStr[k].c_str(), destAddresses[k]);
    }
    return destAddresses[k];
}

void RSU::sendPacket()
{
    std::ostringstream str;
    str << packetName << "-" << numSent;
    Packet *packet = new Packet(str.str().c_str());
    if (dontFragment)
        packet->addTag<FragmentationReq>()->setDontFragment(true);
    const auto& payload = makeShared<ApplicationPacket>();
    payload->setChunkLength(B(par("messageLength")));
    payload->setSequenceNumber(numSent);
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    packet->insertAtBack(payload);
    L3Address destAddr = chooseDestAddr();
    emit(packetSentSignal, packet);
    socket.sendTo(packet, destAddr, destPort);
    numSent++;
}

void RSU::processStart()
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
}

void RSU::processSend()
{
    return;
}

void RSU::processStop()
{
    socket.close();
}

void RSU::handleMessageWhenUp(cMessage *msg)
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

            default:
                throw cRuntimeError("Invalid kind %d in self message", (int)selfMsg->getKind());
        }
    }
    else
        socket.processMessage(msg);
}

void RSU::socketDataArrived(UdpSocket *socket, Packet *packet)
{
    std::string pkname = packet->getName();
    if (pkname == "RSU Secret Key")
        setSecretKey(packet);
    else if (pkname == "Authenticator")
        handleAuthentication(packet);
    else if (pkname == "Success")
        handleLedger(packet);
    else if (pkname == "AuthenticatorSm")
        handleRSUAuthenticator(packet);
    else if (pkname == "AuthReq")
        handleAuthRequest(packet);
    else if (pkname == "Block-Confirmed")
        handleBlockSuccess(packet);
    else if (pkname == "Block-Denied")
        handleBlockDenied(packet);
    else
        delete packet;
}

void RSU::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void RSU::socketClosed(UdpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION)
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void RSU::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

    char buf[100];
    sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
    getDisplayString().setTagArg("t", 0, buf);
}

void RSU::setSecretKey(Packet *pkt)
{
    auto data = pkt->removeAtFront<SecretPacket>();
    if (data != nullptr) {
        std::string keyStr = data->getKey();
        std::vector<unsigned char> keyBinary = hex2bin(keyStr);
        std::copy(keyBinary.begin(), keyBinary.end(), secretKey);
    }
    delete pkt;
}

void RSU::handleAuthentication(Packet *pkt)
{
    numReceived++;
    auto start = std::chrono::high_resolution_clock::now();
    rcvdBytes += pkt->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);
    if (file.is_open()) {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "RSU" << "\n";
        file.close();
    }

    auto data = pkt->removeAtFront<AuthPacket>();
    if (data != nullptr) {
        std::string STencrypted = data->getST();
        std::string AUencrypted = data->getAU();

        // decrypt
        std::string STdecrypted = decrypt(STencrypted, secretKey);
        std::string vehicleId, timestamp, vehicleIP, lt, sessionKeyStr;
        std::stringstream sss(STdecrypted);
        std::getline(sss, vehicleId, '|');
        std::getline(sss, timestamp, '|');
        std::getline(sss, vehicleIP, '|');
        std::getline(sss, lt, '|');
        std::getline(sss, sessionKeyStr, '|');

        unsigned char sessionKey[16];
        std::vector<unsigned char> sessionKeyBinary = hex2bin(sessionKeyStr);
        std::copy(sessionKeyBinary.begin(), sessionKeyBinary.end(), sessionKey);

        std::string AUdecrypted = decrypt(AUencrypted, sessionKey);
        std::string vehId, TS;
        std::stringstream ss(AUdecrypted);
        std::getline(ss, vehId, '|');
        std::getline(ss, TS, '|');

        const char *service = this->getParentModule()->getFullName();

        std::string AUplaintext = std::string(service) + "|" + TS;
        std::string AUsEncrypted = encrypt(AUplaintext, sessionKey);
        sendAuthenticator(AUsEncrypted, vehicleIP);

        AUcache au(AUencrypted, vehicleIP);
        tempAU.push_back(au);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        file.open("results/ComputationCost.csv", std::ios::app);
        if (file.is_open()) {
            file << duration << "," << "RSU" << std::endl;
            file.close();
        }
    }
    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open()) {
        file << numSent << "," << numReceived << "," << "RSU" << std::endl;
        file.close();
    }
    delete pkt;
}

void RSU::sendAuthenticator(std::string AU, std::string ipStr)
{
     L3Address destAddr = L3AddressResolver().resolve(ipStr.c_str());
     Packet *packet = new Packet("Authenticator");
     if (dontFragment)
         packet->addTag<FragmentationReq>()->setDontFragment(true);
     const auto& payload = makeShared<AuthenticatorPacket>();
     payload->setChunkLength(B(par("messageLength")));
     payload->setAU(AU.c_str());
     payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
     packet->insertAtBack(payload);
     emit(packetSentSignal, packet);
     socket.sendTo(packet, destAddr, destPort);
     numSent++;
}

void RSU::handleLedger(Packet *pkt)
{
    auto data = pkt->removeAtFront<HandoverPacket>();
    if (data != nullptr) {
        std::string ipStr = data->getIp();
        for (const auto& AU : tempAU) {
            if (AU.ip == ipStr) {
                std::string service = data->getService();
                if (service != std::string(this->getParentModule()->getFullName())) {
                    L3Address destAddr = L3AddressResolver().resolve(service.c_str());
                    Packet *packet = new Packet("AuthenticatorSm");
                    if (dontFragment)
                        packet->addTag<FragmentationReq>()->setDontFragment(true);
                    const auto& payload = makeShared<AuthPacket>();
                    payload->setChunkLength(B(par("messageLength")));
                    payload->setAU(AU.tempAU.c_str());
                    payload->setST(AU.ip.c_str());
                    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
                    packet->insertAtBack(payload);
                    emit(packetSentSignal, packet);
                    socket.sendTo(packet, destAddr, destPort);
                    numSent++;
                }
            }
        }
    }
    delete pkt;
}

void RSU::handleRSUAuthenticator(Packet *pkt)
{
    auto data = pkt->removeAtFront<AuthPacket>();
    if (data != nullptr) {
        std::string AU = data->getAU();
        std::string ip = data->getST();
        AUcache au(AU, ip);
        tempAU.push_back(au);
    }
    delete pkt;
}

void RSU::handleAuthRequest(Packet *pk)
{
    auto data = pk->removeAtFront<AuthenticatorPacket>();
    if (data != nullptr) {
        std::string AU = data->getAU();
        for (const auto& au : tempAU) {
            if (au.tempAU == AU) {
                Packet *packet = new Packet("Transact");
                if (dontFragment)
                    packet->addTag<FragmentationReq>()->setDontFragment(true);
                const auto& payload = makeShared<AuthenticatorPacket>();
                payload->setChunkLength(B(par("messageLength")));
                payload->setAU(AU.c_str());
                payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
                packet->insertAtBack(payload);
                L3Address destAddr = chooseDestAddr();
                emit(packetSentSignal, packet);
                socket.sendTo(packet, destAddr, destPort);
                numSent++;
            }
        }
    }
}

void RSU::handleBlockSuccess(Packet *pk)
{
    auto data = pk->removeAtFront<AuthenticatorPacket>();
    if (data != nullptr) {
        for (const auto& au : tempAU) {
            if (au.tempAU == data->getAU()) {
                L3Address destAddr = L3AddressResolver().resolve(au.ip.c_str());
                Packet *packet = new Packet("Handover-Success");
                if (dontFragment)
                    packet->addTag<FragmentationReq>()->setDontFragment(true);
                const auto& payload = makeShared<ApplicationPacket>();
                payload->setChunkLength(B(par("messageLength")));
                payload->setSequenceNumber(numSent);
                payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
                packet->insertAtBack(payload);
                emit(packetSentSignal, packet);
                socket.sendTo(packet, destAddr, destPort);
                numSent++;
            }
        }
    }
}

void RSU::handleBlockDenied(Packet *pk)
{
    auto data = pk->removeAtFront<AuthenticatorPacket>();
    if (data != nullptr) {
        for (const auto& au : tempAU) {
            if (au.tempAU == data->getAU()) {
                L3Address destAddr = L3AddressResolver().resolve(au.ip.c_str());
                Packet *packet = new Packet("Handover-Failed");
                if (dontFragment)
                    packet->addTag<FragmentationReq>()->setDontFragment(true);
                const auto& payload = makeShared<ApplicationPacket>();
                payload->setChunkLength(B(par("messageLength")));
                payload->setSequenceNumber(numSent);
                payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
                packet->insertAtBack(payload);
                emit(packetSentSignal, packet);
                socket.sendTo(packet, destAddr, destPort);
                numSent++;
            }
        }
    }
}

void RSU::processPacket(Packet *pk)
{
    emit(packetReceivedSignal, pk);
    EV_INFO << "Received packet: " << UdpSocket::getReceivedPacketInfo(pk) << endl;
    delete pk;
    numReceived++;
}

void RSU::handleStartOperation(LifecycleOperation *operation)
{
    clocktime_t start = std::max(startTime, getClockTime());
    if ((stopTime < CLOCKTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime)) {
        selfMsg->setKind(START);
        scheduleClockEventAt(start, selfMsg);
    }
}

void RSU::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(selfMsg);
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void RSU::handleCrashOperation(LifecycleOperation *operation)
{
    cancelClockEvent(selfMsg);
    socket.destroy(); // TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
}

} // namespace inet


