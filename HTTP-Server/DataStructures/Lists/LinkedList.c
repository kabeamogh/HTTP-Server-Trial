#include "LinkedList.h"
#include <stdlib.h>
#include <string.h>

struct LinkedList linked_list_constructor() {
    struct LinkedList list;
    list.head = NULL;
    list.length = 0;
    return list;
}

// Internal helper to create a node for the list
struct Node *create_node_ll(void *data, unsigned long size) {
    return node_constructor(data, size);
}

void linked_list_insert(struct LinkedList *list, int index, void *data, unsigned long size) {
    struct Node *node_to_insert = create_node_ll(data, size);
    if (index == 0) {
        node_to_insert->next = list->head;
        list->head = node_to_insert;
    } else {
        struct Node *cursor = list->head;
        for (int i = 0; i < index - 1; i++) {
            cursor = cursor->next;
        }
        node_to_insert->next = cursor->next;
        cursor->next = node_to_insert;
    }
    list->length++;
}

void linked_list_remove(struct LinkedList *list, int index) {
    if (index < 0 || index >= list->length) return;
    
    struct Node *temp;
    if (index == 0) {
        temp = list->head;
        list->head = list->head->next;
    } else {
        struct Node *cursor = list->head;
        for (int i = 0; i < index - 1; i++) {
            cursor = cursor->next;
        }
        temp = cursor->next;
        cursor->next = temp->next;
    }
    node_destructor(temp);
    list->length--;
}

void *linked_list_retrieve(struct LinkedList *list, int index) {
    struct Node *cursor = list->head;
    for (int i = 0; i < index; i++) {
        cursor = cursor->next;
    }
    return cursor->data;
}