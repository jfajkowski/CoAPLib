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

    static void insert(unsigned char* &buffer, const ByteArray &array);
    static ByteArray extract(unsigned char *&buffer, unsigned int num);
public:
    static const unsigned int DEFAULT_VERSION = 0x01;
    static const unsigned char PAYLOAD_MARKER = 0xFF;

    Frame();

    unsigned int serialize(unsigned char* buffer_begin);
    static Frame deserialize(unsigned char *buffer_begin, unsigned int num);
    std::ostream &serialize(std::ostream &stream);

    unsigned int getVer() const;

    unsigned int getT() const;
    void setT(unsigned int T);

    unsigned int getTKL() const;

    unsigned int getCode() const;
    void setCode(unsigned int Code);

    unsigned int getMessageId() const;
    void setMessageId(unsigned int MessageId);

    const ByteArray &getToken() const;
    void setToken(const ByteArray &token);

    const ByteArray &getOptions() const;
    void setOptions(const ByteArray &options);

    const ByteArray &getPayload() const;
    void setPayload(const ByteArray &payload);
};

#endif //ARDUINODEMO_FRAME_H
