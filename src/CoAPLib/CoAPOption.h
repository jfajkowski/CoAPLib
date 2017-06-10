#ifndef OPTION_H
#define OPTION_H

#include "Array.hpp"
#include "Constants.h"

class CoAPOption;
typedef Array<CoAPOption> OptionArray;

class CoAPOption {
    struct Header {
        unsigned char length : 4;
        unsigned char delta : 4;
    } header_;
    unsigned int length;
    unsigned int delta;
    ByteArray value_;

    ByteArray serialize() const;
    ByteArray serializeExtendables() const;
    static void deserializeExtendables(unsigned char *&buffer, CoAPOption &option);

    void setLength(unsigned int length);

public:
    CoAPOption();
    CoAPOption(unsigned int delta, String value);

    static ByteArray serialize(const OptionArray &options);
    static OptionArray deserialize(unsigned char* &buffer, unsigned char* &buffer_end);

    CoAPOption &operator=(const CoAPOption & option);

    unsigned int getDelta() const;
    void setDelta(unsigned int delta);

    unsigned int getLength() const;

    const ByteArray &getValue() const;
    void setValue(const ByteArray &value);

    const String toString() const;
};

#endif //OPTION_H
