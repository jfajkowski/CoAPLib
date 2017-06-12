#include "ServerCoAPHandler.h"

void ServerCoAPHandler::handleMessage(CoAPMessage &message) {
    switch (message.getCode()){
        case CODE_EMPTY ... CODE_DELETE:
            handleRequest(message);
            break;
        case 65 ... 69:
            //SUCCESS
            break;
        case 128 ... 143:
            //CLIENT ERROR
            break;
        case 160 ...165:
            //SERVER ERROR
            break;
        default:
            //BAD REQUEST
            break;
    }
}

void ServerCoAPHandler::handleRequest(CoAPMessage &message) {
    switch (message.getCode()) {
        case CODE_EMPTY:
            //PING
            break;
        case CODE_GET:
            handleGet(message);
            break;
        case CODE_PUT:
            handlePut(message);
            break;
        default:
            //BAD REQUEST
            break;
    }
}

void ServerCoAPHandler::handleGet(CoAPMessage &message) {

    OptionArray options = message.getOptions();
    CoAPOption* iterator = options.begin();
    int option_id = 0;

    for(; iterator != options.end(); iterator++) {
        option_id += iterator->getNumber();

        switch(option_id) {
            case 11: //Uri-Path option
                radio_message_to_send_ = createRadioMessage(message.getMessageId(),
                                                            message.getCode(),
                                                            iterator->toString());
                pending_messages_.pushBack(message); //TODO: pushing back a copy, what about original?
                break;
            default:
                //BAD REQUEST?
                //TODO: add more options
                break;
        }
    }
    //TODO: what if there are no options?
}

RadioMessage* ServerCoAPHandler::createRadioMessage(unsigned int message_id, int code, String uri) const {
    RadioMessage* message = new RadioMessage();
    message->message_id = message_id;
    //TODO: translate uri to resource code and change message.resource = 0

    switch(code) {
        case CODE_GET:
            message->code = 1; //GET
            message->resource = 0; //lamp
            break;
        case CODE_PUT:
            message->code = 0; //PUT
            message->resource = 0; //lamp
            //message.value = ?
            break;
        default:
            //error?
            break;
    }
    return message;
}

void ServerCoAPHandler::createResponse(RadioMessage &radioMessage) {
    CoAPMessage message;
    for(int i = 0; i < pending_messages_.size(); ++i) {
        if(pending_messages_[i].getMessageId() == radioMessage.message_id)
            message = pending_messages_[i]; //TODO: delete from Array
    }

    CoAPMessage* response = new CoAPMessage();
    response->setToken(message.getToken());

    if (message.getT() == TYPE_CON) {
        response->setT(TYPE_ACK);
        response->setMessageId(message.getMessageId());
    } else if (message.getT() == TYPE_NON) {
        response->setT(TYPE_NON);
        response->setMessageId(message.getMessageId()+1); //TODO: a method generating new IDs
    }

    if(message.getCode() == CODE_GET)
        response->setCode(69); //if get then code: 2.05-content
    else if(message.getCode() == CODE_PUT)
        response->setCode(68); //if put then code 2.04 -changed

    ByteArray content_format_value(1);
    content_format_value.pushBack(0);
    CoAPOption content_format(12, content_format_value); //For now we assume payload is text/plain
    response->addOption(content_format);

    ByteArray payload;
    payload.pushBack(radioMessage.value); //TODO: fix for strings
    response->setPayload(payload);
    coAP_message_to_send_ = response;
}

void ServerCoAPHandler::handlePut(CoAPMessage &message) {
    //TODO: implement
    /*
    int iterator=0;
    int id=0;
    id+= message.getOptions()[iterator].getNumber();
    if(id==11) //Uri-Path option
    {
        CoAPOption uri_path_option=message.getOptions()[iterator];
        String path=uri_path_option.toString();
        iterator++;
        id+= message.getOptions()[iterator].getNumber();
        if(id == 12){ //Content-Format option
            //TODO: Interpret payload accordingly to Content-Format and update resource
        }
        else{
            //Try to deafault formating for this resource or...
            //throw std::logic_error("No content format option");
        }
        //get updated resource
        iterator++;
        id+= message.getOptions()[iterator].getNumber();
        if(id == 17)//Accept option
        {
            //TODO::try to format resource to specified content_format
        }
        ByteArray mockupPayload=ByteArray((const ByteArray &) "payload");
        CoAPMessage response = sendResponse(message, mockupPayload);
        //TODO: send back/change methods to return response?
    }
    else{
        //throw logic_error("Bad option");
    }
    */
}

CoAPMessage *ServerCoAPHandler::popCoAPMessageToSend() {
    if(coAP_message_to_send_ == nullptr)
        return nullptr;
    else {
        CoAPMessage *temp = coAP_message_to_send_;
        coAP_message_to_send_ = nullptr;
        return temp;
    }
}

RadioMessage *ServerCoAPHandler::popRadioMessageToSend() {
    if(radio_message_to_send_ == nullptr)
        return nullptr;
    else {
        RadioMessage *temp = radio_message_to_send_;
        radio_message_to_send_ = nullptr;
        return temp;
    }
}

void ServerCoAPHandler::handleBadRequest(CoAPMessage &message) {
    CoAPMessage* response = new CoAPMessage();
    response->setToken(message.getToken());
    if (message.getT() == TYPE_CON) {
        response->setT(TYPE_ACK);
        response->setMessageId(message.getMessageId());
    } else {
        response->setT(TYPE_NON);
        response->setMessageId(message.getMessageId()+1); //TODO: a method generating new IDs
    }
    response->setCode(CODE_BAD_REQUEST);
    coAP_message_to_send_ = response;
}
