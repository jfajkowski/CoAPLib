#include "Test.hpp"

beginTest

    test(HandleMessageTest) {
        CoAPMessage message;
        message.setMessageId(100);
        message.setToken(222);
        message.setCode(CODE_GET);
        message.setT(TYPE_CON);
        CoAPOption uripath(11, "uri-path");
        message.addOption(uripath);
        ServerCoAPHandler coAPHandler;

        coAPHandler.handleMessage(message);
        RadioMessage* radioMessage = coAPHandler.popRadioMessageToSend();
        assertEqual(radioMessage->message_id, message.getMessageId());
        assertEqual(radioMessage->code, 1);
        assertEqual(radioMessage->resource, 0);
        delete radioMessage;
    }

    test(CreateResponseTest) {
        CoAPMessage message;
        message.setMessageId(100);
        message.setToken(222);
        message.setCode(CODE_GET);
        message.setT(TYPE_CON);
        CoAPOption uripath(11, "uri-path");
        message.addOption(uripath);
        ServerCoAPHandler coAPHandler;

        coAPHandler.handleMessage(message);
        RadioMessage* radioMessage = coAPHandler.popRadioMessageToSend();
        assertEqual(radioMessage->message_id, message.getMessageId());
        assertEqual(radioMessage->code, 1);
        assertEqual(radioMessage->resource, 0);

        radioMessage->value = 'p';
        coAPHandler.createResponse(*radioMessage);
        CoAPMessage* response = coAPHandler.popCoAPMessageToSend();
        assertEqual(response->getMessageId(), message.getMessageId());
        assertEqual(response->getPayload()[0], 'p');

        delete radioMessage;
        delete response;
    }

endTest