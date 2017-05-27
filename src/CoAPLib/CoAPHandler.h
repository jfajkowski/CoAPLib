#ifndef COAPHANDLER_H
#define COAPHANDLER_H

#include "Frame.h"
#include "Array.hpp"
#include "Option.h"
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
    Frame badRequestResponse(const Frame &frame);
    Frame successResponse(const Frame &frame); //may be response for PUT, but i think we send back value anyway

public:
    void handleMessage(const Frame &frame);
    Frame successWrapper(const Frame &frame,ByteArray payload){return successResponse(frame, payload);}

};


#endif //COAPHANDLER_H
