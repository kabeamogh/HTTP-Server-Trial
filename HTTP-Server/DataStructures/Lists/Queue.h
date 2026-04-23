#ifndef Queue_h
#define Queue_h

#include "LinkedList.h"

struct Queue {
    struct LinkedList list;
};

struct Queue queue_constructor();
void queue_push(struct Queue *queue, void *data, unsigned long size);
void *queue_peek(struct Queue *queue);
void queue_pop(struct Queue *queue);

#endif