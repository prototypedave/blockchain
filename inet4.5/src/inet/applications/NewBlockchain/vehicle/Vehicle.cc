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

#include "Vehicle.h"

#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/applications/NewBlockchain/define_m.h"
#include <chrono>

namespace inet {

Define_Module(Vehicle);

Vehicle::~Vehicle()
{
    cancelAndDelete(selfMsg);
}

void Vehicle::initialize(int stage)
{
    ClockUserModuleMixin::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        numSent = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numReceived);

        localPort = par("localPort");
        destPort = par("destPort");
        tgsPort = par("tgsPort");
        rsuPort = par("rsuPort");
        startTime = par("startTime");
        stopTime = par("stopTime");
        packetName = par("packetName");
        dontFragment = par("dontFragment");
        vehId = par("vehId");
        rsuId = par("rsuId");
        if (stopTime >= CLOCKTIME_ZERO && stopTime < startTime)
            throw cRuntimeError("Invalid startTime/stopTime parameters");
        selfMsg = new ClockEvent("sendTimer");
    }
}

void Vehicle::finish()
{
    recordScalar("packets sent", numSent);
    recordScalar("packets received", numReceived);
    ApplicationBase::finish();
}

void Vehicle::setSocketOptions()
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

L3Address Vehicle::chooseDestAddr()
{
    int k = intrand(destAddresses.size());
    if (destAddresses[k].isUnspecified() || destAddresses[k].isLinkLocal()) {
        L3AddressResolver().tryResolve(destAddressStr[k].c_str(), destAddresses[k]);
    }
    return destAddresses[k];
}

L3Address Vehicle::chooseTgsAddr()
{
    int k = intrand(tgsAddresses.size());
    if (tgsAddresses[k].isUnspecified() || tgsAddresses[k].isLinkLocal()) {
        L3AddressResolver().tryResolve(tgsAddressStr[k].c_str(), tgsAddresses[k]);
    }
    return tgsAddresses[k];
}

L3Address Vehicle::chooseRSUAddr()
{
    std::vector<L3Address> rsuAddresses;
    std::vector<std::string> rsuAddressStr;
    const char *destAddrs = par("rsuDest");

    cStringTokenizer tokenizer(destAddrs);
    const char *token;

    while ((token = tokenizer.nextToken()) != nullptr) {
        rsuAddressStr.push_back(token);
        L3Address result;
        L3AddressResolver().tryResolve(token, result);
        if (result.isUnspecified())
            EV_ERROR << "cannot resolve destination address: " << token << endl;
                rsuAddresses.push_back(result);
        }

    int k = intrand(rsuAddresses.size());
    if (rsuAddresses[k].isUnspecified() || rsuAddresses[k].isLinkLocal()) {
        L3AddressResolver().tryResolve(rsuAddressStr[k].c_str(), rsuAddresses[k]);
    }
    return rsuAddresses[k];
}

void Vehicle::sendPacket()
{
    strt = simTime();
    startT = std::chrono::high_resolution_clock::now();
    Packet *packet = new Packet("RA");
    if (dontFragment)
        packet->addTag<FragmentationReq>()->setDontFragment(true);
    const auto& payload = makeShared<VehicleRA>();
    payload->setChunkLength(B(60));
    payload->setVehId(vehId);
    payload->setRsuId(rsuId);
    payload->setUserIp((L3Address().str()).c_str());
    payload->setRequestedLifetime(120);
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    packet->insertAtBack(payload);
    L3Address destAddr = chooseDestAddr();
    emit(packetSentSignal, packet);
    socket.sendTo(packet, destAddr, destPort);
    numSent++;
}

void Vehicle::processStart()
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

    // do the same for tgs addresses
    const char *tgsAddrs = par("tgsAddresses");
    cStringTokenizer tokenizer2(tgsAddrs);
    const char *token2;

    while ((token2 = tokenizer2.nextToken()) != nullptr) {
        tgsAddressStr.push_back(token2);
        L3Address result2;
        L3AddressResolver().tryResolve(token2, result2);
        if (result2.isUnspecified())
            EV_ERROR << "cannot resolve destination address: " << token2<< endl;
        tgsAddresses.push_back(result2);
    }

    if (!destAddresses.empty()) {
        selfMsg->setKind(SEND);
        processSend();
        sendPacket();   // send RA only once
    }
    else {
        if (stopTime >= CLOCKTIME_ZERO) {
            selfMsg->setKind(STOP);
            scheduleClockEventAt(stopTime, selfMsg);
        }
    }
}

void Vehicle::processSend()
{
    clocktime_t d = par("sendInterval");
    if (stopTime < CLOCKTIME_ZERO || getClockTime() + d < stopTime) {
        selfMsg->setKind(SEND);
        scheduleClockEventAfter(d, selfMsg);
    }
    else {
        selfMsg->setKind(STOP);
        scheduleClockEventAt(stopTime, selfMsg);
    }
}

void Vehicle::processStop()
{
    socket.close();
}

void Vehicle::handleMessageWhenUp(cMessage *msg)
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

void Vehicle::socketDataArrived(UdpSocket *socket, Packet *packet)
{
    // process incoming packet
    //processPacket(packet);
    std::string pkname = packet->getName();
    if (pkname == "ATas")
    {
        handleASAT(packet);
        numReceived++;
    }
    else if (pkname == "TGT")
    {
        handleTGT(packet);
        numReceived++;
    }
    else if (pkname == "Ticket")
    {
        handleTicket(packet);
        numReceived++;
    }
    else if (pkname == "Confirm-Ticket")
    {
        handleConfirmation(packet);
        numReceived++;
    }
    else if (pkname == "ST")
    {
        handleST(packet);
        numReceived++;
    }
}

void Vehicle::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void Vehicle::socketClosed(UdpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION)
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void Vehicle::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

    char buf[100];
    sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
    getDisplayString().setTagArg("t", 0, buf);
}

void Vehicle::processPacket(Packet *pk)
{
    emit(packetReceivedSignal, pk);
    EV_INFO << "Received packet: " << UdpSocket::getReceivedPacketInfo(pk) << endl;
    delete pk;
    numReceived++;
}

void Vehicle::handleStartOperation(LifecycleOperation *operation)
{
    clocktime_t start = std::max(startTime, getClockTime());
    if ((stopTime < CLOCKTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime)) {
        selfMsg->setKind(START);
        scheduleClockEventAt(start, selfMsg);
    }
}

void Vehicle::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(selfMsg);
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void Vehicle::handleCrashOperation(LifecycleOperation *operation)
{
    cancelClockEvent(selfMsg);
    socket.destroy(); // TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
}

std::string Vehicle::aesEncrypt(const std::string &plainText, const unsigned char *key) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

    std::vector<unsigned char> cipherText(plainText.size() + AES_BLOCK_SIZE);
    int len = 0, cipherTextLen = 0;

    EVP_EncryptUpdate(ctx, cipherText.data(), &len, (unsigned char*)plainText.c_str(), plainText.size());
    cipherTextLen = len;

    EVP_EncryptFinal_ex(ctx, cipherText.data() + len, &len);
    cipherTextLen += len;

    EVP_CIPHER_CTX_free(ctx);

    return bin2hex(cipherText.data(), cipherTextLen);
}

std::string Vehicle::aesDecrypt(const std::string &cipherText, const unsigned char *key) {
    std::vector<unsigned char> binCipherText = hex2bin(cipherText);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

    std::vector<unsigned char> plainText(binCipherText.size());
    int len = 0, plainTextLen = 0;

    EVP_DecryptUpdate(ctx, plainText.data(), &len, binCipherText.data(), binCipherText.size());
    plainTextLen = len;

    EVP_DecryptFinal_ex(ctx, plainText.data() + len, &len);
    plainTextLen += len;

    EVP_CIPHER_CTX_free(ctx);

    return std::string((char*)plainText.data(), plainTextLen);
}

void Vehicle::handleASAT(Packet *packet)
{
    rcvdBytes += packet->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);

    if (file.is_open())
    {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "Vehicle" << "\n";
        file.close();
    }

    auto payload = packet->removeAtFront<ASAT>();
    if (payload != nullptr)
    {
        std::string cipher = payload->getAT();
        std::string sk = payload->getSecret();
        vehId = payload->getVehId(); // update id with actual id
        unsigned char secret[16];
        std::vector<unsigned char> secKeyBin = hex2bin(sk);
        std::copy(secKeyBin.begin(), secKeyBin.end(), secret);

        // decrypt AT to get contents
        std::string decipher = aesDecrypt(cipher, secret);
        std::string idtgs, timestamp, lifetime, sesKey;
        std::stringstream ss(decipher);
        std::getline(ss, idtgs, '|');
        std::getline(ss, timestamp, '|');
        std::getline(ss, lifetime, '|');
        std::getline(ss, sesKey, '|');

        // save cache of the current session
        Session sess;
        sess.tgsId = std::stoi(idtgs);
        //sess.ip = payload->getIp();
        sess.lifetime = std::stoi(lifetime) - (std::time(0) - std::stoi(timestamp));
        sess.secretKey = sk;
        sess.sessionId = sesKey;
        sess.vehId = vehId;
        sess.timestamp = timestamp;

        SessionCache.push_back(sess);

        delete packet;
    }
    else
    {
        delete packet;
    }
}

void Vehicle::handleTGT(Packet *packet)
{
    rcvdBytes += packet->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);

    if (file.is_open())
    {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "Vehicle" << "\n";
        file.close();
    }

    auto payload = packet->removeAtFront<TGT>();
    if (payload != nullptr)
    {
        // this message is only forwarded to TGS
        if (!SessionCache.empty())
        {
            // check if cache of this session is already present
            Session& lastSession = SessionCache.back();

            // first AU
            unsigned char secret[16];
            std::vector<unsigned char> secKeyBin = hex2bin(lastSession.sessionId);
            std::copy(secKeyBin.begin(), secKeyBin.end(), secret);

            std::string plaintext = std::to_string(lastSession.vehId) + "|" + lastSession.timestamp;
            std::string cipher = aesEncrypt(plaintext, secret);

            const auto& load = makeShared<VehicleAU>();
            load->setChunkLength(B(144));
            load->setAU(cipher.c_str());
            load->setSecret(lastSession.sessionId.c_str());
            load->addTag<CreationTimeTag>()->setCreationTime(simTime());

            // vehicle AT
            const auto& data = makeShared<VehicleAT>();
            data->setChunkLength(B(20));
            data->setServiceName(rsuId);
            data->setLTReqTGT(lastSession.lifetime);
            data->addTag<CreationTimeTag>()->setCreationTime(simTime());

            Packet *pk = new Packet("TGT + AUvtsg + ATvtgs");
            if (dontFragment)
                pk->addTag<FragmentationReq>()->setDontFragment(true);
            pk->insertAtBack(payload);
            pk->insertAtBack(load);
            pk->insertAtBack(data);

            emit(packetSentSignal, pk);
            L3Address destAddr = chooseTgsAddr();
            socket.sendTo(pk, destAddr, tgsPort);
            numSent++;

            delete packet;
        }
        else {
            delete packet;  // AT not yet processed or invalid TGT request
        }
    }
    else
    {
        delete packet;  // free memory
    }

    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open())
    {
        file << numSent << "," << numReceived << "," << "Vehicle" << std::endl;
        file.close();
    }
}

void Vehicle::handleTicket(Packet *packet)
{
    rcvdBytes += packet->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);

    if (file.is_open())
    {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "Vehicle" << "\n";
        file.close();
    }

    auto payload = packet->removeAtFront<TGSAT>();
    EV_DETAIL << "this" << std::endl;
    if (payload != nullptr)
    {
        std::string AT = payload->getAT();
        const char * sk = payload->getSecret();
        unsigned char secret[16];
        std::vector<unsigned char> secKeyBin = hex2bin(sk);
        std::copy(secKeyBin.begin(), secKeyBin.end(), secret);

        std::string decipher = aesDecrypt(AT, secret);
        std::string idtgs, timestamp, lt, servKey;
        std::stringstream ss(decipher);
        std::getline(ss, idtgs, '|');
        std::getline(ss, timestamp, '|');
        std::getline(ss, lt, '|');
        std::getline(ss, servKey, '|');

        serKey = servKey;
        delete packet;
    }
    else {
        delete packet;
    }

    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open())
    {
        file << numSent << "," << numReceived << "," << "Vehicle" << std::endl;
        file.close();
    }
}

void Vehicle::handleST(Packet *packet)
{
    rcvdBytes += packet->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);

    if (file.is_open())
    {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "Vehicle" << "\n";
        file.close();
    }

    auto ticket = packet->removeAtFront<STPacket>();
    if (ticket != nullptr)
    {
        unsigned char skey[16];
        std::vector<unsigned char> secKBin = hex2bin(serKey);
        std::copy(secKBin.begin(), secKBin.end(), skey);
        std::string plaintext = std::to_string(vehId) + "|" + std::to_string(std::time(0));
        std::string cipher = aesEncrypt(plaintext, skey);

        Packet *pk = new Packet("RSU");
        if (dontFragment)
            pk->addTag<FragmentationReq>()->setDontFragment(true);
        pk->insertAtBack(ticket);

        const auto& load = makeShared<VehicleToServerAU>();
        load->setChunkLength(B(170));
        load->setAU(cipher.c_str());    // will change tO id oF the given RSU
        load->setSecret(serKey.c_str());
        load->addTag<CreationTimeTag>()->setCreationTime(simTime());
        pk->insertAtBack(load);

        L3Address destAddr = chooseRSUAddr();
        emit(packetSentSignal, pk);
        socket.sendTo(pk, destAddr, rsuPort);
        numSent++;

        delete packet;
    }

    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open())
    {
        file << numSent << "," << numReceived << "," << "Vehicle" << std::endl;
        file.close();
    }
}

void Vehicle::handleConfirmation(Packet *packet)
{
    rcvdBytes += packet->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);

    if (file.is_open())
    {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "Vehicle" << "\n";
        file.close();
    }


    auto payload = packet->removeAtFront<STPacket>();
    if (payload != nullptr)
    {
        std::string ST = payload->getST();

        auto payloadAUsm = packet->removeAtFront<ServerToVehicleAU>();
        if (payloadAUsm != nullptr)
        {
            std::string AU = payloadAUsm->getAU();
            const char * sk = payloadAUsm->getSecret();
            int rsuId = payloadAUsm->getRsuId();

            simtime_t start = payloadAUsm->getTag<CreationTimeTag>()->getCreationTime();

            // save to csv
            file.open("results/delay.csv", std::ios::app); // Open file in append mode

            if (file.is_open()) {
                file << (simTime() - strt).dbl() << "," << (simTime() - start).dbl() << "\n";
                file.close();
            }

            unsigned char secret[16];
            std::vector<unsigned char> secKeyBin = hex2bin(sk);
            std::copy(secKeyBin.begin(), secKeyBin.end(), secret);

            std::string decipher = aesDecrypt(AU, secret);
            std::string serviceName, timestamp;
            std::stringstream ss(decipher);
            std::getline(ss, serviceName, '|');
            std::getline(ss, timestamp, '|');

            if (serviceName == "RSU")
            {
                if (120 > (std::time(0) - std::stoi(timestamp)))
                {
                    TicketCache.push_back(ST);
                }
            }
        }
    }
    EV_INFO << "Received packet: " << UdpSocket::getReceivedPacketInfo(packet) << endl;
    delete packet;
    numReceived++;

    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open())
    {
        file << numSent << "," << numReceived << "," << "Vehicle" << std::endl;
        file.close();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - startT).count();

    file.open("results/ComputationCost.csv", std::ios::app);
    if (file.is_open())
    {
        file << duration << "," << "vehicle" << std::endl;
        file.close();
    }
}

} // namespace inet

