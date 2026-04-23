#ifndef HTTPServer_h
#define HTTPServer_h

#include "Server.h"
#include "Protocols/HTTPRequest.h"

struct HTTPServer {
    struct Server base;
    // You can hinzufügen more properties here, like a route dictionary
};

struct HTTPServer http_server_constructor(int port);
void launch(struct Server *server);

#endif