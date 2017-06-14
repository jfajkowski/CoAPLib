#include "CoAPHandler.h"

/** Sets up CoAPHandler, binds callbacks used to process radio and internet input,
 * defines both local and remote resources.
 */
CoAPHandler::CoAPHandler(CoAPMessageListener &coapMessageListener, RadioMessageListener &radioMessageListener) :
        coapMessageListener_(&coapMessageListener),
        radioMessageListener_(&radioMessageListener),
        resources_() {
    prepareSpeakerResource();
    prepareLampResource();
    prepareRttResource();
    prepareJitterResource();
    prepareTimedOutResource();
}

/** Creates "TimedOut" resource **/
void CoAPHandler::prepareTimedOutResource() {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_LOCAL);
    uri_path.pushBack(RESOURCE_TIMED_OUT);
    resources_.insert(uri_path, nullptr);
}

/** Creates "Jitter" resource **/
void CoAPHandler::prepareJitterResource() {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_LOCAL);
    uri_path.pushBack(RESOURCE_JITTER);
    resources_.insert(uri_path, nullptr);
}

/** Creates "Rtt" resource **/
void CoAPHandler::prepareRttResource() {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_LOCAL);
    uri_path.pushBack(RESOURCE_RTT);
    resources_.insert(uri_path, nullptr);
}

/** Creates "Lamp" resource and maps it with resource number used in communication through radio**/
void CoAPHandler::prepareLampResource() {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_REMOTE);
    uri_path.pushBack(RESOURCE_LAMP);
    resources_.insert(uri_path, new unsigned short(RADIO_LAMP));
}

/** Creates "Speker" resource and maps it with resource number used in communication through radio**/
void CoAPHandler::prepareSpeakerResource() {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_REMOTE);
    uri_path.pushBack(RESOURCE_SPEAKER);
    resources_.insert(uri_path, new unsigned short(RADIO_SPEAKER));
}

/** Categorizes CoAP message to adequate category based on it's code (eg. GET, PUT) and calls suitable method **/
void CoAPHandler::handleMessage(CoAPMessage &message) {
    DEBUG_PRINT_TIME();
    DEBUG_PRINTLN("RECEIVED");
    DEBUG_FUNCTION(message.print());

    if(message.getCode() == CODE_EMPTY) {
        handlePing(message);
    }
    else if(message.getCode() == CODE_GET || message.getCode() == CODE_PUT) {
        handleRequest(message);
    }
    else {
        handleBadRequest(message, CODE_BAD_REQUEST);
    }
}

/** Responds with RST message if message is CoAP Ping. **/
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

/** Parses options and prepares radio or CoAP message with proper options **/
void CoAPHandler::handleRequest(const CoAPMessage &message) {
    CoAPMessage coapResponse;
    RadioMessage radioResponse;
    bool sendRadioMessage = false;
    CoAPOption* iterator = message.getOptions().begin();
    CoAPOption* end = message.getOptions().end();
    int option_id = 0;

    for(; iterator != end; iterator++) {
        option_id = iterator->getNumber();

        switch(option_id) {
            case OPTION_URI_PATH:
                {
                    Array<String> uri_path;
                    while (((iterator + 1) != end) && ((iterator + 1)->getNumber() == OPTION_URI_PATH)) {
                        uri_path.pushBack(iterator->toString());
                        ++iterator;
                    }
                    uri_path.pushBack(iterator->toString());

                    Node* resource = resources_.search(uri_path);

                    if (resource != nullptr) {
                        if (uri_path[0] == RESOURCE_REMOTE) {
                            unsigned short resourceId = *resource->getValue();
                            sendRadioMessage = true;
                            createResponse(message, radioResponse);
                            radioResponse.resource = resourceId;
                            addPendingMessage(message);
                        }
                        else if(message.getCode() == CODE_GET) {
                            if (uri_path[0] == RESOURCE_WELL_KNOWN) {
                                createResponse(message, coapResponse);
                                coapResponse.addOption(toContentFormat(CONTENT_LINK_FORMAT));
                                coapResponse.setPayload(toByteArray(RESOURCE_ALL1));
                            }
                            else if (uri_path[0] == RESOURCE_LOCAL) {
                                if(resource->getKey() == RESOURCE_JITTER) {
                                    createResponse(message, coapResponse);
                                    coapResponse.addOption(toContentFormat(CONTENT_TEXT_PLAIN));
                                    coapResponse.setPayload(toByteArray(TO_STRING(last_jitter)));
                                }
                                else if(resource->getKey() == RESOURCE_RTT) {
                                    createResponse(message, coapResponse);
                                    coapResponse.addOption(toContentFormat(CONTENT_TEXT_PLAIN));
                                    coapResponse.setPayload(toByteArray(TO_STRING(mean_rtt)));
                                }
                                else if(resource->getKey() == RESOURCE_TIMED_OUT) {
                                    createResponse(message, coapResponse);
                                    coapResponse.addOption(toContentFormat(CONTENT_TEXT_PLAIN));
                                    coapResponse.setPayload(toByteArray(TO_STRING(timed_out)));
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
                    String s_value = toString(iterator->getValue());
                    unsigned short content_format_type = toUnsignedShort(s_value);

                    if(content_format_type == CONTENT_TEXT_PLAIN) {
                        String s_payload = toString(message.getPayload());
                        unsigned short payload = toUnsignedShort(s_payload);
                        radioResponse.value = payload;
                    } else {
                        handleBadRequest(message, CODE_NOT_IMPLEMENTED);
                    }
                } else {
                    handleBadRequest(message, CODE_BAD_REQUEST);
                }
            }
            break;
            case OPTION_ACCEPT:
            {
                String s_value = toString(iterator->getValue());
                unsigned short content_format_type = toUnsignedShort(s_value);
                if(content_format_type == CONTENT_TEXT_PLAIN) {
                    // Message will have correct format
                } else {
                    // Message will be sent in text/plain format anyway
                }
            }
            break;
            case OPTION_BLOCK2:
                {
                    Block2 values(iterator->toBlock2());
                    values.m = 0;
                    CoAPOption option(values);
                    coapResponse.addOption(option);
                }
                break;
            default:
                handleBadRequest(message, CODE_BAD_REQUEST);
                break;
        }
    }

    if(sendRadioMessage)
        send(radioResponse);
    else
        send(coapResponse);
}

/** Handles RadioMessage, gets value from it and creates CoAP response **/
void CoAPHandler::handleMessage(RadioMessage &radioMessage) {
    DEBUG_PRINT_TIME();
    DEBUG_PRINTLN("RECEIVED");
    DEBUG_FUNCTION(radioMessage.print());

    CoAPMessage message;
    message = finalizePendingMessage(radioMessage.message_id).coapMessage;

    CoAPMessage response;
    createResponse(message, response);
    response.addOption(toContentFormat(0));
    response.setPayload(toByteArray(TO_STRING(radioMessage.value)));
    send(response);

}

/** Creates adequate CoAP response, based on received message TYPE **/
void CoAPHandler::createResponse(const CoAPMessage &message, CoAPMessage &response) {
    response.setToken(message.getToken());

    if (message.getT() == TYPE_CON) {
        response.setT(TYPE_ACK);
        response.setMessageId(message.getMessageId());
    } else if (message.getT() == TYPE_NON) {
        response.setT(TYPE_NON);
        response.setMessageId((unsigned short)(message.getMessageId() + 1));
    }

    if(message.getCode() == CODE_GET)
        response.setCode(69); //if get then code: 2.05-content
    else if(message.getCode() == CODE_PUT)
        response.setCode(68); //if put then code 2.04 -changed
}

/** Creates reponse based on radio response **/
void CoAPHandler::createResponse(const CoAPMessage &message, RadioMessage &response) {
    response.message_id = message.getMessageId();
    if (message.getCode() == CODE_GET) {
        response.code = RADIO_GET;
    }
    else if (message.getCode() == CODE_PUT) {
        response.code = RADIO_PUT;
    }
}
/** Prepares response with given error code and sends it to browser client**/
void CoAPHandler::handleBadRequest(const CoAPMessage &message, unsigned short error_code) {
    CoAPMessage response;

    response.setToken(message.getToken());
    if (message.getT() == TYPE_CON) {
        response.setT(TYPE_ACK);
        response.setMessageId(message.getMessageId());
    } else {
        response.setT(TYPE_NON);
        response.setMessageId((unsigned short)(message.getMessageId() + 1));
    }
    response.setCode(error_code);

    send(response);
}

/** This callback tells CoApServer.ino to send given CoAPMessage**/
void CoAPHandler::send(const CoAPMessage &message) {
    DEBUG_PRINT_TIME();
    DEBUG_PRINTLN("SENT");
    DEBUG_FUNCTION(message.print());

    if (coapMessageListener_ != nullptr)
        (*coapMessageListener_)(message);
}

/** This callback tells CoApServer.ino to send given RadioMessage**/
void CoAPHandler::send(const RadioMessage &message) {
    DEBUG_PRINT_TIME();
    DEBUG_PRINTLN("SENT");
    DEBUG_FUNCTION(message.print());

    if (radioMessageListener_ != nullptr)
        (*radioMessageListener_)(message);
}

/** Adds given message to list of pending request**/
void CoAPHandler::addPendingMessage(const CoAPMessage &message) {
    pending_messages_.pushBack({message, millis()});
    DEBUG_PRINT("Added pending message. Array size: ");
    DEBUG_PRINTLN(pending_messages_.size());
    DEBUG_PRINTLN("");
}
/** Removes from pending reqyest message with given id**/
CoAPHandler::PendingMessage CoAPHandler::finalizePendingMessage(const unsigned short message_id) {
    for(unsigned int i = 0; i < pending_messages_.size(); ++i) {
        if(pending_messages_[i].coapMessage.getMessageId() == message_id)
            return pending_messages_.pop(i);
    }

    // TODO WHAT IF NOT FOUND?
}

/** Deletes request that was not served and updates metric**/
void CoAPHandler::deleteTimedOut() {
    unsigned long now = millis();
    for(unsigned int i = 0; i < pending_messages_.size(); ++i) {
        if(now - pending_messages_[i].timestamp > timeout_) {
            DEBUG_PRINT_TIME();
            DEBUG_PRINTLN("TIMEOUT");
            DEBUG_FUNCTION(pending_messages_[i].coapMessage.print());

            handleBadRequest(pending_messages_[i].coapMessage, CODE_GATEWAY_TIMEOUT);
            pending_messages_.erase(i);
            updateTimeoutMetric();
        }
    }
}

/** Puts given path into resource tree, along with mapping into radio interface notation**/
void CoAPHandler::registerResource(const Array<String> &uri_path, unsigned short *value) {
    resources_.insert(uri_path, value);
}
/** Sends ping message to CoAP Client in order to calculate RTT
void CoAPHandler::sendPing() {
    CoAPMessage message;
    message.setCode(CODE_EMPTY);
    message.setMessageId(ping_messages_sent++);
    send(message);
    addPendingMessage(message);
}

/** Updates metrics describing internet connection with CoAP Client**/
void CoAPHandler::updateMetrics(unsigned short rtt) {
    updateRoundTripTimeMetric(rtt);
    updateJitterMetric(rtt);
    DEBUG_PRINTLN("");
}

/** Calculates RTT metric**/
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

/** Calculates Jitter metric**/
void CoAPHandler::updateJitterMetric(unsigned short rtt) {
    last_jitter = rtt - mean_rtt;

    DEBUG_PRINT("Jitter: ");
    DEBUG_PRINTLN(last_jitter);
}

/** Increments Timeout metric**/
void CoAPHandler::updateTimeoutMetric() {
    ++timed_out;

    DEBUG_PRINT("Timed out messages: ");
    DEBUG_PRINTLN(timed_out);
    DEBUG_PRINTLN("");
}

/** Returns Timeout metric**/
unsigned short CoAPHandler::getTimeout() const {
    return timeout_;
}

/** Converts unsigned short into ByteArray**/
ByteArray CoAPHandler::toByteArray(unsigned short value) {
    ByteArray result(1);
    if (value > 255) {
        result.pushBack((const unsigned char &) (value & 0xff));
        result.pushBack((const unsigned char &) ((value >> 8) & 0xff));
    }
    else {
        result.pushBack((const unsigned char &) value);
    }
    return result;
}

/**Creates ContetntFormat option with given id**/
CoAPOption CoAPHandler::toContentFormat(unsigned short value) {
    CoAPOption result(OPTION_CONTENT_FORMAT, toByteArray(value));
    return result;
}

/** Converts string into ByteArray**/
ByteArray CoAPHandler::toByteArray(const String &value) {
    ByteArray result(value.length());
    for (int i = 0; i < value.length(); ++i) {
        result.pushBack((unsigned char) value[i]);
    }
    return result;
}

/** Converts ByteArray into unsigned short **/
unsigned short CoAPHandler::toUnsignedShort(const String &value) {
    return (unsigned short) TO_INT(value);
}

/** Converts ByteArray into string**/
String CoAPHandler::toString(const ByteArray &value) {
    String result;
    for(int i = 0; i < value.size(); ++i){
        result += char(value[i]);
    }
    return result;
}
