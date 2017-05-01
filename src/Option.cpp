#include "Option.h"

ByteArray Option::serialize(const OptionArray &options) const {
    //TODO implementation
    return ByteArray();
}

OptionArray Option::deserialize(unsigned char* &buffer, unsigned int num) {
    //TODO implementation
    return OptionArray();
}

unsigned char Option::getDelta() const {
    return delta;
}

void Option::setDelta(unsigned char delta) {
    Option::delta = delta;
}

unsigned char Option::getLength() const {
    return length;
}

const ByteArray &Option::getValue() const {
    return value;
}

void Option::setValue(const ByteArray &value) {
    Option::value = value;
    Option::length = (unsigned char) value.size();
}
