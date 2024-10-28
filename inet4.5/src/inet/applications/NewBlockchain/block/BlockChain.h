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

#ifndef __INET4_5_BLOCKCHAIN_H_
#define __INET4_5_BLOCKCHAIN_H_

#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"

#include <iostream>
#include <vector>
#include <string>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <iomanip>
#include <cstring>

namespace inet {

/**
 * UDP application. See NED for more info.
 */

// Helper functions for block chain
std::string sha256(const std::string &str);
std::string signMessage(const std::string &message, RSA *privateKey);
bool verifyMessage(const std::string &message, const std::string &signature, RSA *publicKey);

class INET_API BlockChainApp : public ApplicationBase, public UdpSocket::ICallback
{
  protected:
    UdpSocket socket;
    int numEchoed; // just for WATCH

    struct Transaction {
        std::string fromAddress;
        std::string toAddress;
        std::string message;
        std::string signature;

        Transaction(const std::string &from, const std::string &to, const std::string &msg, RSA *privateKey) {
            fromAddress = from;
            toAddress = to;
            message = msg;
            signature = signMessage(from + to + msg, privateKey);
        }

        bool isValid(RSA *publicKey) const {
            return verifyMessage(fromAddress + toAddress + message, signature, publicKey);
        }
    };

    struct Block {
        int index;
        std::string previousHash;
        std::string timestamp;
        std::vector<Transaction> transactions;
        std::string hash;
        int nonce;

        Block(int idx, const std::string &prevHash, const std::vector<Transaction> &txs) {
            index = idx;
            previousHash = prevHash;
            transactions = txs;
            timestamp = std::to_string(std::time(0));
            nonce = 0;
            hash = calculateHash();
        }

        std::string calculateHash() const {
            std::string txHashes;
            for (const auto &tx : transactions)
                txHashes += tx.fromAddress + tx.toAddress + tx.message + tx.signature;
            return sha256(std::to_string(index) + previousHash + timestamp + txHashes + std::to_string(nonce));
        }

        void mineBlock(int difficulty) {
            std::string target(difficulty, '0');
            while (hash.substr(0, difficulty) != target) {
                nonce++;
                hash = calculateHash();
            }
            std::cout << "Block mined: " << hash << std::endl;
        }
    };

    struct Blockchain {
        int difficulty;
        std::vector<Block> chain;

        Blockchain(int diff) : difficulty(diff) {
            chain.push_back(createGenesisBlock());
        }

        Block createGenesisBlock() {
            std::vector<Transaction> genesisTransactions;
            return Block(0, "0", genesisTransactions);
        }

        Block getLatestBlock() const {
            return chain.back();
        }

        void addBlock(Block &&newBlock) {
            newBlock.previousHash = getLatestBlock().hash;
            newBlock.mineBlock(difficulty);
            chain.push_back(newBlock);
        }

        bool containsMessage(const std::string &message) const {
            for (const auto &block : chain) {
                for (const auto &transaction : block.transactions) {
                    if (transaction.message == message) {
                        return true;
                    }
                }
            }
            return false;
        }
    };

    Blockchain blockchain{4}; // Difficulty set to 4 for mining

    RSA * keypair;
    RSA * publicKey;

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;

    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;

    // Block chain
    RSA *generateKeyPair();
    RSA *generatePublicKey(RSA *key);
    void handleAUfromServer(UdpSocket *socket, Packet *packet);
};

} // namespace inet

#endif
