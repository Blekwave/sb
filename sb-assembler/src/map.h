#pragma once

#include "bucket.h"

typedef struct {
    Bucket **bs; // Buckets
    int len; // # of buckets
    size_t dsz; // Data size
    int (*hash)(void *key);
    int (*keyComp)(void *a, void *b);
} Map;

Map *mapCreate(size_t dsz, int len, int (*hash)(void *key), 
              int (*keyComp)(void *a, void *b));

void mapDestroy(Map *m, void (*freeData)(void *data));

void mapInsert(Map *m, void *key, size_t ksz, void *data);

int mapGet(Map *m, void *key, void *out);

int mapPop(Map *m, void *key, void *out, void (*freeData)(void *data));