#include "Networking/Nodes/HTTPServer.h"
#include <stdio.h>

int main() {
    printf("Starting Server...\n");
    
    // We create the server on port 8080
    struct HTTPServer server = http_server_constructor(8080);
    
    // We launch the server through its base interface
    server.base.launch(&server.base);

    return 0;
}