#ifndef COAPLIB_RADIOMESSAGE_HPP
#define COAPLIB_RADIOMESSAGE_HPP

struct RadioMessage {
    unsigned short message_id : 16;
    unsigned short code : 1; //0 - PUT, 1 - GET
    unsigned short resource : 1; //0 - Lamp, 1 - Speaker
    unsigned short value : 14;
};

#endif //COAPLIB_RADIOMESSAGE_HPP
