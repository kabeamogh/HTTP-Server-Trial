#include "Files.h"
#include <stdlib.h>

struct File read_file(char *path) {
    struct File file;
    FILE *f = fopen(path, "rb");
    
    if (!f) {
        perror("Failed to open file");
        file.data = NULL;
        file.size = 0;
        return file;
    }

    // Seek to the end to find the size (Größe)
    fseek(f, 0, SEEK_END);
    file.size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Allocate memory and read the Inhalt
    file.data = malloc(file.size + 1);
    fread(file.data, 1, file.size, f);
    file.data[file.size] = '\0'; // Ensure it's null-terminated

    fclose(f);
    return file;
}