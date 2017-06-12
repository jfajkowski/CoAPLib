#ifndef COAPLIB_RADIOMESSAGE_HPP
#define COAPLIB_RADIOMESSAGE_HPP

#include "../Environment.h"

struct RadioMessage {
    unsigned short message_id : 16;
    unsigned short code : 1; //0 - PUT, 1 - GET
    unsigned short resource : 1; //0 - Lamp, 1 - Speaker
    unsigned short value : 14;

    void print() const {
        PRINTLN("---Radio message---");
        PRINT("Message ID: ");
        PRINTLN(message_id);
        PRINT("Code: ");
        PRINTLN(code);
        PRINT("Resource: ");
        PRINTLN(resource);
        PRINT("Value: ");
        PRINTLN(value);
    }
};

#endif //COAPLIB_RADIOMESSAGE_HPP
