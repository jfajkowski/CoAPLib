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

    unsigned char buffer;
    memcpy(&buffer, &header_, sizeof(header_));
    result.pushBack(buffer);

    result += value_;

    return result;
}

OptionArray Option::deserialize(unsigned char *&buffer) {
    OptionArray optionArray;
    Header header;
    ByteArray value;

    while (*buffer != PAYLOAD_MARKER) {
        memcpy(&header, buffer, sizeof(header));
        buffer += sizeof(header);

        value = ByteArray(buffer, header.length);
        buffer += header.length;

        Option option;
        option.setDelta(header.delta);
        option.setValue(value);

        optionArray.pushBack(option);
    }

    ++buffer;
    return optionArray;
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
