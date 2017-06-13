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
    coAPHandler.registerResource(uri_path, SPEAKER);
}

void prepareLampResource(CoAPHandler &coAPHandler) {
    Array<String> uri_path;
    uri_path.pushBack("lamp");
    coAPHandler.registerResource(uri_path, LAMP);
}

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
//        radioMessageMock.value = 300;
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
//
//    test(Block2) {
//        unsigned int buffer_size = 23;
//        unsigned char buffer[] = {0x40, 0x01, 0xfd, 0xf8, 0xbb, 0x2e, 0x77, 0x65, 0x6c,
//                                  0x6c, 0x2d, 0x6b, 0x6e, 0x6f, 0x77, 0x6e, 0x04, 0x63,
//                                  0x6f, 0x72, 0x65, 0xc1, 0x02};
//
//        CoAPMessage message;
//        message.deserialize(buffer, buffer_size);
//
//        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
//        coAPHandler.handleMessage(message);
//    }

//    test(RegisteredResourceGet) {
//        CoAPMessage speaker_message;
//        speaker_message.setCode(CODE_GET);
//        speaker_message.addOption(CoAPOption(11, "speaker"));
//
//        CoAPMessage lamp_message;
//        lamp_message.setCode(CODE_GET);
//        lamp_message.addOption(CoAPOption(11, "lamp"));
//
//        CoAPHandler coap_handler(onCoAPMessageToSend, onRadioMessageToSend);
//        prepareSpeakerResource(coap_handler);
//        prepareLampResource(coap_handler);
//
//        coap_handler.handleMessage(speaker_message);
//    }
//
//    test(Timeout) {
//        CoAPHandler coap_handler(onCoAPMessageToSend, onRadioMessageToSend);
//        coap_handler.sendPing();
//
//        coap_handler.deleteTimedOut();
//        delay(6000);
//        coap_handler.deleteTimedOut();
//    }

        test(OptionContentFormat) {
        CoAPMessage message;
        message.setMessageId(100);
        message.setToken(222);
        message.setCode(CODE_PUT);
        message.setT(TYPE_CON);
        CoAPOption uripath(11, RESOURCE_REMOTE);
        message.addOption(uripath);
//        CoAPOption wellknown(11, RESOURCE_WELL_KNOWN);
//        message.addOption(wellknown);
//        CoAPOption core(11, RESOURCE_CORE);
//        message.addOption(core);

        ByteArray c_f(2);
        c_f.pushBack((const unsigned char &) (0 & 0xff));
        c_f.pushBack((const unsigned char &) ((0 >> 8) & 0xff));

        CoAPOption content_format(OPTION_CONTENT_FORMAT, c_f);
        message.addOption(content_format);

        ByteArray payload;
        payload.pushBack('2');
        payload.pushBack('4');
        message.setPayload(payload);

        CoAPHandler coAPHandler(onCoAPMessageToSend, onRadioMessageToSend);
        coAPHandler.handleMessage(message);
    }

endTest