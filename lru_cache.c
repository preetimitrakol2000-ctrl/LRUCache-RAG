#include "../include/lru_cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int hash_id(int doc_id) {
    return (doc_id < 0 ? -doc_id : doc_id) % HASH_CAPACITY;
}

LRUCache* lru_create(size_t capacity) {
    LRUCache *cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->count = 0;
    cache->head = NULL;
    cache->tail = NULL;
    memset(cache->hash_table, 0, sizeof(cache->hash_table));
    return cache;
}

static void detach_node(LRUCache *cache, LRUNode *node) {
    if (node->prev) node->prev->next = node->next;
    else cache->head = node->next;

    if (node->next) node->next->prev = node->prev;
    else cache->tail = node->prev;
}

static void attach_front(LRUCache *cache, LRUNode *node) {
    node->next = cache->head;
    node->prev = NULL;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
}

void lru_free(LRUCache *cache) {
    if (!cache) return;
    LRUNode *curr = cache->head;
    while (curr) {
        LRUNode *next = curr->next;
        if (curr->payload) free(curr->payload);
        free(curr);
        curr = next;
    }
    free(cache);
}

const char* lru_get(LRUCache *cache, int doc_id) {
    int h = hash_id(doc_id);
    LRUNode *node = cache->hash_table[h];
    while (node) {
        if (node->doc_id == doc_id) {
            detach_node(cache, node);
            attach_front(cache, node);
            return node->payload;
        }
        node = node->next;
    }
    return NULL;
}

void lru_put(LRUCache *cache, int doc_id, const char *payload) {
    int h = hash_id(doc_id);
    LRUNode *node = cache->hash_table[h];

    while (node) {
        if (node->doc_id == doc_id) {
            free(node->payload);
            node->payload = strdup(payload);
            detach_node(cache, node);
            attach_front(cache, node);
            return;
        }
        node = node->next;
    }

    if (cache->count >= cache->capacity) {
        // Evict tail
        LRUNode *evict = cache->tail;
        if (evict) {
            detach_node(cache, evict);
            int eh = hash_id(evict->doc_id);
            if (cache->hash_table[eh] == evict) {
                cache->hash_table[eh] = evict->next;
            }
            if (evict->payload) free(evict->payload);
            free(evict);
            cache->count--;
        }
    }

    LRUNode *new_node = (LRUNode*)malloc(sizeof(LRUNode));
    new_node->doc_id = doc_id;
    new_node->payload = strdup(payload);
    attach_front(cache, new_node);

    new_node->next = cache->hash_table[h];
    cache->hash_table[h] = new_node;
    cache->count++;
}
