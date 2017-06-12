#ifndef COAPLIB_RADIOMESSAGE_HPP
#define COAPLIB_RADIOMESSAGE_HPP

struct RadioMessage {
    unsigned int message_id : 16;
    unsigned char code : 1; //0 - PUT, 1 - GET
    unsigned char resource : 1; //0 - Lamp, 1 - Speaker
    unsigned char value; //TODO: extend length
};

#endif //COAPLIB_RADIOMESSAGE_HPP
