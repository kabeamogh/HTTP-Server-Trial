#include "Queue.h"

struct Queue queue_constructor() {
    struct Queue queue;
    queue.list = linked_list_constructor();
    return queue;
}

void queue_push(struct Queue *queue, void *data, unsigned long size) {
    linked_list_insert(&queue->list, queue->list.length, data, size);
}

void queue_pop(struct Queue *queue) {
    linked_list_remove(&queue->list, 0);
}

void *queue_peek(struct Queue *queue) {
    return linked_list_retrieve(&queue->list, 0);
}