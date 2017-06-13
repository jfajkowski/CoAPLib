#include "CoAPHandler.h"


void CoAPHandler::handleMessage(CoAPMessage &message) {
    DEBUG_FUNCTION(
            DEBUG_PRINT_TIME();
            DEBUG_PRINTLN("RECEIVED");
            message.print();
    )

    if(message.getCode() == CODE_EMPTY)
        handlePing(message);
    else if(message.getCode() == CODE_GET &&
            message.getCode() == CODE_PUT)
        handleRequest(message);
    else
        handleBadRequest(message, CODE_BAD_REQUEST);
}

void CoAPHandler::handlePing(const CoAPMessage &message) {
    if (message.getT() == TYPE_ACK) {
        PendingMessage pendingMessage = finalizePendingMessage((unsigned short) (ping_messages_sent - 1));
        updateMetrics((unsigned short) (millis() - pendingMessage.timestamp));
    }
    else {
        CoAPMessage response;

        response.setCode(CODE_EMPTY);
        response.setT(TYPE_ACK);
        response.setMessageId(message.getMessageId());

        send(response);
    }
}

void CoAPHandler::handleRequest(const CoAPMessage &message) {
    CoAPMessage response;
    RadioMessage radioMessage;
    bool sendRadioMessage = false;
    OptionArray options = message.getOptions();
    CoAPOption* iterator = options.begin();
    int option_id = 0;

    for(; iterator != options.end(); iterator++) {
        option_id = iterator->getNumber();

        switch(option_id) {
            case OPTION_URI_PATH:
                {
                    Array<String> uri_path;
                    while (((iterator + 1) != options.end()) && ((iterator + 1)->getNumber() == OPTION_URI_PATH)) {
                        uri_path.pushBack(iterator->toString());
                        ++iterator;
                    }
                    uri_path.pushBack(iterator->toString());

                    Node* resource = resources_.search(uri_path);

                    if (resource != nullptr) {
                        if (uri_path[0] == RESOURCE_REMOTE) {
                            unsigned short resourceId = resource->value;
                            sendRadioMessage = true;
                            radioMessage.code = message.getCode();
                            radioMessage.message_id = message.getMessageId();
                            radioMessage.resource = resourceId;
                            addPendingMessage(message);
                        }
                        else if(message.getCode() == GET) {
                            if (uri_path[0] == RESOURCE_WELL_KNOWN) {
                                createResponse(message, response);
                                response.addOption(toContentFormat(CONTENT_LINK_FORMAT));
                                response.setPayload(toByteArray(resources_.toLinkFormat()));
                            }
                            else if (uri_path[0] == RESOURCE_LOCAL) {
                                if(resource->key == RESOURCE_JITTER) {
                                    createResponse(message, response);
                                    response.addOption(toContentFormat(CONTENT_TEXT_PLAIN));
                                    response.setPayload(toByteArray(TO_STRING(last_jitter)));
                                }
                                else if(resource->key == RESOURCE_RTT) {
                                    createResponse(message, response);
                                    response.addOption(toContentFormat(CONTENT_TEXT_PLAIN));
                                    response.setPayload(toByteArray(TO_STRING(mean_rtt)));
                                }
                                else if(resource->key == RESOURCE_TIMEOUT) {
                                    createResponse(message, response);
                                    response.addOption(toContentFormat(CONTENT_TEXT_PLAIN));
                                    response.setPayload(toByteArray(TO_STRING(timed_out)));
                                }
                            }
                        }
                        else {
                            handleBadRequest(message, CODE_BAD_REQUEST);
                        }
                    }
                }
                break;
            case OPTION_CONTENT_FORMAT:
            {
                if(message.getCode() == CODE_PUT) {
                    String option_value = iterator->toString();
                    if(option_value == TO_STRING(CONTENT_TEXT_PLAIN)) {
                        ByteArray payload = response.getPayload();
                        unsigned char* payload_iterator = payload.begin();
                        while(payload_iterator != payload.end()) {
                            ++payload_iterator;
                        }

                    } else {
                        //bad request
                    }
                } else {
                    //bad request
                }
            }
            case OPTION_BLOCK2:
                {
                    Block2 values(iterator->toBlock2());
                }
                break;
            default:
                //BAD REQUEST?
                //TODO: add more options
                break;
        }
    }

    if(sendRadioMessage)
        send(radioMessage);
    else
        send(response);
}

void CoAPHandler::handleMessage(RadioMessage &radioMessage) {
    DEBUG_FUNCTION(
            DEBUG_PRINT_TIME();
            DEBUG_PRINTLN("RECEIVED");
            radioMessage.print();
    );

    CoAPMessage message;
    message = finalizePendingMessage(radioMessage.message_id).coapMessage;

    CoAPMessage response;
    createResponse(message, response);
    response.addOption(toContentFormat(0));
    response.setPayload(toByteArray(TO_STRING(radioMessage.value)));
    send(response);

}

void CoAPHandler::createResponse(const CoAPMessage &message, CoAPMessage &response) {
    response.setToken(message.getToken());

    if (message.getT() == TYPE_CON) {
        response.setT(TYPE_ACK);
        response.setMessageId(message.getMessageId());
    } else if (message.getT() == TYPE_NON) {
        response.setT(TYPE_NON);
        response.setMessageId((unsigned short) (message.getMessageId() + 1));
    }

    if(message.getCode() == CODE_GET)
        response.setCode(69); //if get then code: 2.05-content
    else if(message.getCode() == CODE_PUT)
        response.setCode(68); //if put then code 2.04 -changed
}

void CoAPHandler::handleBadRequest(const CoAPMessage &message, unsigned short error_code) {
    CoAPMessage response;

    response.setToken(message.getToken());
    if (message.getT() == TYPE_CON) {
        response.setT(TYPE_ACK);
        response.setMessageId(message.getMessageId());
    } else {
        response.setT(TYPE_NON);
        response.setMessageId(message.getMessageId()+1);
    }
    response.setCode(error_code);

    send(response);
}


void CoAPHandler::send(const CoAPMessage &message) {
    DEBUG_FUNCTION(
            DEBUG_PRINT_TIME();
            DEBUG_PRINTLN("SENT");
            message.print();
    );

    if (coapMessageListener_ != nullptr)
        (*coapMessageListener_)(message);
}

void CoAPHandler::send(const RadioMessage &message) {
    DEBUG_FUNCTION(
            DEBUG_PRINT_TIME();
            DEBUG_PRINTLN("SENT");
            message.print();
    );

    if (radioMessageListener_ != nullptr)
        (*radioMessageListener_)(message);
}

void CoAPHandler::addPendingMessage(const CoAPMessage &message) {
    pending_messages_.pushBack({message, millis()});
    DEBUG_PRINT("Added pending message. Array size: ");
    DEBUG_PRINTLN(pending_messages_.size());
    DEBUG_PRINTLN("");
}

CoAPHandler::PendingMessage CoAPHandler::finalizePendingMessage(const unsigned short message_id) {
    for(unsigned int i = 0; i < pending_messages_.size(); ++i) {
        if(pending_messages_[i].coapMessage.getMessageId() == message_id)
            return pending_messages_.pop(i);
    }

    // TODO WHAT IF NOT FOUND?
}

void CoAPHandler::deleteTimedOut() {
    unsigned long now = millis();
    for(unsigned int i = 0; i < pending_messages_.size(); ++i) {
        if(now - pending_messages_[i].timestamp > timeout_) {
            DEBUG_FUNCTION(
                DEBUG_PRINT_TIME();
                DEBUG_PRINTLN("TIMEOUT");
                pending_messages_[i].coapMessage.print();

            );
            handleBadRequest(pending_messages_[i].coapMessage, CODE_GATEWAY_TIMEOUT);
            pending_messages_.erase(i);
            updateTimeoutMetric();
        }
    }
}

void CoAPHandler::registerResource(const Array<String> &uri_path, unsigned short value) {
    resources_.insert(uri_path, value);
}

void CoAPHandler::sendPing() {
    CoAPMessage message;
    message.setCode(CODE_EMPTY);
    message.setMessageId(ping_messages_sent++);
    send(message);
    addPendingMessage(message);
}

void CoAPHandler::updateMetrics(unsigned short rtt) {
    updateRoundTripTimeMetric(rtt);
    updateJitterMetric(rtt);
    DEBUG_PRINTLN("");
}

void CoAPHandler::updateRoundTripTimeMetric(unsigned short rtt) {
    if (mean_rtt == 0)
        mean_rtt = rtt;
    else {
        int tmp = (ping_messages_sent - 1) * mean_rtt;
        mean_rtt = (unsigned short) ((tmp + rtt) / ping_messages_sent);
    }

    DEBUG_PRINT("RTT: ");
    DEBUG_PRINTLN(mean_rtt);
}

void CoAPHandler::updateJitterMetric(unsigned short rtt) {
    last_jitter = rtt - mean_rtt;

    DEBUG_PRINT("Jitter: ");
    DEBUG_PRINTLN(last_jitter);
}

void CoAPHandler::updateTimeoutMetric() {
    ++timed_out;

    DEBUG_PRINT("Timed out messages: ");
    DEBUG_PRINTLN(timed_out);
    DEBUG_PRINTLN("");
}

unsigned short CoAPHandler::getTimeout() const {
    return timeout_;
}

ByteArray CoAPHandler::toByteArray(unsigned short value) {
    ByteArray result(2);
    result.pushBack((const unsigned char &) (value & 0xff));
    result.pushBack((const unsigned char &) ((value >> 8) & 0xff));
    return result;
}

CoAPOption CoAPHandler::toContentFormat(unsigned short value) {
    CoAPOption result(OPTION_CONTENT_FORMAT, toByteArray(value));
    return result;
}

ByteArray CoAPHandler::toByteArray(const String &value) {
    ByteArray result(value.length());
    for (int i = 0; i < value.length(); ++i) {
        result.pushBack((unsigned char) value[i]);
    }
    return result;
}

unsigned short CoAPHandler::toUnsignedShort(const String &value) {
    return (unsigned short) TO_INT(value);
}

String CoAPHandler::toString(const ByteArray &value) {
    String result;
    for(int i = 0; i < value.size(); ++i){
        result += char(value[i]);
    }
    return result;
}

