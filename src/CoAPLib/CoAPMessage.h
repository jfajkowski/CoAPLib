#ifndef FRAME_H
#define FRAME_H

#include "Array.hpp"
#include "CoAPOption.h"

/**
 * Describes CoAP message and provides options for serialization/deserialization
 */
class CoAPMessage {
private:
    struct Header {
        unsigned short Ver : 2;
        unsigned short T : 2;
        unsigned short TKL : 4;
        unsigned short Code : 8;
        unsigned short MessageId : 16;
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

    unsigned short getVer() const;

    unsigned short getT() const;
    void setT(unsigned short T);

    unsigned short getTKL() const;

    unsigned short getCode() const;
    void setCode(unsigned short Code);

    unsigned short getMessageId() const;
    void setMessageId(unsigned short MessageId);

    const ByteArray &getToken() const;
    void setToken(const ByteArray &token);

    const OptionArray &getOptions() const;
    void addOption(const CoAPOption &option);

    const ByteArray &getPayload() const;
    void setPayload(const ByteArray &payload);

    void print() const;
};

#endif //FRAME_H
