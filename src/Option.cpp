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

    result += serializeExtendables();
    result += value_;

    return result;
}

OptionArray Option::deserialize(unsigned char *&buffer) {
    OptionArray optionArray;
    ByteArray value;

    while (*buffer != PAYLOAD_MARKER) {
        Option option;
        deserializeExtendables(buffer, option);
        value = ByteArray(buffer, option.getLength());
        buffer += option.getLength();

        option.setDelta(option.getDelta());
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
        delta = option.delta;
        length = option.length;
        value_ = option.value_;
    }
    return *this;
}

unsigned short Option::getDelta() const {
    if (header_.delta < 13) {
        return header_.delta;
    }
    else if (header_.delta == 13) {
        return (unsigned short) (delta + 13);
    }
    else {
        return (unsigned short) (delta + 269);
    }
}

void Option::setDelta(unsigned short delta) {
    if (delta < 13) {
        header_.delta = (unsigned char) delta;
    }
    else if (delta < 269) {
        header_.delta = 13;
        Option::delta = (unsigned short) (delta - 13);
    }
    else {
        header_.delta = 14;
        Option::delta = (unsigned short) (delta - 269);
    }
}

unsigned short Option::getLength() const {
    if (header_.length < 13) {
        return header_.length;
    }
    else if (header_.length == 13) {
        return (unsigned short) (length + 13);
    }
    else {
        return (unsigned short) (length + 269);
    }
}

void Option::setLength(unsigned short length) {
    if (length < 13) {
        header_.length = (unsigned char) length;
    }
    else if (length < 269) {
        header_.length = 13;
        Option::length = (unsigned short) (length - 13);
    }
    else {
        header_.length = 14;
        Option::length = (unsigned short) (length - 269);
    }
}

const ByteArray &Option::getValue() const {
    return value_;
}

void Option::setValue(const ByteArray &value) {
    Option::value_ = value;
    setLength((unsigned short) value.size());
}

ByteArray Option::serializeExtendables() const {
    ByteArray result;
    unsigned char char_buffer;
    unsigned char short_buffer[2];

    memcpy(&char_buffer, &header_, sizeof(header_));
    result.pushBack(char_buffer);

    if (header_.length == 13) {
        char_buffer = (unsigned char) length;
        result.pushBack(char_buffer);
    }
    else if (header_.length == 14) {
        memcpy(&short_buffer, &length, sizeof(unsigned short));
        result.pushBack(short_buffer[0]);
        result.pushBack(short_buffer[1]);
    }


    if (header_.delta == 13) {
        char_buffer = (unsigned char) delta;
        result.pushBack(char_buffer);
    }
    else if (header_.delta == 14) {
        memcpy(&short_buffer, &delta, sizeof(unsigned short));
        result.pushBack(short_buffer[0]);
        result.pushBack(short_buffer[1]);
    }

    return result;
}

void Option::deserializeExtendables(unsigned char *&buffer, Option &option) {
    memcpy(&option.header_, buffer, sizeof(option.header_));
    buffer += sizeof(option.header_);

    unsigned char char_buffer;
    unsigned short short_buffer;

    if (option.header_.length == 13) {
        memcpy(&char_buffer, buffer, sizeof(char_buffer));
        buffer += sizeof(char_buffer);
        option.length = char_buffer;
    }
    else if (option.header_.length == 14) {
        memcpy(&short_buffer, buffer, sizeof(short_buffer));
        buffer += sizeof(short_buffer);
        option.length = short_buffer;
    }

    if (option.header_.delta == 13) {
        memcpy(&char_buffer, buffer, sizeof(char_buffer));
        buffer += sizeof(char_buffer);
        option.delta = char_buffer;
    }
    else if (option.header_.delta == 14) {
        memcpy(&short_buffer, buffer, sizeof(short_buffer));
        buffer += sizeof(short_buffer);
        option.delta = short_buffer;
    }
}
