#include "Dictionary.h"
#include "Entry.h"
#include <string.h>

// --- Internal Helper Function ---
// Declaring it static restricts its scope to this file only.
static unsigned long hash_function(void *key, unsigned long key_size) {
    unsigned char *ptr = key;
    unsigned long hash = 0;
    for (unsigned long i = 0; i < key_size; i++) {
        hash += ptr[i];
    }
    return hash % DICTIONARY_CAPACITY;
}

struct Dictionary dictionary_constructor() {
    struct Dictionary dict;
    for (int i = 0; i < DICTIONARY_CAPACITY; i++) {
        dict.buckets[i] = linked_list_constructor();
    }
    return dict;
}

void dictionary_insert(struct Dictionary *dict, void *key, unsigned long key_size, void *value, unsigned long value_size) {
    unsigned long bucket = hash_function(key, key_size);
    struct Entry entry = entry_constructor(key, key_size, value, value_size);
    // We insert the entry into the list at the calculated bucket index
    linked_list_insert(&dict->buckets[bucket], dict->buckets[bucket].length, &entry, sizeof(struct Entry));
}

void *dictionary_search(struct Dictionary *dict, void *key, unsigned long key_size) {
    unsigned long bucket = hash_function(key, key_size);
    struct LinkedList *list = &dict->buckets[bucket];
    
    for (int i = 0; i < list->length; i++) {
        struct Entry *entry = (struct Entry *)linked_list_retrieve(list, i);
        // Check if the key matches
        if (memcmp(entry->key, key, key_size) == 0) {
            return entry->value;
        }
    }
    return NULL; // Return NULL if the key is not vorhanden
}