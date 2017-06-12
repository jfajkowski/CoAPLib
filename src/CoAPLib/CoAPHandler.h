#ifndef COAPLIB_SERVERCOAPHANDLER_H
#define COAPLIB_SERVERCOAPHANDLER_H


#include "CoAPMessage.h"
#include "../RadioLib/RadioMessage.hpp"
#include "CoAPMessageListener.h"
#include "../RadioLib/RadioMessageListener.hpp"

class CoAPHandler {
private:
    CoAPMessageListener* coapMessageListener_;
    RadioMessageListener* radioMessageListener_;

    CoAPMessage* coAP_message_to_send_;

    Array<CoAPMessage> pending_messages_;

    void handleGet(CoAPMessage &message);
    void handlePut(CoAPMessage &message);
    void handleBadRequest(CoAPMessage &message);

    RadioMessage prepareRadioMessage(unsigned int message_id, unsigned int code, String uri) const;

    void sendCoAPMessage(const CoAPMessage &message);
    void sendRadioMessage(const RadioMessage &message);
public:
    CoAPHandler(CoAPMessageListener &coapMessageListener, RadioMessageListener &radioMessageListener) :
                coapMessageListener_(&coapMessageListener),
                radioMessageListener_(&radioMessageListener),
                coAP_message_to_send_(nullptr) {}

    void handleMessage(CoAPMessage &message);
    void createResponse(RadioMessage &radioMessage);
};


#endif //COAPLIB_SERVERCOAPHANDLER_H
