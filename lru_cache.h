#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <stddef.h>

#define HASH_CAPACITY 128

typedef struct LRUNode {
    int doc_id;
    char *payload;
    struct LRUNode *prev;
    struct LRUNode *next;
} LRUNode;

typedef struct {
    size_t capacity;
    size_t count;
    LRUNode *head;
    LRUNode *tail;
    LRUNode *hash_table[HASH_CAPACITY];
} LRUCache;

#ifdef __cplusplus
extern "C" {
#endif

LRUCache* lru_create(size_t capacity);
void lru_free(LRUCache *cache);
void lru_put(LRUCache *cache, int doc_id, const char *payload);
const char* lru_get(LRUCache *cache, int doc_id);

#ifdef __cplusplus
}
#endif

#endif // LRU_CACHE_H
