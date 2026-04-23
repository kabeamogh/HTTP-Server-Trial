#include "HTTPServer.h"
#include "Systems/ThreadPool.h"
#include "Systems/Files.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// This is the worker function that threads will execute
void handle_request(void *arg) {
    int new_socket = *(int *)arg;
    free(arg); 

    char buffer[30000] = {0};
    read(new_socket, buffer, 30000);

    struct HTTPRequest request = http_request_constructor(buffer);

    char *file_to_read = (strcmp(request.uri, "/") == 0) ? "index.html" : request.uri + 1;

    // --- MAKE SURE THIS NAME MATCHES BELOW ---
    struct File html_file = read_file(file_to_read); 

    if (html_file.data) {
        char response_header[1024];
        // Use html_file.size here
        sprintf(response_header, 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %ld\r\n"
                "Connection: close\r\n\r\n", 
                html_file.size);
        
        write(new_socket, response_header, strlen(response_header));
        write(new_socket, html_file.data, html_file.size);
        
        free(html_file.data);
    } else {
        char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 22\r\n\r\n<h1>404 Not Found</h1>";
        write(new_socket, not_found, strlen(not_found));
    }

    free(request.uri);
    close(new_socket);
}

void launch(struct Server *server) {
    // Use a pointer now!
    struct ThreadPool *pool = thread_pool_constructor(10); 
    int address_length = sizeof(server->address);

    while (1) {
        int new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_length);
        int *socket_ptr = malloc(sizeof(int));
        *socket_ptr = new_socket;

        // Pass the pointer directly
        thread_pool_add_work(pool, handle_request, socket_ptr);
    }
}

struct HTTPServer http_server_constructor(int port) {
    struct HTTPServer server;
    // We pass our refined launch function as a function pointer
    server.base = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 10, launch);
    return server;
}