#include "CoAPMessage.h"

CoAPMessage::CoAPMessage() {
    header_ = {DEFAULT_VERSION, 0, 0, 0, 0};
}

unsigned int CoAPMessage::serialize(unsigned char* buffer_begin) {
    unsigned char* buffer = buffer_begin;

    insert(buffer, header_);
    insert(buffer, token_);
    insert(buffer, options_);
    insert(buffer, payload_);

    return (unsigned int) (buffer - buffer_begin);
}

void CoAPMessage::insert(unsigned char *&buffer, const Header &header) {
    *buffer = (header.Ver << OFFSET_VER) | (header.T << OFFSET_T) | header.TKL;
	*++buffer = header.Code;
	*++buffer = (unsigned char) (header.MessageId >> OFFSET_MESSAGE_ID);
	*++buffer = (unsigned char) (header.MessageId & MASK_MESSAGE_ID);
}

void CoAPMessage::insert(unsigned char* &buffer, const OptionArray &options) {
    insert(buffer, CoAPOption::serialize(options));
}

void CoAPMessage::insert(unsigned char* &buffer, const ByteArray &bytes) {
    memcpy(buffer, bytes.begin(), bytes.size());
    buffer += bytes.size();
}

void CoAPMessage::deserialize(CoAPMessage* frame, unsigned char *buffer_begin, unsigned int num) {
    unsigned char* buffer = buffer_begin;
    unsigned char* buffer_end = buffer_begin + num;

    extract(&frame->header_, buffer, sizeof(frame->header_));
    frame->token_ = extract(buffer, frame->header_.TKL);
    frame->options_ = extract(buffer, buffer_end);
    frame->setPayload(extract(buffer, buffer_end - buffer));
}

void CoAPMessage::extract(Header* header, unsigned char* &buffer, unsigned int num) {
    header->Ver = (*buffer & MASK_VER) >> OFFSET_VER;
    header->T = (*buffer & MASK_T) >> OFFSET_T;
    header->TKL = (*buffer & MASK_TKL);
    header->Code = (*++buffer);
    header->MessageId = ((*++buffer) << OFFSET_MESSAGE_ID) | (*++buffer);
    ++buffer;
}

OptionArray CoAPMessage::extract(unsigned char* &buffer, unsigned char* &buffer_end) {
    return CoAPOption::deserialize(buffer, buffer_end);;
}

ByteArray CoAPMessage::extract(unsigned char* &buffer, unsigned int num) {
    ByteArray result(buffer, num);
    buffer += num;
    return result;
}

unsigned int CoAPMessage::getVer() const {
    return header_.Ver;
}

unsigned int CoAPMessage::getT() const {
    return header_.T;
}

void CoAPMessage::setT(unsigned int T) {
    header_.T = T;
}

unsigned int CoAPMessage::getTKL() const {
    return header_.TKL;
}

unsigned int CoAPMessage::getCode() const {
    return header_.Code;
}

void CoAPMessage::setCode(unsigned int Code) {
    header_.Code = Code;
}

unsigned int CoAPMessage::getMessageId() const {
    return header_.MessageId;
}

void CoAPMessage::setMessageId(unsigned int MessageId) {
    header_.MessageId = MessageId;
}

const ByteArray &CoAPMessage::getToken() const {
    return token_;
}

void CoAPMessage::setToken(const ByteArray &token) {
    token_ = token;
    header_.TKL = token.size();
}

const OptionArray &CoAPMessage::getOptions() const {
    return options_;
}

void CoAPMessage::addOption(unsigned int option_number, ByteArray option_value) {
    // TODO
}

const ByteArray &CoAPMessage::getPayload() const {
    return payload_;
}

void CoAPMessage::setPayload(const ByteArray &payload) {
    payload_ = payload;
}

void CoAPMessage::print() {
    PRINT("Version: ");
    PRINTLN(header_.Ver);
    PRINT("Type: ");
    PRINTLN(header_.T);
    PRINT("Token length: ");
    PRINTLN(header_.TKL);
    PRINT("Code: ");
    PRINTLN(header_.Code);
    PRINT("Message ID: ");
    PRINTLN(header_.MessageId);

    if (token_.size() != 0) {
        PRINT("Token:");
        PRINTLN(CoAPMessage::toString(payload_));
    }

    if (options_.size() != 0) {
        PRINT("Options:\n");
        print(options_);
    }

    if (payload_.size() != 0) {
        PRINT("Payload: \n\t");
        PRINTLN(CoAPMessage::toString(payload_));
    }
}

const String CoAPMessage::toString(const ByteArray &byte_array) {
    String s;
  
    for(int i = 0; i < byte_array.size(); ++i){
        s += char(byte_array[i]);
    }

    return s;
}

void CoAPMessage::print(const OptionArray &options) {
    unsigned int option_code = 0; 
    for (int i = 0; i < options.size(); ++i) {
        option_code += options[i].getDelta();
        PRINT("\t");
        PRINT(option_code);
        PRINT(": ");
        PRINT(options[i].toString());
        PRINT("\n");
    }
}
