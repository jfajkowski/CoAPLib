#include "Test.hpp"

static CoAPMessage coapMessage;
static RadioMessage radioMessage;

static struct OnCoAPMessageToSend : public CoAPMessageListener {
    void operator()(const CoAPMessage &message) override {
        coapMessage = message;
    }
} onCoAPMessageToSend;


static struct OnRadioMessageToSend : public RadioMessageListener {
    void operator()(const RadioMessage &message) override {
        radioMessage = message;
    }
} onRadioMessageToSend;

beginTest

//    test(HandleMessage) {
//        CoAPMessage message;
//        message.setMessageId(100);
//        message.setToken(222);
//        message.setCode(CODE_GET);
//        message.setT(TYPE_CON);
//        CoAPOption uripath(11, "uri-path");
//        message.addOption(uripath);
//        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
//
//        coAPHandler.handleMessage(message);
//    }
//
//    test(CreateResponse) {
//        CoAPMessage message;
//        message.setMessageId(100);
//        message.setToken(222);
//        message.setCode(CODE_GET);
//        message.setT(TYPE_CON);
//        CoAPOption uripath(11, "uri-path");
//        message.addOption(uripath);
//        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
//
//        coAPHandler.handleMessage(message);
//    }
//
//    test(ExplicitCreateResponse) {
//        CoAPMessage message;
//        message.setMessageId(100);
//        message.setToken(222);
//        message.setCode(CODE_GET);
//        message.setT(TYPE_CON);
//        CoAPOption uripath(11, "uri-path");
//        message.addOption(uripath);
//
//        RadioMessage radioMessageMock;
//        radioMessageMock.message_id = message.getMessageId();
//        radioMessageMock.code = message.getCode();
//        radioMessageMock.resource = 0;
//        radioMessageMock.key = 300;
//        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
//        coAPHandler.handleMessage(message);
//        coAPHandler.handleMessage(radioMessageMock);
//    }
//
//    test(Ping) {
//        unsigned int buffer_size = 4;
//        unsigned char buffer[] = {0x40, 0x00, 0xb7, 0x6c};
//
//        CoAPMessage message;
//        message.deserialize(buffer, buffer_size);
//
//
//        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
//        coAPHandler.handleMessage(message);
//    }

    test(Block2) {
        unsigned int buffer_size = 23;
        unsigned char buffer[] = {0x40, 0x01, 0xfd, 0xf8, 0xbb, 0x2e, 0x77, 0x65, 0x6c,
                                  0x6c, 0x2d, 0x6b, 0x6e, 0x6f, 0x77, 0x6e, 0x04, 0x63,
                                  0x6f, 0x72, 0x65, 0xc1, 0x02};

        CoAPMessage message;
        message.deserialize(buffer, buffer_size);

        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
        coAPHandler.handleMessage(message);
    }

endTest