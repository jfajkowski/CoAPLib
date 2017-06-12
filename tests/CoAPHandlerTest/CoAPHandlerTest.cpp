#include "Test.hpp"

static struct OnCoAPMessageToSend : public CoAPMessageListener {
    void operator()(const CoAPMessage &message) override {
        PRINTLN("CoAP message:");
        message.print();
    }
} onCoAPMessageToSend;


static struct OnRadioMessageToSend : public RadioMessageListener {
    void operator()(const RadioMessage &message) override {
        PRINTLN("Radio message:");
        PRINT("Message ID: ");
        PRINTLN(message.message_id);
        PRINT("Code: ");
        PRINTLN(message.code);
        PRINT("Resource: ");
        PRINTLN(message.resource);
        PRINT("Value: ");
        PRINTLN(message.value);
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

endTest