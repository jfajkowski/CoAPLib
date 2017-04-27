#include "CoAP.h"

std::ostream &operator<<(std::ostream &os, const Header &header) {
    os << header.Ver << header.T << header.TKL << header.Code << header.MessageId;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Frame &frame) {
    os << frame.header << frame.token << frame.options << frame.payload;
    return os;
}
