#include "Frame.h"

Frame::Frame() {
    header_ = {DEFAULT_VERSION, 0, 0, 0, 0};
}

unsigned int Frame::serialize(unsigned char* buffer_begin) {
    unsigned char* buffer = buffer_begin;

    insert(buffer, header_);
    insert(buffer, token_);
    insert(buffer, Option::serialize(options_));
    insert(buffer, payload_);

    return (unsigned int) (buffer - buffer_begin);
}

void Frame::insert(unsigned char *&buffer, const Header &header) {
    *buffer = (header.Ver << OFFSET_VER) | (header.T << OFFSET_T) | header.TKL;
	*++buffer = header.Code;
	*++buffer = (unsigned char) (header.MessageId >> OFFSET_MESSAGE_ID);
	*++buffer = (unsigned char) (header.MessageId & MASK_MESSAGE_ID);
}

void Frame::insert(unsigned char* &buffer, const ByteArray &array) {
    memcpy(buffer, array.begin(), array.size());
    buffer += array.size();
}

void Frame::deserialize(Frame* frame, unsigned char *buffer_begin, unsigned int num) {
    unsigned char* buffer = buffer_begin;
    unsigned char* buffer_end = buffer_begin + num;
	
    extract(&frame->header_, buffer, sizeof(frame->header_));
    frame->setToken(extract(buffer, frame->header_.TKL));
	// TODO Change deserialization method.
    //frame->addOption(Option::deserialize(buffer));
    //frame->setPayload(extract(buffer, buffer_end - buffer));
}

void Frame::extract(Header* header, unsigned char* buffer, unsigned int num) {
    header->Ver = (*buffer & MASK_VER) >> OFFSET_VER;    
    header->T = (*buffer & MASK_T) >> OFFSET_T;
    header->TKL = (*buffer & MASK_TKL);
    header->Code = (*++buffer);
    header->MessageId = ((*++buffer) << OFFSET_MESSAGE_ID) | (*++buffer);
}

ByteArray Frame::extract(unsigned char *&buffer, unsigned int num) {
    return ByteArray(buffer, num);
}

unsigned int Frame::getVer() const {
    return header_.Ver;
}

unsigned int Frame::getT() const {
    return header_.T;
}

void Frame::setT(unsigned int T) {
    header_.T = T;
}

unsigned int Frame::getTKL() const {
    return header_.TKL;
}

unsigned int Frame::getCode() const {
    return header_.Code;
}

void Frame::setCode(unsigned int Code) {
    header_.Code = Code;
}

unsigned int Frame::getMessageId() const {
    return header_.MessageId;
}

void Frame::setMessageId(unsigned int MessageId) {
    header_.MessageId = MessageId;
}

const ByteArray &Frame::getToken() const {
    return token_;
}

void Frame::setToken(const ByteArray &token) {
    token_ = token;
    header_.TKL = token.size();
}

const OptionArray &Frame::getOptions() const {
    return options_;
}

void Frame::addOption(unsigned int option_number, ByteArray option_value) {
    options_ = option;
}

const ByteArray &Frame::getPayload() const {
    return payload_;
}

void Frame::setPayload(const ByteArray &payload) {
    payload_ = payload;
}

void Frame::print() {
    Serial.print("Version: ");
    Serial.println(header_.Ver);
    Serial.print("Type: ");
    Serial.println(header_.T);
    Serial.print("Token length: ");
    Serial.println(header_.TKL);
    Serial.print("Code: ");
    Serial.println(header_.Code);
    Serial.print("Message ID: ");
    Serial.println(header_.MessageId);

    if (token_.size() != 0) {
        Serial.print("Token:");
        Serial.println(Frame::toString(payload_));
    }

    if (options_.size() != 0) {
        Serial.print("Options:\n");
        print(options_);
    }

    if (payload_.size() != 0) {
        Serial.print("Payload: \n\t");
        Serial.println(Frame::toString(payload_));
    }
}

const String Frame::toString(const ByteArray &byte_array) {
    String s;
  
    for(int i = 0; i < byte_array.size(); ++i){
        s += char(byte_array[i]);
    }

    return s;
}

void Frame::print(const OptionArray &options) {
    unsigned int option_code = 0; 
    for (int i = 0; i < options.size(); ++i) {
        option_code += options[i].getDelta();
        Serial.print("\t");
        Serial.print(option_code);
        Serial.print(": ");
        Serial.print(options[i].toString());
        Serial.print("\n");
    }
}
