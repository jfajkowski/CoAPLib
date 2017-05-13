//
// Created by welos on 06.05.17.
//

#ifndef ARDUINODEMO_COAPHANDLER_H
#define ARDUINODEMO_COAPHANDLER_H


#include "Frame.h"
/*
 * Client side Coap Handler, server side may or may not inherit from this class
 */
class CoAPHandler {

private:
    // Code class handling
    void handleRequest(const Frame &frame);
    void handleSuccess(const Frame &frame);
    void handleClientError(const Frame &frame);
    void handleServerError(const Frame &frame);

    //Requests handling
    void handleGet(const Frame &frame);
    void handlePut(const Frame &frame);



    //Responses
    Frame successResponse(const Frame &frame,ByteArray payload); //generally response for GET
    Frame successResponse(const Frame &frame); //may be response for PUT, but i think we send back value anyway
    Frame badRequestResponse(const Frame &frame);
public:
    void handleMessage(const Frame &frame);


};


#endif //ARDUINODEMO_COAPHANDLER_H
