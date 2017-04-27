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

    unsigned int getVer() const;
    void setVer(unsigned int Ver);
    unsigned int getT() const;
    void setT(unsigned int T);
    unsigned int getCode() const;
    void setCode(unsigned int Code);
    unsigned int getMessageId() const;
    void setMessageId(unsigned int MessageId);
    const ucArray &getToken_() const;
    void setToken_(const ucArray &token_);
    const ucArray &getOptions_() const;
    void setOptions_(const ucArray &options_);
    const ucArray &getPayload_() const;
    void setPayload_(const ucArray &payload_);
};

#endif //ARDUINODEMO_FRAME_H
