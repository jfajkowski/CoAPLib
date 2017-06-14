#ifndef COAPLIB_COAPMESSAGELISTENER_H
#define COAPLIB_COAPMESSAGELISTENER_H

#include "CoAPMessage.h"

/** Part of callback used to pass message from library to "ino" **/
struct CoAPMessageListener {
    virtual void operator()(const CoAPMessage &message) = 0;
};

#endif //COAPLIB_COAPMESSAGELISTENER_H
