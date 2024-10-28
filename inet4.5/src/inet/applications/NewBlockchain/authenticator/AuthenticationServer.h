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

#ifndef __INET4_5_AUTHENTICATIONSERVER_H_
#define __INET4_5_AUTHENTICATIONSERVER_H_

#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <iomanip>


namespace inet {

/**
 * UDP application. See NED for more info.
 */
class INET_API AS : public ApplicationBase, public UdpSocket::ICallback
{
  protected:
    UdpSocket socket;
    int numEchoed; // just for WATCH
    int numReceived;

    // BlockChain
    int tgsId;
    int rcvdBytes = 0;

    struct VehicleData {
        std::string vehicleID;
        std::string userID;
        std::string password;
        std::string source;
        std::string destination;
        std::string serviceType;
        std::string role;
        std::string ip;
        int id;
        unsigned char secretKey[16];
    };

    std::vector<VehicleData> vehicleRecords;

    struct RSUData {
        std::string location;
        std::string macAddress;
        std::string ip;
        std::string rsuID;
        int id;
        unsigned char secretKey[16];
    };

    std::vector<RSUData> rsuRecords;

    // use common IV for encryption and decryption for very run
    unsigned char iv[16] = {
            0x2a, 0x7f, 0xd3, 0x4c, 0x9b, 0x12, 0x58, 0x6d,
            0x1e, 0xa0, 0xc5, 0x37, 0xf8, 0x99, 0x23, 0x4b
    };

    unsigned char TGSkey[16];   // TGS secret key

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

    // Block Chain
    bool checkRecord(int vehicleId);
    void OfflineRegistration(const char * vFiles, const char * rFiles);

    std::string aesEncrypt(const std::string &plainText, const unsigned char *key);
    std::string aesDecrypt(const std::string &cipherText, const unsigned char *key);

    void handleVehicleRA(UdpSocket *socket, Packet *packet);

    // Convert a binary buffer to a hex string
    std::string bin2hex(const unsigned char* bin, size_t len) {
        std::ostringstream oss;
        for (size_t i = 0; i < len; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0') << (int)bin[i];
        }
        return oss.str();
    }

    // Convert a hex string to a binary buffer
    std::vector<unsigned char> hex2bin(const std::string &hex) {
        std::vector<unsigned char> bin(hex.size() / 2);
        for (size_t i = 0; i < hex.size(); i += 2) {
            std::istringstream iss(hex.substr(i, 2));
            int byte;
            iss >> std::hex >> byte;
            bin[i / 2] = static_cast<unsigned char>(byte);
        }
        return bin;
    }
};

} // namespace inet

#endif
