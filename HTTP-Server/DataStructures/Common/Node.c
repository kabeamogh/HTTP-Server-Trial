#include "Node.h"
#include <string.h>

struct Node *node_constructor(void *data, unsigned long size) {
    struct Node *node = malloc(sizeof(struct Node));
    node->data = malloc(size);
    memcpy(node->data, data, size);
    node->next = NULL;
    return node;
}

void node_destructor(struct Node *node) {
    free(node->data);
    free(node);
}