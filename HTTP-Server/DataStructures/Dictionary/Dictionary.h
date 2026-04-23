#ifndef Dictionary_h
#define Dictionary_h

#include "DataStructures/Lists/LinkedList.h"

#define DICTIONARY_CAPACITY 20

struct Dictionary {
    struct LinkedList buckets[DICTIONARY_CAPACITY];
};

struct Dictionary dictionary_constructor();
void dictionary_insert(struct Dictionary *dict, void *key, unsigned long key_size, void *value, unsigned long value_size);
void *dictionary_search(struct Dictionary *dict, void *key, unsigned long key_size);

#endif