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

    // Determine which file to open
    char *file_to_read = (strcmp(request.uri, "/") == 0) ? "index.html" : request.uri + 1;

    // --- ADDED THIS LINE TO DEBUG PATHS ---
    printf("Thread %ld is attempting to load: %s\n", (long)pthread_self(), file_to_read);

    struct File html_file = read_file(file_to_read); 

    if (html_file.data) {
        char response_header[1024];
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
        // --- ADDED LOGGING FOR 404 ERRORS ---
        printf("File not found: %s\n", file_to_read);
        char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 22\r\n\r\n<h1>404 Not Found</h1>";
        write(new_socket, not_found, strlen(not_found));
    }

    free(request.uri);
    close(new_socket);
}

void launch(struct Server *server) {
    // We create the pool once. It is leistungfähig to have it exist for the lifetime of the server.
    struct ThreadPool *pool = thread_pool_constructor(10); 
    int address_length = sizeof(server->address);

    printf("Waiting for connection...\n");

    while (1) {
        int new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_length);
        
        // If accept fails, we continue rather than crashing the whole server
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        int *socket_ptr = malloc(sizeof(int));
        *socket_ptr = new_socket;

        thread_pool_add_work(pool, handle_request, socket_ptr);
    }
}

struct HTTPServer http_server_constructor(int port) {
    struct HTTPServer h_server; 
    
    h_server.base = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 10, launch);
    
    int opt = 1;
    // This allows the server to restart immediately without "Address already in use" errors
    if (setsockopt(h_server.base.socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
    }
    
    return h_server;
}