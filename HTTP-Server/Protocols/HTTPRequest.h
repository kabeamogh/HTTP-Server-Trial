#ifndef HTTPRequest_h
#define HTTPRequest_h

#include "DataStructures/Dictionary/Dictionary.h"

struct HTTPRequest {
    int method; // We can use an enum for GET, POST, etc.
    char *uri;
    float http_version;
    struct Dictionary header_fields;
};

struct HTTPRequest http_request_constructor(char *request_string);

#endif