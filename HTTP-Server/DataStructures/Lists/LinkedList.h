#ifndef LinkedList_h
#define LinkedList_h

#include "../Common/Node.h"

struct LinkedList {
    struct Node *head;
    int length;
};

struct LinkedList linked_list_constructor();
void linked_list_insert(struct LinkedList *list, int index, void *data, unsigned long size);
void linked_list_remove(struct LinkedList *list, int index);
void *linked_list_retrieve(struct LinkedList *list, int index);

#endif