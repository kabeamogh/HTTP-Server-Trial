#ifndef Files_h
#define Files_h

#include <stdio.h>

struct File {
    char *data;
    long size;
};

struct File read_file(char *path);

#endif