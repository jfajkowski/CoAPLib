#include "Test.hpp"

static struct OnCoAPMessageToSend : public CoAPMessageListener {
    void operator()(const CoAPMessage &message) override {
        message.print();
    }
} onCoAPMessageToSend;


static struct OnRadioMessageToSend : public RadioMessageListener {
    void operator()(const RadioMessage &message) override {
        message.print();
    }
} onRadioMessageToSend;

beginTest

    test(HandleMessageTest) {
        CoAPMessage message;
        message.setMessageId(100);
        message.setToken(222);
        message.setCode(CODE_GET);
        message.setT(TYPE_CON);
        CoAPOption uripath(11, "uri-path");
        message.addOption(uripath);
        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);

        coAPHandler.handleMessage(message);
    }

    test(CreateResponseTest) {
        CoAPMessage message;
        message.setMessageId(100);
        message.setToken(222);
        message.setCode(CODE_GET);
        message.setT(TYPE_CON);
        CoAPOption uripath(11, "uri-path");
        message.addOption(uripath);
        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);

        coAPHandler.handleMessage(message);
    }

    test(ExplicitCreateResponseTest) {
        CoAPMessage message;
        message.setMessageId(100);
        message.setToken(222);
        message.setCode(CODE_GET);
        message.setT(TYPE_CON);
        CoAPOption uripath(11, "uri-path");
        message.addOption(uripath);

        RadioMessage radioMessageMock;
        radioMessageMock.message_id = (unsigned short)message.getMessageId();
        radioMessageMock.code = (unsigned short)message.getCode();
        radioMessageMock.resource = 0;
        radioMessageMock.value = 300;
        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
        coAPHandler.handleMessage(message);
        coAPHandler.createResponse(radioMessageMock);
    }

endTest