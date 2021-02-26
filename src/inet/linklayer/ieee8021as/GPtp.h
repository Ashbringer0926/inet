//
// @authors: Enkhtuvshin Janchivnyambuu
//           Henning Puttnies
//           Peter Danielis
//           University of Rostock, Germany
// 

#ifndef __IEEE8021AS_GPTP_H_
#define __IEEE8021AS_GPTP_H_

#include "inet/clock/contract/ClockTime.h"
#include "inet/clock/common/ClockTime.h"
#include "inet/clock/model/SettableClock.h"
#include "inet/common/INETDefs.h"
#include "inet/common/clock/ClockUserModuleBase.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/linklayer/ieee8021as/GPtpPacket_m.h"

namespace inet {

class GPtp : public ClockUserModuleBase
{
    //parameters:
    IInterfaceTable *interfaceTable = nullptr;

    GPtpNodeType gPtpNodeType;
    int slavePortId = -1; // interface ID of slave port
    std::set<int> masterPortIds; // interface IDs of master ports
    clocktime_t correctionField;

    double gmRateRatio = 1.0;
    double receivedRateRatio = 1.0;

    clocktime_t originTimestamp; // last outgoing timestamp

    clocktime_t receivedTimeSync;

    /* time to receive Sync message before synchronize local time with master */
    clocktime_t timeBeforeSync;

    // This is used to calculate residence time within time-aware system
    // Its value has the time receiving Sync message from master port of other system
    clocktime_t receivedTimeAtHandleMessage;

    // Adjusted time when Sync received
    // For constant drift, setTime = sentTime + delay
    clocktime_t setTime;

    clocktime_t schedulePdelay;
    clocktime_t syncInterval;
    clocktime_t pdelayInterval;

    /* Slave port - Variables is used for Peer Delay Measurement */
    uint16_t pdelaySequenceId = 0;
    clocktime_t peerDelay;
    clocktime_t peerRequestReceiptTimestamp;  // pdelayReqIngressTimestamp from peer (received in GPtpPdelayResp)
    clocktime_t peerResponseOriginTimestamp; // pdelayRespEgressTimestamp from peer (received in GPtpPdelayRespFollowUp)
    clocktime_t pdelayRespEventIngressTimestamp;  // receiving time of last GPtpPdelayResp
    clocktime_t pdelayReqEventEgressTimestamp;   // sending time of last GPtpPdelayReq
    clocktime_t pDelayReqProcessingTime;  // processing time between arrived PDelayReq and send of PDelayResp
    bool rcvdPdelayResp = false;

    clocktime_t sentTimeSyncSync;

    /* Slave port - Variables is used for Rate Ratio. All times are drifted based on constant drift */
    // clocktime_t sentTimeSync;
    clocktime_t newLocalTimeAtTimeSync;
    clocktime_t oldLocalTimeAtTimeSync;
    clocktime_t peerSentTimeSync;  // sending time of last received GPtpSync
    clocktime_t peerSentTimeSyncEgressTimestamp;  // sending time of last received GPtpSync
    clocktime_t syncIngressTimestamp;  // receiving time of last incoming GPtpSync

    // self timers:
    ClockEvent* selfMsgSync = nullptr;
    ClockEvent* selfMsgDelayReq = nullptr;
    ClockEvent* requestMsg = nullptr;

    // Statistics information: // TODO remove, and replace with emit() calls
    static simsignal_t localTimeSignal;
    static simsignal_t timeDifferenceSignal;
    static simsignal_t rateRatioSignal;
    static simsignal_t peerDelaySignal;
  public:
    static const MacAddress GPTP_MULTICAST_ADDRESS;

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;

    virtual void handleSelfMessage(cMessage *msg);

  public:
    virtual ~GPtp();
    void sendPacketToNIC(Packet *packet, int portId);

    void sendSync();
    void sendFollowUp(int portId, clocktime_t preciseOriginTimestamp);
    void sendPdelayReq();
    void sendPdelayResp(GPtpReqAnswerEvent *req);
    void sendPdelayRespFollowUp(int portId, const GPtpPdelayResp* resp);

    void processSync(Packet *packet, const GPtpSync* gptp);
    void processFollowUp(Packet *packet, const GPtpFollowUp* gptp);
    void processPdelayReq(Packet *packet, const GPtpPdelayReq* gptp);
    void processPdelayResp(Packet *packet, const GPtpPdelayResp* gptp);
    void processPdelayRespFollowUp(Packet *packet, const GPtpPdelayRespFollowUp* gptp);

    clocktime_t getCalculatedDrift(IClock *clock, clocktime_t value) { return CLOCKTIME_ZERO; }
    void synchronize();

    virtual void receiveSignal(cComponent *source, simsignal_t signal, cObject *obj, cObject *details) override;
};

}

#endif
