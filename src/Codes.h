#ifndef ARDUINODEMO_CODES_H
#define ARDUINODEMO_CODES_H

// Method codes:
static const unsigned int CODE_GET = 1;
static const unsigned int CODE_POST = 2;
static const unsigned int CODE_PUT = 3;
static const unsigned int CODE_DELETE = 4;

// Response codes:
static const unsigned int CODE_CREATED = 201;
static const unsigned int CODE_DELETED = 202;
static const unsigned int CODE_VALID = 203;
static const unsigned int CODE_CHANGED = 204;
static const unsigned int CODE_CONTENT = 205;
static const unsigned int CODE_BAD_REQUEST = 400;
static const unsigned int CODE_UNAUTHORIZED = 401;
static const unsigned int CODE_BAD_OPTION = 402;
static const unsigned int CODE_FORBIDDEN = 403;
static const unsigned int CODE_NOT_FOUND = 404;
static const unsigned int CODE_METHOD_NOT_ALLOWED = 405;
static const unsigned int CODE_NOT_ACCEPTABLE = 406;
static const unsigned int CODE_PRECONDITION_FAILED = 412;
static const unsigned int CODE_REQUEST_ENTITY_TOO_LARGE = 413;
static const unsigned int CODE_UNSUPPORTED_CONTENT_FORMAT = 415;
static const unsigned int CODE_INTERNAL_SERVER_ERROR = 500;
static const unsigned int CODE_NOT_IMPLEMENTED = 501;
static const unsigned int CODE_BAD_GATEWAY = 502;
static const unsigned int CODE_SERVICE_UNAVAILABLE = 503;
static const unsigned int CODE_GATEWAY_TIMEOUT = 504;
static const unsigned int CODE_PROXYING_NOT_SUPPORTED = 505;

#endif //ARDUINODEMO_CODES_H
