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

#include "TAS.h"

#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/applications/kbc/Kbc_m.h"
#include "inet/transportlayer/common/L4PortTag_m.h"

namespace inet {

Define_Module(TAS);

TAS::~TAS()
{
    cancelAndDelete(selfMsg);
}

void TAS::initialize(int stage)
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
        if (!generateSecretKey(TGSSecretKey, sizeof(TGSSecretKey)))
            throw cRuntimeError("Failed to initialize secret key");
        TGSname = this->getParentModule()->getFullName();
    }
}

void TAS::RegisterRSU()
{
    static bool isRegistered = false;
    if (isRegistered) {
        return;
    }
    isRegistered = true;
    int numTAS = par("numTAS");
    int numRSU = par("numRSU");
    if (numTAS > 1) {
        cModule *network = getSimulation()->getSystemModule();
        cModule *TAS1 = network->getSubmodule("TAS1");
        cModule *TAS1App = TAS1->getSubmodule("app", 0);
        if (TAS1App != nullptr) {
            TAS *tas = dynamic_cast<TAS *>(TAS1App);
            if (tas != nullptr) {
                tas->generateRSU(numRSU);
                for (int i = 2; i <= numTAS; i++) {
                    std::string serv = "TAS" + std::to_string(i);
                    cModule *Serv = network->getSubmodule(serv.c_str());
                    cModule *TASApp = Serv->getSubmodule("app", 0);
                    if (TASApp != nullptr) {
                        TAS *tasApp = dynamic_cast<TAS *>(TASApp);
                        tasApp->setRSU(tas->rsus);
                    }
                }
            }
        }
    } else
        generateRSU(numRSU);
}

void TAS::generateRSU(int num)
{
    for (int i = 1; i <= num; i++) {
        std::string RSU = "RSU" + std::to_string(i);
        int long id = random();
        L3Address ip = L3AddressResolver().resolve(RSU.c_str());
        RSUData rsu(id, ip.str(), RSU);

        if (!generateSecretKey(rsu.secretKey, sizeof(rsu.secretKey)))
            throw cRuntimeError("Failed to initialize secret key");
            rsus.push_back(rsu);

            Packet *packet = new Packet("RSU Secret Key");
            if (dontFragment)
                packet->addTag<FragmentationReq>()->setDontFragment(true);
            const auto& payload = makeShared<SecretPacket>();
            payload->setChunkLength(B(par("messageLength")));
            payload->setKey(bin2hex(rsu.secretKey, sizeof(rsu.secretKey)).c_str());
            payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
            packet->insertAtBack(payload);
            emit(packetSentSignal, packet);
            socket.sendTo(packet, ip, destPort);
            numSent++;
       }
}

void TAS::finish()
{
    recordScalar("packets sent", numSent);
    recordScalar("packets received", numReceived);
    ApplicationBase::finish();
}

void TAS::setRSU(std::vector<RSUData> rsu)
{
    rsus = rsu;
}

void TAS::setSocketOptions()
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

L3Address TAS::chooseDestAddr()
{
    int k = intrand(destAddresses.size());
    if (destAddresses[k].isUnspecified() || destAddresses[k].isLinkLocal()) {
        L3AddressResolver().tryResolve(destAddressStr[k].c_str(), destAddresses[k]);
    }
    return destAddresses[k];
}

void TAS::sendPacket()
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

void TAS::processStart()
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

void TAS::processSend()
{
    RegisterRSU();
}

void TAS::processStop()
{
    socket.close();
}

void TAS::handleMessageWhenUp(cMessage *msg)
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

void TAS::socketDataArrived(UdpSocket *socket, Packet *packet)
{
    std::string pkname = packet->getName();
    if (pkname == "RAPacket")
        handleRegistrationOfVehiclePacket(packet);
    else if (pkname == "TGT+ATvtgs+AUvtgs")
        handleVehicleToTGSPacket(packet);
    else
        processPacket(packet);
}

void TAS::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void TAS::socketClosed(UdpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION)
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void TAS::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

    char buf[100];
    sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
    getDisplayString().setTagArg("t", 0, buf);
}

void TAS::handleRegistrationOfVehiclePacket(Packet *pkt)
{
    L3Address remoteAddress = pkt->getTag<L3AddressInd>()->getSrcAddress();
    int srcPort = pkt->getTag<L4PortInd>()->getSrcPort();
    numReceived++;
    EV_ERROR << remoteAddress << endl;
    auto startT = std::chrono::high_resolution_clock::now();
    rcvdBytes += pkt->getByteLength();

    auto data = pkt->removeAtFront<RAPacket>();
    if (data != nullptr) {
        simtime_t start = data->getTag<CreationTimeTag>()->getCreationTime();
        std::ofstream file;
        file.open("results/end2end.csv", std::ios::app); // Open file in append mode
        if (file.is_open()) {
            file << (simTime() - start).dbl()<< "," << simTime().dbl() << "\n";
            file.close();
        }

        file.open("results/throughput.csv", std::ios::app);
        if (file.is_open()) {
            file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "TAS"<< "\n";
            file.close();
        }

        // Vehicle Attributes
        unsigned char vehicleSecret[16];
        if (!generateSecretKey(vehicleSecret, sizeof(vehicleSecret)))
            throw cRuntimeError("Problem getting vehicle Secret key");

        unsigned char sessionKey[16];
        RAND_bytes(sessionKey, 16);

        std::string vID = data->getVID();
        const char *serviceType = data->getService();
        std::string vIP = data->getVIP();
        double lifetime = data->getLifetime();

        // register vehicle
        VehicleData newVehicle(vID, serviceType, remoteAddress.str(), lifetime, vehicleSecret);
        vehicles.push_back(newVehicle);

        // encrypt
        std::string sessionKeyStr = bin2hex(sessionKey, sizeof(sessionKey));
        std::string ATplaintext = TGSname + "|" + std::to_string(simTime().dbl()) + "|" + std::to_string(lifetime) + "|" + sessionKeyStr;
        std::string ATencrypted = encrypt(ATplaintext, vehicleSecret);

        std::string TGTplaintext = vID + "|" + TGSname + "|" + std::to_string(simTime().dbl()) + "|" + remoteAddress.str() + "|" + std::to_string(lifetime) + "|" + sessionKeyStr;
        std::string TGTencrypted = encrypt(TGTplaintext, TGSSecretKey);

        const auto& payload = makeShared<TGTPacket>();
        payload->setChunkLength(B(par("messageLength")));
        payload->setAT(ATencrypted.c_str());
        payload->setTGT(TGTencrypted.c_str());
        payload->setKey(bin2hex(vehicleSecret, sizeof(vehicleSecret)).c_str());
        payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
        pkt->insertAtBack(payload);
        pkt->setName("AStV+TGT");
        pkt->clearTags();
        pkt->trim();
        emit(packetSentSignal, pkt);
        socket.sendTo(pkt, remoteAddress, srcPort);
        numSent++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto dura = std::chrono::duration_cast<std::chrono::microseconds>(end - startT).count();
    std::ofstream file;
    file.open("results/ComputationCost.csv", std::ios::app);
    if (file.is_open()) {
        file << dura << "," << "RA" << std::endl;
        file.close();
    }

    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open())
    {
        file << numSent << "," << numReceived << "," << "TAS" << std::endl;
        file.close();
    }
}

void TAS::handleVehicleToTGSPacket(Packet *pkt)
{
    numReceived++;
    auto start = std::chrono::high_resolution_clock::now();
    rcvdBytes += pkt->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);
    if (file.is_open()) {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "TAS" << "\n";
        file.close();
    }

    L3Address remoteAddress = pkt->getTag<L3AddressInd>()->getSrcAddress();
    auto data = pkt->removeAtFront<AttributePacket>();
    if (data != nullptr) {
        std::string TGT = data->getTGT();
        std::string AU = data->getAU();
        std::string lifetime = data->getLifetime();
        std::string service = data->getService();

        if (AU != tempAU) {
            // cache the AU
            tempAU = AU;
            // decrypt
            std::string decryptedTGT = decrypt(TGT, TGSSecretKey);
            std::string vehicleId, TGSname, timestamp, vehicleIP, lt, sessionKeyStr;
            std::stringstream sss(decryptedTGT);
            std::getline(sss, vehicleId, '|');
            std::getline(sss, TGSname, '|');
            std::getline(sss, timestamp, '|');
            std::getline(sss, vehicleIP, '|');
            std::getline(sss, lt, '|');
            std::getline(sss, sessionKeyStr, '|');

            unsigned char sessionKey[16];
            std::vector<unsigned char> sessionKeyBinary = hex2bin(sessionKeyStr);
            std::copy(sessionKeyBinary.begin(), sessionKeyBinary.end(), sessionKey);
            std::string decryptedAU = decrypt(AU, sessionKey);

            // check lifetime
            if (std::stod(lifetime) > (simTime().dbl() - std::stod(timestamp))) {
                std::string vehId, ts;
                std::stringstream ss(decryptedAU);
                std::getline(ss, vehId, '|');
                std::getline(ss, ts, '|');

                file.open("results/auth.csv", std::ios::app);
                if (file.is_open()) {
                    file << decryptedAU << "," << AU << std::endl;
                    file.close();
                }

                for (const auto& vehicle : vehicles) {
                    if (vehicleIP == vehicle.vehicleIp) {
                        if (vehicleId == vehId) {
                            // send to blockchain
                            saveBlock(AU, vehicleId);
                            // generate & send ticket
                            unsigned char serviceSession[16];
                            RAND_bytes(serviceSession, 16);
                            std::string serviceSessionStr = bin2hex(serviceSession, sizeof(serviceSession));
                            std::string ATplaintext = service + "|" + timestamp + "|" + lifetime + "|" + serviceSessionStr;
                            std::string ATencrypted = encrypt(ATplaintext, sessionKey);

                            for (const auto& rsu : rsus) {
                                if (rsu.RSUname == service) {
                                    std::string STplaintext = vehicleId + "|" + timestamp + "|" + vehicleIP + "|" + lifetime + "|" + serviceSessionStr;
                                    std::string STencrypted = encrypt(STplaintext, rsu.secretKey);
                                    generateTicket(ATencrypted, STencrypted, bin2hex(rsu.secretKey, sizeof(rsu.secretKey)), remoteAddress);
                                }
                            }
                            auto end = std::chrono::high_resolution_clock::now();
                            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

                            file.open("results/ComputationCost.csv", std::ios::app);
                            if (file.is_open()) {
                                file << duration << "," << "TAS" << std::endl;
                                file.close();
                            }
                        }
                    }
                }
            }
        }
    }
    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open()) {
        file << numSent << "," << numReceived << "," << "TAS" << std::endl;
        file.close();
    }
    delete pkt;
}

void TAS::saveBlock(std::string AU, std::string vehicleId)
{
    Packet *packet = new Packet();
        if (dontFragment)
    packet->addTag<FragmentationReq>()->setDontFragment(true);
    const auto& payload = makeShared<BlockchainPacket>();
    payload->setChunkLength(B(par("messageLength")));
    payload->setAU(AU.c_str());
    payload->setVehicleId(vehicleId.c_str());
    payload->setDestination("Okadai");
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    packet->insertAtBack(payload);
    packet->setName("ATvtgs");
    L3Address destAddr = chooseDestAddr();
    emit(packetSentSignal, packet);
    socket.sendTo(packet, destAddr, destPort);
    numSent++;
}

void TAS::generateTicket(std::string AT, std::string ST, std::string key, L3Address remoteAddress)
{
    Packet *packet = new Packet();
    if (dontFragment)
        packet->addTag<FragmentationReq>()->setDontFragment(true);
    const auto& payload = makeShared<TicketPacket>();
    payload->setChunkLength(B(par("messageLength")));
    payload->setAT(AT.c_str());
    payload->setST(ST.c_str());
    payload->setKey(key.c_str());
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    packet->insertAtBack(payload);
    packet->setName("Ticket");
    emit(packetSentSignal, packet);
    socket.sendTo(packet, remoteAddress, destPort);
    numSent++;
}

void TAS::processPacket(Packet *pk)
{
    emit(packetReceivedSignal, pk);
    EV_INFO << "Received packet: " << UdpSocket::getReceivedPacketInfo(pk) << endl;
    delete pk;
    numReceived++;
}

void TAS::handleStartOperation(LifecycleOperation *operation)
{
    clocktime_t start = std::max(startTime, getClockTime());
    if ((stopTime < CLOCKTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime)) {
        selfMsg->setKind(START);
        scheduleClockEventAt(start, selfMsg);
    }
}

void TAS::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(selfMsg);
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void TAS::handleCrashOperation(LifecycleOperation *operation)
{
    cancelClockEvent(selfMsg);
    socket.destroy(); // TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
}

} // namespace inet
