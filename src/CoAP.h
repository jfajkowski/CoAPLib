#ifndef ARDUINODEMO_LIBRARY_H
#define ARDUINODEMO_LIBRARY_H

#include <ostream>

struct Header {
    unsigned int Ver : 2;
    unsigned int T : 2;
    unsigned int TKL : 4;
    unsigned int Code : 8;
    unsigned int MessageId : 16;

    friend std::ostream &operator<<(std::ostream &os, const Header &header);
};

struct Frame {
    static const unsigned char PAYLOAD_MARKER = 0xFF;

    Header header;
    unsigned char* token;
    unsigned char* options;
    unsigned char payload_marker;
    unsigned char* payload;

    //TODO: Serialization to unsigned char array.

    friend std::ostream &operator<<(std::ostream &os, const Frame &frame);
};

#endif