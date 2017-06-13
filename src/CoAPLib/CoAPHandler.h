#ifndef COAPLIB_SERVERCOAPHANDLER_H
#define COAPLIB_SERVERCOAPHANDLER_H


#include "CoAPMessage.h"
#include "CoAPMessageListener.h"
#include "CoAPResources.h"
#include "../RadioLib.h"

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

    ByteArray toByteArray(const String &value);
    ByteArray toByteArray(unsigned short value);
    unsigned short toUnsignedShort(const String &value);
    String toString(const ByteArray &value);
    CoAPOption toContentFormat(unsigned short value);
public:
    CoAPHandler(CoAPMessageListener &coapMessageListener, RadioMessageListener &radioMessageListener) :
                coapMessageListener_(&coapMessageListener),
                radioMessageListener_(&radioMessageListener),
                resources_() {}

    void handleMessage(CoAPMessage &message);
    void handleMessage(RadioMessage &radioMessage);

    void registerResource(const Array<String> &uri_path, unsigned short value);
    void sendPing();
    void deleteTimedOut();

    unsigned short getTimeout() const;

    void createResponse(const CoAPMessage &message, CoAPMessage &response);
};


#endif //COAPLIB_SERVERCOAPHANDLER_H
