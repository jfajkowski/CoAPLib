#ifndef CODES_H
#define CODES_H

#define COAP_CODE(N) (((N)/100 << 5) | (N)%100)

static const unsigned short DEFAULT_VERSION = 0x01;
static const unsigned char PAYLOAD_MARKER = 0xFF;

// Message types:
static const unsigned short TYPE_CON = 0;
static const unsigned short TYPE_NON = 1;
static const unsigned short TYPE_ACK = 2;
static const unsigned short TYPE_RST = 3;

// Method codes:
static const unsigned short CODE_EMPTY = COAP_CODE(0);
static const unsigned short CODE_GET = COAP_CODE(1);
static const unsigned short CODE_POST = COAP_CODE(2);
static const unsigned short CODE_PUT = COAP_CODE(3);
static const unsigned short CODE_DELETE = COAP_CODE(4);

// Response codes:
static const unsigned short CODE_CREATED = COAP_CODE(201);
static const unsigned short CODE_DELETED = COAP_CODE(202);
static const unsigned short CODE_VALID = COAP_CODE(203);
static const unsigned short CODE_CHANGED = COAP_CODE(204);
static const unsigned short CODE_CONTENT = COAP_CODE(205);
static const unsigned short CODE_BAD_REQUEST = COAP_CODE(400);
static const unsigned short CODE_UNAUTHORIZED = COAP_CODE(401);
static const unsigned short CODE_BAD_OPTION = COAP_CODE(402);
static const unsigned short CODE_FORBIDDEN = COAP_CODE(403);
static const unsigned short CODE_NOT_FOUND = COAP_CODE(404);
static const unsigned short CODE_METHOD_NOT_ALLOWED = COAP_CODE(405);
static const unsigned short CODE_NOT_ACCEPTABLE = COAP_CODE(406);
static const unsigned short CODE_PRECONDITION_FAILED = COAP_CODE(412);
static const unsigned short CODE_REQUEST_ENTITY_TOO_LARGE = COAP_CODE(413);
static const unsigned short CODE_UNSUPPORTED_CONTENT_FORMAT = COAP_CODE(415);
static const unsigned short CODE_shortERNAL_SERVER_ERROR = COAP_CODE(500);
static const unsigned short CODE_NOT_IMPLEMENTED = COAP_CODE(501);
static const unsigned short CODE_BAD_GATEWAY = COAP_CODE(502);
static const unsigned short CODE_SERVICE_UNAVAILABLE = COAP_CODE(503);
static const unsigned short CODE_GATEWAY_TIMEOUT = COAP_CODE(504);
static const unsigned short CODE_PROXYING_NOT_SUPPORTED = COAP_CODE(505);


// Option codes:
static const unsigned short OPTION_URI_PATH = 11;
static const unsigned short OPTION_CONTENT_FORMAT = 12;
static const unsigned short OPTION_BLOCK2 = 23;

// Supported content formats:
static const unsigned short CONTENT_TEXT_PLAIN = 0;
static const unsigned short CONTENT_LINK_FORMAT = 40;

// Message header constants:
static const unsigned char MASK_VER = 0xC0;
static const unsigned char MASK_T = 0x30;
static const unsigned char MASK_TKL = 0x0F;
static const unsigned short MASK_MESSAGE_ID = 0x00FF;

static const unsigned char OFFSET_VER = 6;
static const unsigned char OFFSET_T = 4;
static const unsigned char OFFSET_MESSAGE_ID = 8;

// Option header constants:
static const unsigned char MASK_DELTA = 0xF0;
static const unsigned char MASK_LENGTH = 0x0F;
static const unsigned char MASK_EXTENDABLE = 0x00FF;

static const unsigned char OFFSET_DELTA = 4;
static const unsigned char OFFSET_EXTENDABLE = 8;

// Resource types constants:
static const String RESOURCE_WELL_KNOWN = ".well-known";
static const String RESOURCE_CORE = "core";
static const String RESOURCE_LOCAL = "local";
static const String RESOURCE_REMOTE = "remote";
static const String RESOURCE_RTT = "rtt";
static const String RESOURCE_TIMED_OUT = "timed_out";
static const String RESOURCE_JITTER = "jitter";
static const String RESOURCE_SPEAKER = "speaker";
static const String RESOURCE_LAMP = "lamp";

#endif //CODES_H
