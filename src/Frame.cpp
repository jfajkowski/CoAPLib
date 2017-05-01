#include <ostream>
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
    memcpy(buffer, &header, sizeof(header));
    buffer += sizeof(header);
}

void Frame::insert(unsigned char* &buffer, const ByteArray &array) {
    memcpy(buffer, array.begin(), array.size());
    buffer += array.size();
}

Frame Frame::deserialize(unsigned char* buffer_begin, unsigned int num) {
    Frame frame;

    unsigned char* buffer = buffer_begin;
    memcpy(&frame.header_, buffer, sizeof(frame.header_));
    buffer += sizeof(frame.header_);

    extract(buffer, frame.header_.TKL);

    //TODO: Extract options and payload.

    return frame;
}

ByteArray Frame::extract(unsigned char *&buffer, unsigned int num) {
    //TODO Implementation.
    return ByteArray();
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
    Frame::token_ = token;
    Frame::header_.TKL = token.size();
}

const OptionArray &Frame::getOptions() const {
    return options_;
}

void Frame::setOptions(const OptionArray &options) {
    Frame::options_ = options;
}

const ByteArray &Frame::getPayload() const {
    return payload_;
}

void Frame::setPayload(const ByteArray &payload) {
    Frame::payload_ = payload;
}
