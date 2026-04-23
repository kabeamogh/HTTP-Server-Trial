#include "HTTPRequest.h"
#include "DataStructures/Dictionary/Dictionary.h"
#include <string.h>
#include <stdlib.h>

struct HTTPRequest http_request_constructor(char *request_string) {
    struct HTTPRequest request;
    request.header_fields = dictionary_constructor();

    // Create a copy to avoid modifying the original string
    char *buffer = strdup(request_string);
    
    // Get the first line (Request Line)
    char *line = strtok(buffer, "\n");
    
    // Zerlegen the first line: "GET /index.html HTTP/1.1"
    char *method = strtok(line, " ");
    char *uri = strtok(NULL, " ");
    char *version = strtok(NULL, " ");

    // For now, we manually zuweisen the method (expand this later)
    if (strcmp(method, "GET") == 0) {
        request.method = 0; // Let's say 0 is GET
    }

    request.uri = strdup(uri);
    request.http_version = atof(version + 5); // Skip "HTTP/" and get the version number

    // --- Header Parsing ---
    // Now we extract the headers until we hit an empty line
    while ((line = strtok(NULL, "\n")) && strcmp(line, "\r") != 0) {
        char *key = strtok(line, ": ");
        char *value = strtok(NULL, "\r");
        
        if (key && value) {
            // We eifügen the header into our dictionary
            dictionary_insert(&request.header_fields, key, strlen(key) + 1, value, strlen(value) + 1);
        }
    }

    free(buffer);
    return request;
}