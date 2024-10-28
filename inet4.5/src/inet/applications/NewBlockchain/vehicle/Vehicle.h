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

#ifndef __INET4_5_VEHICLE_H_
#define __INET4_5_VEHICLE_H_

#include <vector>

#include "inet/applications/base/ApplicationBase.h"
#include "inet/common/clock/ClockUserModuleMixin.h"
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
#include <chrono>

namespace inet {

extern template class ClockUserModuleMixin<ApplicationBase>;

/**
 * UDP application. See NED for more info.
 */
class INET_API Vehicle : public ClockUserModuleMixin<ApplicationBase>, public UdpSocket::ICallback
{
  protected:
    enum SelfMsgKinds { START = 1, SEND, STOP };

    // parameters
    std::vector<L3Address> destAddresses;
    std::vector<std::string> destAddressStr;
    std::vector<L3Address> tgsAddresses;
    std::vector<std::string> tgsAddressStr;
    int localPort = -1, destPort = -1, tgsPort = -1, rsuPort;
    clocktime_t startTime;
    clocktime_t stopTime;
    bool dontFragment = false;
    const char *packetName = nullptr;

    // state
    UdpSocket socket;
    ClockEvent *selfMsg = nullptr;

    // statistics
    int numSent = 0;
    int numReceived = 0;

    // blockchain
    int vehId, rsuId;

    // manage session details
    struct Session {
        int vehId;
        std::string ip;
        std::string sessionId;    // to be conerted back to hex when needed
        std::string secretKey;
        std::string timestamp;
        int lifetime;
        int tgsId;
    };

    std::vector<Session> SessionCache;
    std::vector<std::string> TicketCache;

    int rcvdBytes = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> startT;
    std::string serKey;

    simtime_t strt = 0;
  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;

    // chooses random destination address
    virtual L3Address chooseDestAddr();
    virtual void sendPacket();
    virtual void processPacket(Packet *msg);
    virtual void setSocketOptions();

    virtual void processStart();
    virtual void processSend();
    virtual void processStop();

    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;

    //###########  BLOCK CHAIN FUNCTIONS ###########################
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

    // use common IV for encryption and decryption for very run
    // later generate random iv for different encryption for max security
    unsigned char iv[16] = {
            0x2a, 0x7f, 0xd3, 0x4c, 0x9b, 0x12, 0x58, 0x6d,
            0x1e, 0xa0, 0xc5, 0x37, 0xf8, 0x99, 0x23, 0x4b
    };

    // encryption decryption
    std::string aesEncrypt(const std::string &plainText, const unsigned char *key);
    std::string aesDecrypt(const std::string &cipherText, const unsigned char *key);

    // packet handling
    void handleASAT(Packet *packet);
    void handleTGT(Packet *packet);
    void handleTicket(Packet *packet);
    void handleConfirmation(Packet *packet);
    void handleST(Packet *packet);

    L3Address chooseTgsAddr();
    L3Address chooseRSUAddr();

  public:
    Vehicle() {}
    ~Vehicle();
};

} // namespace inet

#endif

