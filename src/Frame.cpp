#include <ostream>
#include "Frame.h"

Frame::Frame() {

}

void Frame::serialize(unsigned char* buffer) {
    unsigned int* buff_begin_ = (unsigned int*)buffer;

    memcpy(buff_begin_, (unsigned char*)&header_, 4);
    buff_begin_++;

    memcpy(buff_begin_, token_.getArray_begin_(), token_.size());
    buff_begin_++;

    memcpy(buff_begin_, options_.getArray_begin_(), options_.size());
    buff_begin_++;

    if(payload_.size() > 0) {
        *buff_begin_ = PAYLOAD_MARKER;
        buff_begin_++;
    }

    memcpy(buff_begin_, payload_.getArray_begin_(), payload_.size());
}

std::ostream& Frame::serialize(std::ostream& stream) {
    stream.write((char*)&header_, 4);
    stream.write((char*)token_.getArray_begin_(), token_.size());
    stream.write((char*)options_.getArray_begin_(), options_.size());
    stream.write((char*)payload_.getArray_begin_(), payload_.size());
    return stream;

    //TODO: Compare two serialization methods above, choose better one and test it.
}


unsigned int Frame::getVer() const {
    return header_.Ver;
}

void Frame::setVer(unsigned int Ver) {
    header_.Ver = Ver;
}

unsigned int Frame::getT() const {
    return header_.T;
}

void Frame::setT(unsigned int T) {
    header_.T = T;
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

const ByteArray &Frame::getToken_() const {
    return token_;
}

void Frame::setToken_(const ByteArray &token_) {
    Frame::token_ = token_;
}

const ByteArray &Frame::getOptions_() const {
    return options_;
}

void Frame::setOptions_(const ByteArray &options_) {
    Frame::options_ = options_;
}

const ByteArray &Frame::getPayload_() const {
    return payload_;
}

void Frame::setPayload_(const ByteArray &payload_) {
    Frame::payload_ = payload_;
}

unsigned int Frame::getTKL() const {
    return Frame::header_.TKL;
}

