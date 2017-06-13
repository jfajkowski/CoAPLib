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
        unsigned long timeArrived;
    };

    CoAPResources resources_;
    CoAPMessageListener* coapMessageListener_;
    RadioMessageListener* radioMessageListener_;

    Array<PendingMessage> pending_messages_;

    void handlePing(const CoAPMessage &message);
    void handleGet(const CoAPMessage &message);
    void handlePut(const CoAPMessage &message);
    void handleBadRequest(const CoAPMessage &message);

    RadioMessage prepareRadioMessage(unsigned short code, unsigned short message_id, String uri) const;

    void addPendingMessage(const CoAPMessage &message);
    CoAPMessage finalizePendingMessage(const unsigned int message_id);
    void appendPendingMessage(const CoAPMessage &message);

    void sendCoAPMessage(const CoAPMessage &message);
    void sendRadioMessage(const RadioMessage &message);
public:
    CoAPHandler(CoAPMessageListener &coapMessageListener, RadioMessageListener &radioMessageListener) :
                coapMessageListener_(&coapMessageListener),
                radioMessageListener_(&radioMessageListener),
                resources_() {}

    void handleMessage(CoAPMessage &message);
    void handleMessage(RadioMessage &radioMessage);
};


#endif //COAPLIB_SERVERCOAPHANDLER_H
