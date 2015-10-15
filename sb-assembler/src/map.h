typedef struct {
    BST *data;
    int len; // # of buckets
    size_t dsz; // Data size
    size_t ksz; // Key size
    int (*hash)(void *key);
} Map;

Map mapCreate(size_t ksz, size_t dsz, int (*hash)(void *key), int len);

void mapDelete(Map *m, void (*freeData)(void *data));

void mapInsert(Map *m, void *key, void *data);

void mapGet(Map *m, void *key, void *out);

void mapPop(Map *m, void *key, void *out);