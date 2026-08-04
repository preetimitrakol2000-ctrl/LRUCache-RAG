# LRUCache-RAG

A zero-allocation doubly linked list + hash table LRU cache in C for context window management.

## Compilation & Run Instructions

```bash
# Compile shared library
gcc -O3 -shared -fPIC -Iinclude src/lru_cache.c -o liblru.so

# Execute python test driver
python3 app/lru_rag_app.py
