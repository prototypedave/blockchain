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

#include "AuthenticationServer.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/common/L4PortTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/applications/NewBlockchain/define_m.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/common/TimeTag_m.h"
#include <chrono>
namespace inet {

Define_Module(AS);

void AS::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        // init statistics
        numEchoed = 0;
        numReceived = 0;
        WATCH(numEchoed);
        WATCH(numReceived);
    }
    tgsId = par("tgsId");
    OfflineRegistration(par("vFile"), par("rFile"));
    RAND_bytes(TGSkey, 16);
}

void AS::handleMessageWhenUp(cMessage *msg)
{
    socket.processMessage(msg);
}

void AS::socketDataArrived(UdpSocket *socket, Packet *pk)
{
    std::string pkname = pk->getName();
    if (pkname == "RA")
    {
        handleVehicleRA(socket, pk);
        numReceived++;
    }
}

void AS::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void AS::socketClosed(UdpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION)
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void AS::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

    char buf[40];
    sprintf(buf, "echoed: %d pks", numEchoed);
    getDisplayString().setTagArg("t", 0, buf);
}

void AS::finish()
{
    ApplicationBase::finish();
}

void AS::handleStartOperation(LifecycleOperation *operation)
{
    socket.setOutputGate(gate("socketOut"));
    int localPort = par("localPort");
    socket.bind(localPort);
    MulticastGroupList mgl = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this)->collectMulticastGroups();
    socket.joinLocalMulticastGroups(mgl);
    socket.setCallback(this);
}

void AS::handleStopOperation(LifecycleOperation *operation)
{
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void AS::handleCrashOperation(LifecycleOperation *operation)
{
    if (operation->getRootModule() != getContainingNode(this)) // closes socket when the application crashed only
        socket.destroy(); // TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
    socket.setCallback(nullptr);
}

void AS::handleVehicleRA(UdpSocket *socket, Packet *packet)
{
    auto startT = std::chrono::high_resolution_clock::now();
    rcvdBytes += packet->getByteLength();
    // determine its source address/port
    L3Address remoteAddress = packet->getTag<L3AddressInd>()->getSrcAddress();
    int srcPort = packet->getTag<L4PortInd>()->getSrcPort();

    auto payload = packet->removeAtFront<VehicleRA>();
    if (payload != nullptr)
    {
        simtime_t start = payload->getTag<CreationTimeTag>()->getCreationTime();
        // End-to-End Delay
        std::ofstream file;
        file.open("results/end2end.csv", std::ios::app); // Open file in append mode

        if (file.is_open()) {
            file << (simTime() - start).dbl()<< "," << simTime().dbl() << "\n";
            file.close();
        }

        file.open("results/throughput.csv", std::ios::app);

        if (file.is_open())
        {
            file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "AS" << "\n";
            file.close();
        }

        int VehId = payload->getVehId();
        int rsuId = payload->getRsuId();
        std::string userIp = payload->getUserIp();
        int reqLt = payload->getRequestedLifetime();

        // create vehicle secret key
        if (checkRecord(VehId)){
            // create session key for this vehicle
            unsigned char sessionKey[16];
            RAND_bytes(sessionKey, 16);

            // record timestamp
            std::string timestamp = std::to_string(std::time(0));

            // session key to string
            std::string sesKey = bin2hex(sessionKey, sizeof(sessionKey));

            // create plaintext: TGS ID, TIMESTAMP, LIFETIME, SESSION KEY
            std::string plaintext = std::to_string(tgsId) + "|" + timestamp + "|" + std::to_string(reqLt) + "|" + sesKey;

            // LOOP to get vehcle secret key
            for (auto& record : vehicleRecords) {
                if (record.id == VehId)
                {
                    EV_DETAIL << "Received RA from vehicle : "<< record.vehicleID << " IP : " << remoteAddress << std::endl;
                    std::string remo = remoteAddress.str();
                    record.ip = remo;

                    // encrypt
                    std::string cipher = aesEncrypt(plaintext, record.secretKey);
                    std::string secKey = bin2hex(record.secretKey, sizeof(record.secretKey));

                    // send AT
                    const auto& payloadAT = makeShared<ASAT>();
                    payloadAT->setAT(cipher.c_str());
                    payloadAT->setSecret(secKey.c_str());
                    payloadAT->setVehId(std::stoi(record.vehicleID));
                    //payloadAT->setIp(remoteAddress.str().c_str());
                    payloadAT->setChunkLength(B(154));
                    payloadAT->addTag<CreationTimeTag>()->setCreationTime(simTime());
                    packet->insertAtBack(payloadAT);
                    packet->setName("ATas");

                    packet->clearTags();
                    packet->trim();

                    // statistics
                    numEchoed++;
                    emit(packetSentSignal, packet);
                    // send back
                    socket->sendTo(packet, remoteAddress, srcPort);

                    // TGT generation
                    std::string text = record.vehicleID + "|" + std::to_string(tgsId) + "|"  + timestamp + "|" + remoteAddress.str() + "|" + std::to_string(reqLt) + "|" + sesKey;
                    std::string ciphertext = aesEncrypt(text, TGSkey);

                    std::string tgkey = bin2hex(TGSkey, sizeof(TGSkey));

                    Packet *pk = new Packet();
                    pk->addTag<FragmentationReq>()->setDontFragment(true);
                    const auto& load = makeShared<TGT>();
                    load->setChunkLength(B(144));
                    load->setTGT(ciphertext.c_str());
                    load->setSecret(tgkey.c_str());
                    load->addTag<CreationTimeTag>()->setCreationTime(simTime());
                    pk->insertAtBack(load);
                    pk->setName("TGT");
                    pk->clearTags();
                    pk->trim();

                    // statistics
                    numEchoed++;
                    emit(packetSentSignal, pk);
                    // send back
                    socket->sendTo(pk, remoteAddress, srcPort);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto dura = std::chrono::duration_cast<std::chrono::microseconds>(end - startT).count();

                    file.open("results/ComputationCost.csv", std::ios::app);
                    if (file.is_open())
                    {
                        file << dura << "," << "RA" << std::endl;
                        file.close();
                    }

                    break;
                }
            }

        }
        else {
            // vehicle not recognized
            packet->insertAtBack(payload);
            packet->clearTags();
            packet->trim();

            // statistics
            numEchoed++;
            emit(packetSentSignal, packet);
            return;
        }
    }
    else {
        // no RA found return
        packet->clearTags();
        packet->trim();

        // statistics
        numEchoed++;
        emit(packetSentSignal, packet);
    }

    std::ofstream file;
    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open())
    {
        file << numEchoed << "," << numReceived << "," << "AS" << std::endl;
        file.close();
    }
}

// registers vehicle data and rsu
void AS::OfflineRegistration(const char * vFiles, const char * rFiles)
{
    // vehicle data
    std::ifstream veh(vFiles);
    if (!veh.is_open()) {
        EV_ERROR << "Error opening file: " << vFiles << std::endl;
    }

    std::string line;
    std::getline(veh, line);
    int count = 0;
    while (std::getline(veh, line)) {
        std::stringstream ss(line);
        std::string field;

        VehicleData record;
        std::getline(ss, record.vehicleID, ',');
        std::getline(ss, record.userID, ',');
        std::getline(ss, record.password, ',');
        std::getline(ss, record.source, ',');
        std::getline(ss, record.destination, ',');
        std::getline(ss, record.serviceType, ',');
        std::getline(ss, record.role, ',');
        record.id = count;
        RAND_bytes(record.secretKey, 16);  // vehicle secret
        vehicleRecords.push_back(record);
        count++;
    }

    veh.close();

    std::ifstream rsu(rFiles);
    if (!rsu.is_open()) {
        EV_ERROR << "Error opening file: " << vFiles << std::endl;
    }

    int sum = 0;
    std::string line2;
    std::getline(rsu, line2);
    while (std::getline(rsu, line2)) {
        std::stringstream ss(line2);
        std::string field;

        RSUData record;
        std::getline(ss, record.location, ',');
        std::getline(ss, record.macAddress, ',');
        std::getline(ss, record.ip, ',');
        std::getline(ss, record.rsuID, ',');
        record.id = sum;
        RAND_bytes(record.secretKey, sizeof(record.secretKey));
        rsuRecords.push_back(record);
        sum++;
    }
    rsu.close();
}

// checks if a vehicle is already registered
bool AS::checkRecord(int vehicleId)
{
    for (const auto& record : vehicleRecords) {
        if (record.id == vehicleId)
        {
            return true;
        }
    }
    return false;
}

// Encryption function
std::string AS::aesEncrypt(const std::string &plainText, const unsigned char *key) {
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

// Decryption function
std::string AS::aesDecrypt(const std::string &cipherText, const unsigned char *key) {
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

} // namespace inet
