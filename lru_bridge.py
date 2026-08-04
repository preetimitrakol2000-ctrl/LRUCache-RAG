import ctypes
import os
import sys

lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../liblru.so"))
if sys.platform.startswith("win"):
    lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../liblru.dll"))

class LRUCacheStruct(ctypes.Structure):
    pass

lru_lib = ctypes.CDLL(lib_path)

lru_lib.lru_create.argtypes = [ctypes.c_size_t]
lru_lib.lru_create.restype = ctypes.POINTER(LRUCacheStruct)

lru_lib.lru_free.argtypes = [ctypes.POINTER(LRUCacheStruct)]
lru_lib.lru_free.restype = None

lru_lib.lru_put.argtypes = [ctypes.POINTER(LRUCacheStruct), ctypes.c_int, ctypes.c_char_p]
lru_lib.lru_put.restype = None

lru_lib.lru_get.argtypes = [ctypes.POINTER(LRUCacheStruct), ctypes.c_int]
lru_lib.lru_get.restype = ctypes.c_char_p

class ContextLRUCache:
    def __init__(self, capacity: int = 2):
        self.cache = lru_lib.lru_create(capacity)

    def __del__(self):
        if hasattr(self, 'cache') and self.cache:
            lru_lib.lru_free(self.cache)

    def put(self, doc_id: int, payload: str):
        lru_lib.lru_put(self.cache, doc_id, payload.encode('utf-8'))

    def get(self, doc_id: int) -> str:
        res = lru_lib.lru_get(self.cache, doc_id)
        return res.decode('utf-8') if res else None
