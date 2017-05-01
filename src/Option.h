#ifndef ARDUINODEMO_OPTION_H
#define ARDUINODEMO_OPTION_H

#include "Array.hpp"

typedef Array<Option> OptionArray;

class Option {
    unsigned char delta : 4;
    unsigned char length : 4;
    ByteArray value;

public:
    static ByteArray serialize(const OptionArray &options) const;
    static OptionArray deserialize(unsigned char* &buffer, unsigned int num);

    unsigned char getDelta() const;
    void setDelta(unsigned char delta);

    unsigned char getLength() const;

    const ByteArray &getValue() const;
    void setValue(const ByteArray &value);
};

#endif //ARDUINODEMO_OPTION_H
