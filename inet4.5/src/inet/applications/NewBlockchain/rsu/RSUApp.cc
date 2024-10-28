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

#include "RSUApp.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/common/L4PortTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/applications/NewBlockchain/define_m.h"
#include <chrono>
#include "inet/common/TimeTag_m.h"

namespace inet {

Define_Module(RSUApp);

void RSUApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        // init statistics
        numEchoed = 0;
        numReceived = 0;
        WATCH(numReceived);
        WATCH(numEchoed);
    }
}

void RSUApp::handleMessageWhenUp(cMessage *msg)
{
    socket.processMessage(msg);
}

void RSUApp::socketDataArrived(UdpSocket *socket, Packet *pk)
{
    std::string pkname = pk->getName();
    if (pkname == "RSU")
    {
        handleAuthenticatorFromVehicle(socket, pk);
        numReceived++;
    }
}

void RSUApp::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void RSUApp::socketClosed(UdpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION)
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void RSUApp::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

    char buf[40];
    sprintf(buf, "echoed: %d pks", numEchoed);
    getDisplayString().setTagArg("t", 0, buf);
}

void RSUApp::finish()
{
    ApplicationBase::finish();
}

void RSUApp::handleStartOperation(LifecycleOperation *operation)
{
    socket.setOutputGate(gate("socketOut"));
    int localPort = par("localPort");
    socket.bind(localPort);
    MulticastGroupList mgl = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this)->collectMulticastGroups();
    socket.joinLocalMulticastGroups(mgl);
    socket.setCallback(this);
}

void RSUApp::handleStopOperation(LifecycleOperation *operation)
{
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void RSUApp::handleCrashOperation(LifecycleOperation *operation)
{
    if (operation->getRootModule() != getContainingNode(this)) // closes socket when the application crashed only
        socket.destroy(); // TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
    socket.setCallback(nullptr);
}

void RSUApp::handleAuthenticatorFromVehicle(UdpSocket *socket, Packet *packet)
{
    EV_DETAIL << "received" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    rcvdBytes += packet->getByteLength();
    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);

    if (file.is_open())
    {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "RSU" << "\n";
        file.close();
    }

    // determine its source address/port
    L3Address remoteAddress = packet->getTag<L3AddressInd>()->getSrcAddress();
    int srcPort = packet->getTag<L4PortInd>()->getSrcPort();

    auto payload = packet->removeAtFront<STPacket>();
    if (payload != nullptr)
    {
        std::string ST = payload->getST();
        std::string sk = payload->getSecret();
        simtime_t k = payload->getTag<CreationTimeTag>()->getCreationTime();

        unsigned char secret[16];
        std::vector<unsigned char> secKeyBin = hex2bin(sk);
        std::copy(secKeyBin.begin(), secKeyBin.end(), secret);

        std::string decipher = aesDecrypt(ST, secret);
        std::string st, vehId, serviceName, timestamp, ip, lt, serKey;
        std::stringstream ss(decipher);
        std::getline(ss, st, '|');
        std::getline(ss, vehId, '|');
        std::getline(ss, serviceName, '|');
        std::getline(ss, timestamp, '|');
        std::getline(ss, ip, '|');
        std::getline(ss, lt, '|');
        std::getline(ss, serKey, '|');

        unsigned char skey[16];
        std::vector<unsigned char> secKBin = hex2bin(serKey);
        std::copy(secKBin.begin(), secKBin.end(), skey);

        auto payloadAU = packet->removeAtFront<VehicleToServerAU>();
        if (payloadAU != nullptr)
        {
            packet->insertAtBack(payload);

            std::string AU = payloadAU->getAU();
            std::string d = payloadAU->getSecret();
            std::string decipherText = aesDecrypt(AU, skey);
            std::string vehicleId, timstamp;
            std::stringstream sss(decipherText);
            std::getline(sss, vehicleId, '|');
            std::getline(sss, timstamp, '|');

            EV_DETAIL << serKey << " : " << d << std::endl;
            if (vehId == vehicleId)
            {

                // create own AU
                std::string plaintext = serviceName + "|" + std::to_string(std::time(0));
                std::string cipher = aesEncrypt(plaintext, skey);
                const auto& load = makeShared<ServerToVehicleAU>();
                load->setChunkLength(B(144));
                load->setAU(cipher.c_str());    // will change tO id oF the given RSU
                load->setSecret(serKey.c_str());
                load->setRsuId(getId());
                load->addTag<CreationTimeTag>()->setCreationTime(k);
                packet->insertAtBack(load);
                packet->setName("Confirm-Ticket");

                packet->clearTags();
                packet->trim();

                // statistics
                numEchoed++;
                emit(packetSentSignal, packet);
                // send back
                socket->sendTo(packet, remoteAddress, srcPort);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

                file.open("results/ComputationCost.csv", std::ios::app);
                if (file.is_open())
                {
                    file << duration << "," << "RSU" << std::endl;
                    file.close();
                }
            }
        }
    }
    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open())
    {
        file << numEchoed << "," << numReceived << "," << "RSU" << std::endl;
        file.close();
    }
}

std::string RSUApp::aesEncrypt(const std::string &plainText, const unsigned char *key) {
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

std::string RSUApp::aesDecrypt(const std::string &cipherText, const unsigned char *key) {
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
