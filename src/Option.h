#ifndef ARDUINODEMO_OPTION_H
#define ARDUINODEMO_OPTION_H

#include <cstring>
#include "Array.hpp"
#include "Constants.h"

class Option;
typedef Array<Option> OptionArray;

class Option {
    struct Header {
        unsigned char length : 4;
        unsigned char delta : 4;
    } header_;
    ByteArray value_;

    ByteArray serialize() const;
public:
    static ByteArray serialize(const OptionArray &options);
    static OptionArray deserialize(unsigned char *&buffer);

    Option &operator=(const Option & option);

    unsigned char getDelta() const;
    void setDelta(unsigned char delta);

    unsigned char getLength() const;

    const ByteArray &getValue() const;
    void setValue(const ByteArray &value);
};

#endif //ARDUINODEMO_OPTION_H
