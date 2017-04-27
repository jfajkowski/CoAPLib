#ifndef ARDUINODEMO_FRAME_H
#define ARDUINODEMO_FRAME_H

#include "Array.hpp"

class Frame;
typedef Array<unsigned char> ucArray;

struct Header {
    unsigned int Ver : 2;
    unsigned int T : 2;
    unsigned int TKL : 4;
    unsigned int Code : 8;
    unsigned int MessageId : 16;
};


class Frame {
private:
    Header header_;
    ucArray token_;
    ucArray options_;
    ucArray payload_;
public:
    static const unsigned char PAYLOAD_MARKER = 0xFF;

    Frame();
    Frame(Header header, ucArray token, ucArray options, ucArray payload);

    Header getHeader();
    ucArray getToken();
    ucArray getOptions();
    ucArray getPayload();

};

#endif //ARDUINODEMO_FRAME_H
