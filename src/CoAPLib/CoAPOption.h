#ifndef OPTION_H
#define OPTION_H

#include "Array.hpp"
#include "CoAPConstants.h"

class CoAPOption;
typedef Array<CoAPOption> OptionArray;

class CoAPOption {
    unsigned int number_;
    ByteArray value_;

    void insert(unsigned char* &cursor, unsigned int delta, unsigned int length) const;
    void insert(unsigned char* &cursor, const ByteArray &bytes) const;
    void extractExtendables(unsigned char* &cursor, unsigned int &delta, unsigned int &length);
    void extractValue(unsigned char* &cursor, unsigned int num);

    void prepareExtendable(unsigned char &header_value, unsigned int &extendable_value) const;
    void insertHeaderValues(unsigned char* &cursor, unsigned char &header_delta, unsigned char &header_length) const;
    void insertExtendableValue(unsigned char* &cursor, unsigned char header_value, unsigned int extendable_value) const;
    void extractHeaderValues(unsigned char* &cursor, unsigned char &header_delta, unsigned char &header_length) const;
    void extractExtendableValue(unsigned char* &cursor, unsigned char header_value, unsigned int &extendable_value);
public:
    CoAPOption();
    CoAPOption(unsigned int number, String value);
    CoAPOption(unsigned int number, ByteArray value);

    static void serialize(unsigned char *&cursor, const OptionArray &options);
    static void deserialize(unsigned char *&cursor, unsigned char *buffer_end, OptionArray &options);
    void serialize(unsigned char* &cursor, unsigned int delta) const;
    void deserialize(unsigned char* &cursor, unsigned char* &buffer_end, unsigned int delta_sum);

    CoAPOption &operator=(const CoAPOption & option);

    unsigned int getNumber() const;
    const ByteArray &getValue() const;

    const String toString() const;
};

#endif //OPTION_H
