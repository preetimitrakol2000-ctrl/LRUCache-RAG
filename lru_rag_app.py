from bindings.lru_bridge import ContextLRUCache

def main():
    # Cache with capacity of 2 contexts
    cache = ContextLRUCache(capacity=2)

    cache.put(101, "Context 101: GPU acceleration details")
    cache.put(102, "Context 102: Transformer attention layer breakdown")

    print("=== LRUCACHE-RAG O(1) CONTEXT EVICTION ===")
    print(f"[*] Fetch 101: {cache.get(101)}")

    # Insert 103 -> causes eviction of 102 (since 101 was accessed recently)
    cache.put(103, "Context 103: Dynamic Memory Allocation in C")

    print(f"[*] Fetch 102 (Should be evicted): {cache.get(102)}")
    print(f"[*] Fetch 103: {cache.get(103)}")

if __name__ == "__main__":
    main()
        
