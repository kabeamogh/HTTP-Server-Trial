#ifndef Node_h
#define Node_h

#include <stdlib.h>

struct Node {
    void *data;
    struct Node *next;
};

struct Node *node_constructor(void *data, unsigned long size);
void node_destructor(struct Node *node);

#endif