#ifndef COAPHANDLER_H
#define COAPHANDLER_H

#include "Environment.h"
#include "CoAPMessage.h"
#include "Array.hpp"
#include "CoAPOption.h"
/*
 * Client side Coap Handler, server side may or may not inherit from this class
 */
class CoAPHandler {

private:
    // Code class handling
    void handleRequest(const CoAPMessage &frame);
    void handleSuccess(const CoAPMessage &frame);
    void handleClientError(const CoAPMessage &frame);
    void handleServerError(const CoAPMessage &frame);

    //Requests handling
    void handleGet(const CoAPMessage &frame);
    void handlePut(const CoAPMessage &frame);



    //Responses
    CoAPMessage successResponse(const CoAPMessage &frame,ByteArray payload); //generally response for GET
    CoAPMessage badRequestResponse(const CoAPMessage &frame);
    CoAPMessage successResponse(const CoAPMessage &frame); //may be response for PUT, but i think we send back value anyway

public:
    void handleMessage(const CoAPMessage &frame);
    CoAPMessage successWrapper(const CoAPMessage &frame,ByteArray payload){return successResponse(frame, payload);}

};


#endif //COAPHANDLER_H
