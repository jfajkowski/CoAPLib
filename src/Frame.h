#ifndef ARDUINODEMO_FRAME_H
#define ARDUINODEMO_FRAME_H

#include <cstring>
#include "Array.hpp"

class Frame;
typedef Array<unsigned char> ByteArray;

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
    ByteArray token_;
    ByteArray options_;
    ByteArray payload_;
public:
    static const unsigned char PAYLOAD_MARKER = 0xFF;

    Frame();

    void serialize(unsigned char* buffer);

    unsigned int getVer() const;
    void setVer(unsigned int Ver);

    unsigned int getT() const;
    void setT(unsigned int T);

    unsigned int getTKL() const;

    unsigned int getCode() const;
    void setCode(unsigned int Code);

    unsigned int getMessageId() const;
    void setMessageId(unsigned int MessageId);

    const ByteArray &getToken_() const;
    void setToken_(const ByteArray &token_);

    const ByteArray &getOptions_() const;
    void setOptions_(const ByteArray &options_);

    const ByteArray &getPayload_() const;
    void setPayload_(const ByteArray &payload_);
};

#endif //ARDUINODEMO_FRAME_H
