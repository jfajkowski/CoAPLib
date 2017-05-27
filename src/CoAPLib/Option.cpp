#include "Option.h"

#include "Arduino.h"


Option::Option() : header_({0, 0}), length(0), delta(0) {}

Option::Option(unsigned int delta, String value) : Option() {
    ByteArray byte_array(value.length());
    for (int i = 0; i < value.length(); ++i) {
        byte_array.pushBack(value.charAt(i));
    }

    setDelta(delta);
    setValue(byte_array);
}

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

unsigned int Option::getDelta() const {
    if (header_.delta < 13) {
        return header_.delta;
    }
    else if (header_.delta == 13) {
        return (unsigned int) (delta + 13);
    }
    else {
        return (unsigned int) (delta + 269);
    }
}

void Option::setDelta(unsigned int delta) {
    if (delta < 13) {
        header_.delta = (unsigned char) delta;
    }
    else if (delta < 269) {
        header_.delta = 13;
        Option::delta = (unsigned int) (delta - 13);
    }
    else {
        header_.delta = 14;
        Option::delta = (unsigned int) (delta - 269);
    }
}

unsigned int Option::getLength() const {
    if (header_.length < 13) {
        return header_.length;
    }
    else if (header_.length == 13) {
        return (unsigned int) (length + 13);
    }
    else {
        return (unsigned int) (length + 269);
    }
}

void Option::setLength(unsigned int length) {
    if (length < 13) {
        header_.length = (unsigned char) length;
    }
    else if (length < 269) {
        header_.length = 13;
        Option::length = (unsigned int) (length - 13);
    }
    else {
        header_.length = 14;
        Option::length = (unsigned int) (length - 269);
    }
}

const ByteArray &Option::getValue() const {
    return value_;
}

void Option::setValue(const ByteArray &value) {
    Option::value_= value;
    setLength((unsigned int) value.size());
}

ByteArray Option::serializeExtendables() const {
    ByteArray result;
    unsigned char char_buffer;
    unsigned char int_buffer[2];

    memcpy(&char_buffer, &header_, sizeof(header_));
    result.pushBack(char_buffer);

    if (header_.length == 13) {
        char_buffer = (unsigned char) length;
        result.pushBack(char_buffer);
    }
    else if (header_.length == 14) {
        memcpy(&int_buffer, &length, sizeof(unsigned int));
        result.pushBack(int_buffer[0]);
        result.pushBack(int_buffer[1]);
    }


    if (header_.delta == 13) {
        char_buffer = (unsigned char) delta;
        result.pushBack(char_buffer);
    }
    else if (header_.delta == 14) {
        memcpy(&int_buffer, &delta, sizeof(unsigned int));
        result.pushBack(int_buffer[0]);
        result.pushBack(int_buffer[1]);
    }

    return result;
}

void Option::deserializeExtendables(unsigned char *&buffer, Option &option) {
    memcpy(&option.header_, buffer, sizeof(option.header_));
    buffer += sizeof(option.header_);

    unsigned char char_buffer;
    unsigned int int_buffer;

    if (option.header_.length == 13) {
        memcpy(&char_buffer, buffer, sizeof(char_buffer));
        buffer += sizeof(char_buffer);
        option.length = char_buffer;
    }
    else if (option.header_.length == 14) {
        memcpy(&int_buffer, buffer, sizeof(int_buffer));
        buffer += sizeof(int_buffer);
        option.length = int_buffer;
    }

    if (option.header_.delta == 13) {
        memcpy(&char_buffer, buffer, sizeof(char_buffer));
        buffer += sizeof(char_buffer);
        option.delta = char_buffer;
    }
    else if (option.header_.delta == 14) {
        memcpy(&int_buffer, buffer, sizeof(int_buffer));
        buffer += sizeof(int_buffer);
        option.delta = int_buffer;
    }
}

const String Option::toString() const {
    String s;
  
    for(int i = 0; i < value_.size(); ++i){
        s += char(value_[i]);
    }

    return s;
}
