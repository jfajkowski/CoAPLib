#ifndef ARDUINODEMO_OPTION_H
#define ARDUINODEMO_OPTION_H

#include "WString.h"

#include "Array.hpp"
#include "Constants.h"

class Option;
typedef Array<Option> OptionArray;

class Option {
    struct Header {
        unsigned char length : 4;
        unsigned char delta : 4;
    } header_;
    unsigned int length;
    unsigned int delta;
    ByteArray value_;

    ByteArray serialize() const;
    ByteArray serializeExtendables() const;
    static void deserializeExtendables(unsigned char *&buffer, Option &option);

    void setLength(unsigned int length);

public:
    Option();
    Option(unsigned int delta, String value);

    static ByteArray serialize(const OptionArray &options);
    static OptionArray deserialize(unsigned char *&buffer);

    Option &operator=(const Option & option);

    unsigned int getDelta() const;
    void setDelta(unsigned int delta);

    unsigned int getLength() const;

    const ByteArray &getValue() const;
    void setValue(const ByteArray &value);

    const String toString() const;
    static void print(const OptionArray &options);
};

#endif //ARDUINODEMO_OPTION_H
