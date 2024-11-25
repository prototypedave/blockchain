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

#ifndef __INET4_5_TAS_H_
#define __INET4_5_TAS_H_

#include <vector>

#include <sstream>
#include <iomanip>
#include <fstream>
#include <chrono>

#include "inet/applications/kbc/kbc.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/common/clock/ClockUserModuleMixin.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"

namespace inet {

extern template class ClockUserModuleMixin<ApplicationBase>;

/**
 * UDP application. See NED for more info.
 */
class INET_API TAS : public ClockUserModuleMixin<ApplicationBase>, public UdpSocket::ICallback, public KBC
{
  protected:
    enum SelfMsgKinds { START = 1, SEND, STOP };

    // parameters
    std::vector<L3Address> destAddresses;
    std::vector<std::string> destAddressStr;
    int localPort = -1, destPort = -1;
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

    std::string tempAU;
    int rcvdBytes = 0;

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

    void handleRegistrationOfVehiclePacket(Packet *pkt);
    void handleVehicleToTGSPacket(Packet *pkt);

    void saveBlock(std::string AU, std::string vehicleId);
    void generateTicket(std::string AT, std::string ST, std::string key, L3Address remoteAddress);
    void generateRSU(int num);

  private:
    unsigned char TGSSecretKey[16];
    std::vector<VehicleData> vehicles;
    std::vector<RSUData> rsus;

    void RegisterRSU();
  public:
    std::string TGSname;
    void setRSU(std::vector<RSUData> rsu);
    TAS() {}
    ~TAS();
};

} // namespace inet

#endif

