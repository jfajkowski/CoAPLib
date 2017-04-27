#include "Frame.h"

Frame::Frame() {

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

const ucArray &Frame::getToken_() const {
    return token_;
}

void Frame::setToken_(const ucArray &token_) {
    Frame::token_ = token_;
}

const ucArray &Frame::getOptions_() const {
    return options_;
}

void Frame::setOptions_(const ucArray &options_) {
    Frame::options_ = options_;
}

const ucArray &Frame::getPayload_() const {
    return payload_;
}

void Frame::setPayload_(const ucArray &payload_) {
    Frame::payload_ = payload_;
}