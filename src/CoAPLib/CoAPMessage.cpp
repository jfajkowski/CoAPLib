#include "CoAPMessage.h"

CoAPMessage::CoAPMessage() {
    header_ = {DEFAULT_VERSION, 0, 0, 0, 0};
}

unsigned int CoAPMessage::serialize(unsigned char* buffer_begin) {
    unsigned char* cursor = buffer_begin;

    insert(cursor, header_);
    insert(cursor, token_);
    insert(cursor, options_);
    insert(cursor, payload_);

    return cursor - buffer_begin;
}

void CoAPMessage::insert(unsigned char* &cursor, const Header &header) {
    *cursor = (header.Ver << OFFSET_VER) | (header.T << OFFSET_T) | header.TKL;
	*++cursor = header.Code;
	*++cursor = (unsigned char) (header.MessageId >> OFFSET_MESSAGE_ID);
	*++cursor = (unsigned char) (header.MessageId & MASK_MESSAGE_ID);
}

void CoAPMessage::insert(unsigned char* &cursor, const ByteArray &bytes) {
    bytes.serialize(cursor);
    cursor += bytes.size();
}

void CoAPMessage::insert(unsigned char* &cursor, const OptionArray &options) {
    options.serialize(cursor);
}

void CoAPMessage::deserialize(unsigned char *buffer_begin, unsigned int num) {
    unsigned char* cursor = buffer_begin;
    unsigned char* buffer_end = buffer_begin + num;

    extractHeader(cursor, buffer_end);
    extractToken(cursor, buffer_end);
    extractOptions(cursor, buffer_end);
    extractPayload(cursor, buffer_end);
}

void CoAPMessage::extractHeader(unsigned char *&cursor, unsigned char* buffer_end) {
    header_.Ver = (*cursor & MASK_VER) >> OFFSET_VER;
    header_.T = (*cursor & MASK_T) >> OFFSET_T;
    header_.TKL = (*cursor & MASK_TKL);
    header_.Code = (*++cursor);
    header_.MessageId = ((*++cursor) << OFFSET_MESSAGE_ID) | (*++cursor);
    ++cursor;
}

void CoAPMessage::extractToken(unsigned char *&cursor, unsigned char* buffer_end) {
    token_.deserialize(cursor, header_.TKL);
    cursor += header_.TKL;
}

void CoAPMessage::extractOptions(unsigned char* &cursor, unsigned char* buffer_end) {
    CoAPOption::deserialize(cursor, buffer_end, options_);
}

void CoAPMessage::extractPayload(unsigned char *&cursor, unsigned char* buffer_end) {
    payload_.deserialize(cursor, buffer_end - cursor);
    cursor += buffer_end - cursor;
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

void CoAPMessage::addOption(const CoAPOption &option) {
    for (unsigned int i = 0; i < options_.size(); ++i) {
        if (option.getNumber() < options_[i].getNumber()) {
            options_.insert(option, i);
            return;
        }
    }

    options_.pushBack(option);
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
        option_code += options[i].getNumber();
        PRINT("\t");
        PRINT(option_code);
        PRINT(": ");
        PRINT(options[i].toString());
        PRINT("\n");
    }
}
