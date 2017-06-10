#include "CoAPOption.h"

CoAPOption::CoAPOption() : header_({0, 0}), length(0), delta(0) {}

CoAPOption::CoAPOption(unsigned int delta, String value) : CoAPOption() {
    ByteArray byte_array(value.length());
    for (int i = 0; i < value.length(); ++i) {
        byte_array.pushBack((const unsigned char &) value[i]);
    }

    setDelta(delta);
    setValue(byte_array);
}

ByteArray CoAPOption::serialize(const OptionArray &options) {
    ByteArray byteArray;

    for (int i = 0; i < options.size(); ++i) {
        byteArray += options[i].serialize();
    }

    byteArray.pushBack(PAYLOAD_MARKER);

    return byteArray;
}

ByteArray CoAPOption::serialize() const {
    ByteArray result;

    result += serializeExtendables();
    result += value_;

    return result;
}

OptionArray CoAPOption::deserialize(unsigned char *&buffer, unsigned char* &buffer_end) {
    OptionArray optionArray;
    ByteArray value;

    while (buffer != buffer_end && *buffer != PAYLOAD_MARKER) {
        CoAPOption option;
        deserializeExtendables(buffer, option);
        value = ByteArray(buffer, option.getLength());
        buffer += option.getLength();

        option.setDelta(option.getDelta());
        option.setValue(value);

        optionArray.pushBack(option);
    }

    if (buffer != buffer_end)
        ++buffer;

    return optionArray;
}

CoAPOption &CoAPOption::operator=(const CoAPOption &option) {
    if(&option != this) {
        header_.delta = option.header_.delta;
        header_.length = option.header_.length;
        delta = option.delta;
        length = option.length;
        value_ = option.value_;
    }
    return *this;
}

unsigned int CoAPOption::getDelta() const {
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

void CoAPOption::setDelta(unsigned int delta) {
    if (delta < 13) {
        header_.delta = (unsigned char) delta;
    }
    else if (delta < 269) {
        header_.delta = 13;
        CoAPOption::delta = (unsigned int) (delta - 13);
    }
    else {
        header_.delta = 14;
        CoAPOption::delta = (unsigned int) (delta - 269);
    }
}

unsigned int CoAPOption::getLength() const {
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

void CoAPOption::setLength(unsigned int length) {
    if (length < 13) {
        header_.length = (unsigned char) length;
    }
    else if (length < 269) {
        header_.length = 13;
        CoAPOption::length = (unsigned int) (length - 13);
    }
    else {
        header_.length = 14;
        CoAPOption::length = (unsigned int) (length - 269);
    }
}

const ByteArray &CoAPOption::getValue() const {
    return value_;
}

void CoAPOption::setValue(const ByteArray &value) {
    CoAPOption::value_= value;
    setLength((unsigned int) value.size());
}

ByteArray CoAPOption::serializeExtendables() const {
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

void CoAPOption::deserializeExtendables(unsigned char *&buffer, CoAPOption &option) {
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

const String CoAPOption::toString() const {
    String s;
  
    for(int i = 0; i < value_.size(); ++i){
        s += char(value_[i]);
    }

    return s;
}
