#ifndef COAPLIB_SERVERCOAPHANDLER_H
#define COAPLIB_SERVERCOAPHANDLER_H


#include "CoAPMessage.h"
#include "CoAPMessageListener.h"
#include "CoAPResources.h"
#include "../Environment.h"
#include "../RadioLib.h"

/**
 * Main class responsible for handling both CoAP and radio messages
 */
class CoAPHandler {
private:
    struct PendingMessage {
        CoAPMessage coapMessage;
        unsigned long timestamp;
    };

    unsigned short timeout_ = 5000;

    unsigned short ping_messages_sent = 0;
    unsigned short mean_rtt = 0;
    short last_jitter = 0;
    unsigned short timed_out = 0;

    CoAPResources resources_;
    CoAPMessageListener* coapMessageListener_;
    RadioMessageListener* radioMessageListener_;

    Array<PendingMessage> pending_messages_;

    void handlePing(const CoAPMessage &message);
    void handleRequest(const CoAPMessage &message);
    void handleBadRequest(const CoAPMessage &message, unsigned short error_code);

    void updateMetrics(unsigned short rtt);
    void updateRoundTripTimeMetric(unsigned short rtt);
    void updateJitterMetric(unsigned short rtt);
    void updateTimeoutMetric();

    void addPendingMessage(const CoAPMessage &message);
    PendingMessage finalizePendingMessage(const unsigned short message_id);

    void send(const CoAPMessage &message);
    void send(const RadioMessage &message);

    void createResponse(const CoAPMessage &message, CoAPMessage &response);
    void createResponse(const CoAPMessage &message, RadioMessage &response);

    ByteArray toByteArray(const String &value);
    ByteArray toByteArray(unsigned short value);
    unsigned short toUnsignedShort(const String &value);
    String toString(const ByteArray &value);
    CoAPOption toContentFormat(unsigned short value);

    void prepareSpeakerResource();
    void prepareLampResource();
    void prepareRttResource();
    void prepareJitterResource();
    void prepareTimedOutResource();
public:
    CoAPHandler(CoAPMessageListener &coapMessageListener, RadioMessageListener &radioMessageListener);

    void handleMessage(CoAPMessage &message);
    void handleMessage(RadioMessage &radioMessage);

    void registerResource(const Array<String> &uri_path, unsigned short *value);
    void sendPing();
    void deleteTimedOut();

    unsigned short getTimeout() const;
    void print() {
        PRINT(resources_.toLinkFormat());
    }
};


#endif //COAPLIB_SERVERCOAPHANDLER_H
