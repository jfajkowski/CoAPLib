#ifndef COAPLIB_COAPMESSAGELISTENER_H
#define COAPLIB_COAPMESSAGELISTENER_H

#include "CoAPMessage.h"

struct CoAPMessageListener {
    virtual void operator()(const CoAPMessage &message) = 0;
};

#endif //COAPLIB_COAPMESSAGELISTENER_H
