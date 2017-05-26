#ifndef ARDUINODEMO_OPTION_H
#define ARDUINODEMO_OPTION_H

#include <cstring>
#include <string>
#include "Array.hpp"
#include "Constants.h"

class Option;
typedef Array<Option> OptionArray;

class Option {
    struct Header {
        unsigned char length : 4;
        unsigned char delta : 4;
    } header_;
    unsigned short length;
    unsigned short delta;
    ByteArray value_;

    ByteArray serialize() const;
    ByteArray serializeExtendables() const;
    static void deserializeExtendables(unsigned char *&buffer, Option &option);

    void setLength(unsigned short length);

public:
    static ByteArray serialize(const OptionArray &options);
    static OptionArray deserialize(unsigned char *&buffer);

    Option &operator=(const Option & option);

    unsigned short getDelta() const;
    void setDelta(unsigned short delta);

    unsigned short getLength() const;

    const ByteArray &getValue() const;
    void setValue(const ByteArray &value);

    std::string toString();
};

#endif //ARDUINODEMO_OPTION_H
