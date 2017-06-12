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

    test(PingTest) {
        unsigned int buffer_size = 4;
        unsigned char buffer[] = {0x40, 0x00, 0xb7, 0x6c};

        CoAPMessage message;
        message.deserialize(buffer, buffer_size);


        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
        coAPHandler.handleMessage(message);
    }

endTest