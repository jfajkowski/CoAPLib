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

void prepareSpeakerResource(CoAPHandler &coAPHandler) {
    Array<String> uri_path;
    uri_path.pushBack("speaker");
    coAPHandler.registerResource(uri_path);
}

void prepareLampResource(CoAPHandler &coAPHandler) {
    Array<String> uri_path;
    uri_path.pushBack("lamp");
    coAPHandler.registerResource(uri_path);
}

void sleep(unsigned long seconds, unsigned long nanos) {
    struct timespec tim, tim2;
    tim.tv_sec  = seconds;
    tim.tv_nsec = nanos;
    nanosleep(&tim , &tim2);
}

beginTest

    test(HandleMessage) {
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

    test(CreateResponse) {
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

    test(ExplicitCreateResponse) {
        CoAPMessage message;
        message.setMessageId(100);
        message.setToken(222);
        message.setCode(CODE_GET);
        message.setT(TYPE_CON);
        CoAPOption uripath(11, "uri-path");
        message.addOption(uripath);

        RadioMessage radioMessageMock;
        radioMessageMock.message_id = message.getMessageId();
        radioMessageMock.code = message.getCode();
        radioMessageMock.resource = 0;
        radioMessageMock.key = 300;
        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
        coAPHandler.handleMessage(message);
        coAPHandler.handleMessage(radioMessageMock);
    }

    test(Ping) {
        unsigned int buffer_size = 4;
        unsigned char buffer[] = {0x40, 0x00, 0xb7, 0x6c};

        CoAPMessage message;
        message.deserialize(buffer, buffer_size);


        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
        coAPHandler.handleMessage(message);
    }

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

    test(RegisteredResourceGet) {
        CoAPMessage speaker_message;
        speaker_message.setCode(CODE_GET);
        speaker_message.addOption(CoAPOption(11, "speaker"));

        CoAPMessage lamp_message;
        lamp_message.setCode(CODE_GET);
        lamp_message.addOption(CoAPOption(11, "lamp"));

        CoAPHandler coap_handler(onCoAPMessageToSend, onRadioMessageToSend);
        prepareSpeakerResource(coap_handler);
        prepareLampResource(coap_handler);

        coap_handler.handleMessage(speaker_message);
    }

    test(Timeout) {
        CoAPHandler coap_handler(onCoAPMessageToSend, onRadioMessageToSend);
        coap_handler.sendPing();

        coap_handler.deleteTimedOut();
        sleep(6, 0);
        coap_handler.deleteTimedOut();
    }

endTest