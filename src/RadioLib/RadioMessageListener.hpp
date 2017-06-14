#ifndef COAPLIB_RADIOMESSAGELISTENER_HPP
#define COAPLIB_RADIOMESSAGELISTENER_HPP

#include "RadioMessage.hpp"

/** Part of callback used to pass message from library to "ino" **/
struct RadioMessageListener {
    virtual void operator()(const RadioMessage &message) = 0;
};

#endif //COAPLIB_RADIOMESSAGELISTENER_H
