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

#include "BlockChain.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/common/L4PortTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/applications/NewBlockchain/define_m.h"
#include <chrono>
#include <fstream>

namespace inet {

Define_Module(BlockChainApp);

void BlockChainApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        // init statistics
        numEchoed = 0;
        WATCH(numEchoed);
    }
    keypair = generateKeyPair();
    publicKey = generatePublicKey(keypair);
}

void BlockChainApp::handleMessageWhenUp(cMessage *msg)
{
    socket.processMessage(msg);
}

void BlockChainApp::socketDataArrived(UdpSocket *socket, Packet *pk)
{
    std::string pkname = pk->getName();
    if (pkname == "AUvtTGS")
    {
        handleAUfromServer(socket, pk);
    }
}

void BlockChainApp::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void BlockChainApp::socketClosed(UdpSocket *socket)
{
    if (operationalState == State::STOPPING_OPERATION)
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void BlockChainApp::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

    char buf[40];
    sprintf(buf, "echoed: %d pks", numEchoed);
    getDisplayString().setTagArg("t", 0, buf);
}

void BlockChainApp::finish()
{
    ApplicationBase::finish();
}

void BlockChainApp::handleStartOperation(LifecycleOperation *operation)
{
    socket.setOutputGate(gate("socketOut"));
    int localPort = par("localPort");
    socket.bind(localPort);
    MulticastGroupList mgl = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this)->collectMulticastGroups();
    socket.joinLocalMulticastGroups(mgl);
    socket.setCallback(this);
}

void BlockChainApp::handleStopOperation(LifecycleOperation *operation)
{
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void BlockChainApp::handleCrashOperation(LifecycleOperation *operation)
{
    if (operation->getRootModule() != getContainingNode(this)) // closes socket when the application crashed only
        socket.destroy(); // TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
    socket.setCallback(nullptr);
}

void BlockChainApp::handleAUfromServer(UdpSocket *socket, Packet *packet)
{
    auto start = std::chrono::high_resolution_clock::now();
    L3Address remoteAddress = packet->getTag<L3AddressInd>()->getSrcAddress();
    int srcPort = packet->getTag<L4PortInd>()->getSrcPort();

    auto payload = packet->removeAtFront<CHAIN>();
    if (payload != nullptr)
    {
        std::string AU = payload->getAU();
        std::string vehId = payload->getVehId();
        std::string destId = payload->getDestId();

        // check if AU exists in the block
        if (blockchain.containsMessage(AU)) {
            EV_DETAIL << "Auth Message already in the blockchain." << std::endl;
            packet->insertAtBack(payload);
            packet->setName("Block-Confirmed");
            packet->clearTags();
            packet->trim();

            // statistics
            numEchoed++;
            emit(packetSentSignal, packet);
            // send back
            socket->sendTo(packet, remoteAddress, srcPort);
        }

        // if not add message
        Transaction tx1(vehId, destId, AU, keypair);
        if (tx1.isValid(publicKey)) {
            std::vector<Transaction> transactions = { tx1 };
            Block newBlock(blockchain.getLatestBlock().index + 1, blockchain.getLatestBlock().hash, transactions);
            blockchain.addBlock(std::move(newBlock));
            packet->insertAtBack(payload);
            packet->setName("Block-Confirmed");
            packet->clearTags();
            packet->trim();

            // statistics
            numEchoed++;
            emit(packetSentSignal, packet);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            std::ofstream file;
            file.open("results/ComputationCost.csv", std::ios::app);
            if (file.is_open())
            {
                file << duration << "," << "Block" << std::endl;
                file.close();
            }
            // send back
            socket->sendTo(packet, remoteAddress, srcPort);
        } else {
            EV_DETAIL << "Invalid transaction" << std::endl;
            // Maybe send a packet back to signal error?
        }
    }
}

RSA * BlockChainApp::generateKeyPair()
{
    RSA *key = nullptr;
    BIGNUM *bn = BN_new();
    int ret = 0;

    ret = BN_set_word(bn, RSA_F4);
    if(ret != 1){
        // if error
    }

    key = RSA_new();
    ret = RSA_generate_key_ex(key, 2048, bn, NULL);
    if(ret != 1){
        // if error
    }
    return key;
}

RSA * BlockChainApp::generatePublicKey(RSA *key)
{
    BIO *pri = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(pri, key, NULL, NULL, 0, NULL, NULL);
    RSA *publick = RSAPublicKey_dup(key);
    return publick;
}

std::string sha256(const std::string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

std::string signMessage(const std::string &message, RSA *privateKey) {
    unsigned char *sig = new unsigned char[RSA_size(privateKey)];
    unsigned int sigLen;
    RSA_sign(NID_sha256, (unsigned char*)message.c_str(), message.size(), sig, &sigLen, privateKey);
    std::string signature((char*)sig, sigLen);
    delete[] sig;
    return signature;
}

bool verifyMessage(const std::string &message, const std::string &signature, RSA *publicKey) {
    return RSA_verify(NID_sha256, (unsigned char*)message.c_str(), message.size(), (unsigned char*)signature.c_str(), signature.size(), publicKey) == 1;
}

} // namespace inet
