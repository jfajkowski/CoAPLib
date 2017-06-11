#include "CoAPHandler.h"

void CoAPHandler::handleMessage(const CoAPMessage &frame) {
    switch (frame.getCode()){
        case 0 ... 4:
            handleRequest(frame);
            break;
        case 65 ... 69:
            handleSuccess(frame);
            break;
        case 128 ... 143:
            handleClientError(frame);
            break;
        case 160 ...165:
            handleServerError(frame);
            break;
        default:
            //TODO: handle bad request
            break;

    }
}

void CoAPHandler::handleRequest(const CoAPMessage &frame) {
    switch (frame.getCode()){
        case 0:
            //if ack ping back (Do we need this?)
            break;
        case 1:
            handleGet(frame);
            break;
        case 3:
            handlePut(frame);
            break;
        default:
            badRequestResponse(frame);
            break;
    }
}

void CoAPHandler::handleSuccess(const CoAPMessage &frame) {

}

void CoAPHandler::handleClientError(const CoAPMessage &frame) {

}

void CoAPHandler::handleServerError(const CoAPMessage &frame) {

}


void CoAPHandler::handleGet(const CoAPMessage &frame)
{
    int iterator=0;
    int id=0;
    id+= frame.getOptions()[iterator].getNumber();
    if(id==11) //Uri-Path option
    {
        CoAPOption uri_path_option=frame.getOptions()[iterator];
        String path=uri_path_option.toString();
        //TODO: get resource specified by this path
        iterator++;
        id+= frame.getOptions()[iterator].getNumber();
        if(id==17)//Accept option
        {
            //TODO::try to format resource to specified content_format
        }
        ByteArray payload_value(3);
        payload_value.pushBack('p');
        payload_value.pushBack('a');
        payload_value.pushBack('y');

        CoAPMessage response=successResponse(frame, payload_value);
        //TODO: send back/change methods to return response?
    }
    else{
        //throw logic_error("Bad option");
    }
}

void CoAPHandler::handlePut(const CoAPMessage &frame) {
    int iterator=0;
    int id=0;
    id+= frame.getOptions()[iterator].getNumber();
    if(id==11) //Uri-Path option
    {
        CoAPOption uri_path_option=frame.getOptions()[iterator];
        String path=uri_path_option.toString();
        iterator++;
        id+= frame.getOptions()[iterator].getNumber();
        if(id==12){ //Content-Format option
            //TODO: Interpret payload accordingly to Content-Format and update resource
        }
        else{
            //Try to deafault formating for this resource or...
            //throw std::logic_error("No content format option");
        }
        //get updated resource
        iterator++;
        id+= frame.getOptions()[iterator].getNumber();
        if(id==17)//Accept option
        {
            //TODO::try to format resource to specified content_format
        }
        ByteArray mockupPayload=ByteArray((const ByteArray &) "payload");
        CoAPMessage response=successResponse(frame, mockupPayload);
        //TODO: send back/change methods to return response?
    }
    else{
        //throw logic_error("Bad option");
    }
}




CoAPMessage CoAPHandler::successResponse(const CoAPMessage &frame,ByteArray payload ){
    CoAPMessage response;
    response.setToken(frame.getToken());
    if (frame.getT()==0) //if CON
    {
        response.setT(2); //ACK (Piggybacked response)
        response.setMessageId(frame.getMessageId());
    }
    else if (frame.getT()==1) //if NON
    {
        response.setT(1); //NON
        response.setMessageId(frame.getMessageId()+1); //TODO: a method generating new IDs
    }
    if(frame.getCode()==1)
        response.setCode(69); //if get then code: 2.05-content
    else if(frame.getCode()==3)
        response.setCode(68); //if put then code 2.04 -changed
    ByteArray content_format_value(1);
    content_format_value.pushBack(0);
    CoAPOption content_format(12, content_format_value); //For now we assume payload is text/plain
    OptionArray optionArray(1);
    optionArray.pushBack(content_format);
    //response.addOption(optionArray);
    response.setPayload(payload);
    return response;

}

CoAPMessage CoAPHandler::successResponse(const CoAPMessage &frame){
    //TODO: request for comment-do we need this?
}

CoAPMessage CoAPHandler::badRequestResponse(const CoAPMessage &frame) {
    CoAPMessage response;
    response.setToken(frame.getToken());
    if (frame.getT()==0) //if CON
    {
        response.setT(2); //ACK (Piggybacked response)
        response.setMessageId(frame.getMessageId());
    }
    else
    {
        response.setT(1); //NON
        response.setMessageId(frame.getMessageId()+1); //TODO: a method generating new IDs
    }
    response.setCode(128); //Bad request
    return response;
}
