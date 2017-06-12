#ifndef COAPLIB_RADIOMESSAGELISTENER_HPP
#define COAPLIB_RADIOMESSAGELISTENER_HPP

#include "RadioMessage.hpp"

struct RadioMessageListener {
    virtual void operator()(const RadioMessage &message) = 0;
};

#endif //COAPLIB_RADIOMESSAGELISTENER_H
