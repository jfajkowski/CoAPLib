#include "Frame.h"

Frame::Frame() {

}

void Frame::serialize(unsigned char* buffer) {
    unsigned int* buff_begin_ = (unsigned int*)buffer;
    *buff_begin_ = getVer();
    buff_begin_++; //TODO: should be size of ver, not just ++
    *buff_begin_ = getT();
    buff_begin_++;
    *buff_begin_ = getTKL();
    buff_begin_++;
    *buff_begin_ = getCode();
    buff_begin_++;
    *buff_begin_ = getMessageId();
    buff_begin_++;

    memcpy (buff_begin_, token_.getArray_begin_(), token_.size());
    buff_begin_ += token_.size();

    memcpy (buff_begin_, options_.getArray_begin_(), options_.size());
    buff_begin_ += options_.size();

    if(payload_.size() > 0) {
        *buff_begin_ = PAYLOAD_MARKER;
        buff_begin_++;
    }

    memcpy (buff_begin_, payload_.getArray_begin_(), payload_.size());
    buff_begin_ += payload_.size();

    //TODO: Create tests to validate serialization
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

