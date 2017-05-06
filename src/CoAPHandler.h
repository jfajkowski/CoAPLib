//
// Created by welos on 06.05.17.
//

#ifndef ARDUINODEMO_COAPHANDLER_H
#define ARDUINODEMO_COAPHANDLER_H


#include "Frame.h"

class CoAPHandler {

private:
    // Code class handling
    void handleRequest(Frame &frame);
    void handleSuccess(Frame &frame);
    void handleClientError(Frame &frame);
    void handleServerError(Frame &frame);

    //Requests handling
    void handleGet(Frame &frame);
public:
    void handleMessage(Frame &frame);


};


#endif //ARDUINODEMO_COAPHANDLER_H
