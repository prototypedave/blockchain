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

#include "TicketGrantingServer.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/common/L4PortTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/applications/NewBlockchain/define_m.h"
#include "inet/applications/NewBlockchain/Ticket/TGSApp.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include <chrono>

namespace inet {

Define_Module(TGS);

void TGS::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        // init statistics
        numEchoed = 0;
        numReceived = 0;
        WATCH(numEchoed);
    }
    RAND_bytes(serviceSecret, sizeof(serviceSecret));
}

void TGS::handleMessageWhenUp(cMessage *msg)
{
    socket.processMessage(msg);
}

void TGS::socketDataArrived(UdpSocket *socket, Packet *pk)
{
    // determine its source address/port
    std::string pkname = pk->getName();
    if (pkname == "TGT + AUvtsg + ATvtgs")
    {
        handlePacketFromVehicle(socket, pk);
        numReceived++;
    }
}

void TGS::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void TGS::socketClosed(UdpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION)
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void TGS::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

    char buf[40];
    sprintf(buf, "echoed: %d pks", numEchoed);
    getDisplayString().setTagArg("t", 0, buf);
}

void TGS::finish()
{
    ApplicationBase::finish();
}

void TGS::handleStartOperation(LifecycleOperation *operation)
{
    socket.setOutputGate(gate("socketOut"));
    int localPort = par("localPort");
    socket.bind(localPort);
    MulticastGroupList mgl = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this)->collectMulticastGroups();
    socket.joinLocalMulticastGroups(mgl);
    socket.setCallback(this);
}

void TGS::handleStopOperation(LifecycleOperation *operation)
{
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void TGS::handleCrashOperation(LifecycleOperation *operation)
{
    if (operation->getRootModule() != getContainingNode(this)) // closes socket when the application crashed only
        socket.destroy(); // TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
    socket.setCallback(nullptr);
}

void TGS::sendPacket(std::string message, std::string vehId, std::string id)
{

    cModule *appModule = findModuleByPath(par("path"));
    if (appModule != nullptr)
    {
        TGSApp *app = check_and_cast<TGSApp *>(appModule);
        if (app != nullptr)
        {
            app->saveToBlockChain(message, vehId, id);
            EV_DETAIL << "Sending Message to TGSApp" << std::endl;
        }
    }
}

void TGS::handlePacketFromVehicle(UdpSocket *socket, Packet *packet)
{
    EV_DETAIL << "RECEIVED" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    rcvdBytes += packet->getByteLength();

    std::ofstream file;
    file.open("results/throughput.csv", std::ios::app);

    if (file.is_open())
    {
        file << rcvdBytes / simTime().dbl() << "," << simTime().dbl() << "," << "TGS" << "\n";
        file.close();
    }

    // determine its source address/port
    L3Address remoteAddress = packet->getTag<L3AddressInd>()->getSrcAddress();
    int srcPort = packet->getTag<L4PortInd>()->getSrcPort();

    // Remove chunks
    auto payloadTGT = packet->removeAtFront<TGT>();
    if(payloadTGT != nullptr)
    {
        // decrypt TGT
        std::string TGT = payloadTGT->getTGT();
        std::string skTGT = payloadTGT->getSecret();

        unsigned char secretTGT[16];
        std::vector<unsigned char> secKeyBin = hex2bin(skTGT);
        std::copy(secKeyBin.begin(), secKeyBin.end(), secretTGT);

        std::string decipherTGT = aesDecrypt(TGT, secretTGT);
        std::string vehId, idTGS, timestampTGT, IP, lt, sesKey;
        std::stringstream sss(decipherTGT);
        std::getline(sss, vehId, '|');
        std::getline(sss, idTGS, '|');
        std::getline(sss, timestampTGT, '|');
        std::getline(sss, IP, '|');
        std::getline(sss, lt, '|');
        std::getline(sss, sesKey, '|');

        auto payloadAU = packet->removeAtFront<VehicleAU>();
        if (payloadAU != nullptr)
        {
            // decrypt AU to get ID
            std::string AU = payloadAU->getAU();
            std::string sk = payloadAU->getSecret();

            unsigned char secret[16];
            std::vector<unsigned char> sKBin = hex2bin(sk);
            std::copy(sKBin.begin(), sKBin.end(), secret);

            std::string decipher = aesDecrypt(AU, secret);
            // split back to individual variables
            std::string vId, timestamp;
            std::stringstream ss(decipher);
            std::getline(ss, vId, '|');
            std::getline(ss, timestamp, '|');

            file.open("results/auth.csv", std::ios::app);
            if (file.is_open())
            {
                file << decipher << "," << AU << std::endl;
                file.close();
            }

            if (std::find(authMessages.begin(), authMessages.end(), AU) == authMessages.end())
            {
                auto payloadAT = packet->removeAtFront<VehicleAT>();
                if (payloadAT != nullptr)
                {
                    if (vehId == vId && std::stoi(lt) > (std::time(0) - std::stoi(timestamp)))
                    {
                        int ids = payloadAT->getServiceName();
                        // save to cache AU
                        authMessages.push_back(AU);
                        // send the AU to blockchain
                        sendPacket(AU, vehId, idTGS);
                        numEchoed++;

                        // ATTGSTV
                        unsigned char serviceKey[16];
                        RAND_bytes(serviceKey, sizeof(serviceKey));

                        std::string serKey = bin2hex(serviceKey, sizeof(serviceKey));

                        unsigned char sKey[16];
                        std::vector<unsigned char> scKeyBin = hex2bin(sesKey);
                        std::copy(scKeyBin.begin(), scKeyBin.end(), sKey);

                        std::string text = std::to_string(ids) + "|" + std::to_string(std::time(0)) + "|" + std::to_string(std::stoi(lt) - (std::time(0) - std::stoi(timestamp))) + "|" + serKey;
                        std::string cipher = aesEncrypt(text, sKey);

                        const auto& ATtgs = makeShared<TGSAT>();
                        ATtgs->setChunkLength(B(170));
                        ATtgs->setAT(cipher.c_str());
                        ATtgs->setSecret(sesKey.c_str());
                        ATtgs->addTag<CreationTimeTag>()->setCreationTime(simTime());
                        packet->insertAtBack(ATtgs);

                        // ST
                        unsigned char sessionTicket[16];
                        RAND_bytes(sessionTicket, sizeof(sessionTicket));
                        std::string st = bin2hex(sessionTicket, sizeof(sessionTicket));

                        std::string plaintext = st + "|" + vehId + "|" + std::to_string(ids) + "|" + std::to_string(std::time(0)) + "|" + IP + "|" + std::to_string(std::stoi(lt) - (std::time(0) - std::stoi(timestamp))) + "|" + serKey;
                        std::string ciphertext = aesEncrypt(plaintext, serviceSecret);

                        std::string seckey = bin2hex(serviceSecret, sizeof(serviceSecret));

                        const auto& sTicket = makeShared<STPacket>();
                        sTicket->setChunkLength(B(170));
                        sTicket->setST(ciphertext.c_str());
                        sTicket->setSecret(seckey.c_str());

                        sTicket->addTag<CreationTimeTag>()->setCreationTime(simTime());
                        //packet->insertAtBack(sTicket);
                        packet->setName("Ticket");

                        Packet *pk = new Packet("ST");
                            pk->addTag<FragmentationReq>()->setDontFragment(true);
                        pk->insertAtBack(sTicket);

                        packet->clearTags();
                        packet->trim();

                        // statistics
                        numEchoed++;
                        emit(packetSentSignal, packet);
                        socket->sendTo(packet, remoteAddress, srcPort);
                        numEchoed++;
                        emit(packetSentSignal, pk);
                        socket->sendTo(pk, remoteAddress, srcPort);
                        auto end = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

                        file.open("results/ComputationCost.csv", std::ios::app);
                        if (file.is_open())
                        {
                            file << duration << "," << "TGS" << std::endl;
                            file.close();
                        }
                    }
                }
                else
                {
                    // invalid packet
                    delete packet;
                    return;
                }
            }
            else
            {
                // possible replay attack
                delete packet;
                return;
            }
        }
        else
        {
            // delete packet corrupted packet
            delete packet;
            return;
        }
    }
    else
    {
        // return packet
        packet->clearTags();
        packet->trim();

        // statistics
        numEchoed++;
        emit(packetSentSignal, packet);
        //socket->sendTo(packet, remoteAddress, srcPort);
    }
    file.open("results/packetCount.csv", std::ios::app);
    if (file.is_open())
    {
        file << numEchoed << "," << numReceived << "," << "TGS" << std::endl;
        file.close();
    }
}

std::string TGS::aesEncrypt(const std::string &plainText, const unsigned char *key) {
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

std::string TGS::aesDecrypt(const std::string &cipherText, const unsigned char *key) {
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
