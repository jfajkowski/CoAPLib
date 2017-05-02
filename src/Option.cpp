#include "Option.h"

ByteArray Option::serialize(const OptionArray &options) {
    ByteArray byteArray;

    for (int i = 0; i < options.size(); ++i) {
        byteArray += options[i].serialize();
    }

    byteArray.pushBack(PAYLOAD_MARKER);

    return byteArray;
}

ByteArray Option::serialize() const {
    ByteArray result;

    unsigned char buffer[1];
    memcpy(&buffer, &header_, sizeof(header_));
    result.pushBack(buffer[0]);

    result += value_;

    return result;
}

OptionArray Option::deserialize(unsigned char* &buffer, unsigned int num) {
    //TODO implementation
    return OptionArray();
}

Option &Option::operator=(const Option &option) {
    if(&option != this) {
        header_.delta = option.header_.delta;
        header_.length = option.header_.length;
        value_ = option.value_;
    }
    return *this;
}

unsigned char Option::getDelta() const {
    return header_.delta;
}

void Option::setDelta(unsigned char delta) {
    Option::header_.delta = delta;
}

unsigned char Option::getLength() const {
    return header_.length;
}

const ByteArray &Option::getValue() const {
    return value_;
}

void Option::setValue(const ByteArray &value) {
    Option::value_ = value;
    Option::header_.length = (unsigned char) value.size();
}
