#ifndef FRAME_H
#define FRAME_H

#include "Arduino.h"
#include "Array.hpp"
#include "Option.h"

class Frame {
private:
    struct Header {
        unsigned int Ver : 2;
        unsigned int T : 2;
        unsigned int TKL : 4;
        unsigned int Code : 8;
        unsigned int MessageId : 16;
    } header_;

    ByteArray token_;
    OptionArray options_;
    ByteArray payload_;

    static void insert(unsigned char* &buffer, const Header &header);
    static void insert(unsigned char* &buffer, const ByteArray &array);
	static void extract(Header* header, unsigned char* buffer, unsigned int num);
    static ByteArray extract(unsigned char *&buffer, unsigned int num);
    
    static const String toString(const ByteArray &byte_array);
    void print(const OptionArray &options);
public:
    Frame();

    unsigned int serialize(unsigned char* buffer_begin);
    static void deserialize(Frame* frame, unsigned char *buffer_begin, unsigned int num);

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

    const OptionArray &getOptions() const;
    void addOption(unsigned int option_number, ByteArray option_value);

    const ByteArray &getPayload() const;
    void setPayload(const ByteArray &payload);

    void print();
};

#endif //FRAME_H
