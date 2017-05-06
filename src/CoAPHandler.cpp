//
// Created by welos on 06.05.17.
//

#include <iostream>
#include <string>
#include "CoAPHandler.h"

void CoAPHandler::handleMessage(Frame &frame) {
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

void CoAPHandler::handleRequest(Frame &frame) {
    switch (frame.getCode()){
        case 0:
            //if ack ping back
            break;
        case 1:
            handleGet(frame);
            break;
        case 3:
            //handle put
            break;
        default:
            //bad request or not implemented
            break;
    }
}

void CoAPHandler::handleSuccess(Frame &frame) {

}

void CoAPHandler::handleClientError(Frame &frame) {

}

void CoAPHandler::handleServerError(Frame &frame) {

}


void CoAPHandler::handleGet(Frame &frame) {
    try{
        int iterator=0;
        int id=0;
        while(frame.getOptions()[iterator].getDelta()!=15)
        {
            id+=frame.getOptions()[iterator].getDelta();
            if(id==11)
            {
                int length=frame.getOptions()[iterator].getLength();
                unsigned char val[frame.getOptions()[iterator].getValue().size()];
                for(int i=0;i < frame.getOptions()[iterator].getValue().size();++i){
                    val[i]=frame.getOptions()[iterator].getValue()[i];
                }
                std::string path(reinterpret_cast<char const*>(val),length);
                //TODO: do something with this path
            }
            else{
                throw std::logic_error("Bad option");
            }
        }
    }
    catch(const std::logic_error& e){
        std::cout<<e.what();
        //TODO: Send back Bad option response
    }


}


