#ifndef FRAME_H
#define FRAME_H

#include "Array.hpp"
#include "CoAPOption.h"

class CoAPMessage {
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

    void insert(unsigned char* &cursor, const Header &header) const;
    void insert(unsigned char* &cursor, const ByteArray &bytes) const;
    void insert(unsigned char* &cursor, const OptionArray &options) const;
    void insert(unsigned char* &cursor, unsigned char value) const;
    void extractHeader(unsigned char* &cursor, unsigned char* buffer_end);
    void extractToken(unsigned char* &cursor, unsigned char* buffer_end);
    void extractOptions(unsigned char* &cursor, unsigned char* buffer_end);
    void extractPayload(unsigned char *&cursor, unsigned char* buffer_end);
    
    static const String toString(const ByteArray &byte_array);
    void print(const OptionArray &options) const;
public:
    CoAPMessage();

    unsigned int serialize(unsigned char* buffer_begin) const;
    void deserialize(unsigned char* buffer_begin, unsigned int num);

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
    void addOption(const CoAPOption &option);

    const ByteArray &getPayload() const;
    void setPayload(const ByteArray &payload);

    void print() const;
};

#endif //FRAME_H
