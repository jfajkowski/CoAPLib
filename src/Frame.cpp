#include "Frame.h"

Frame::Frame() {

}

Frame::Frame(Header header, ucArray token, ucArray options, ucArray payload) {
    header_ = header;
    token_ = token;
    options_ = options;
    payload_ = payload;

    header_.TKL = token_.size();
}

Header Frame::getHeader() {
    return header_;
}

ucArray Frame::getToken() {
    return token_;
}

ucArray Frame::getOptions() {
    return options_;
}

ucArray Frame::getPayload() {
    return payload_;
}


