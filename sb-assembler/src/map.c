#include "map.h"
#include <stdlib.h>
#include <string.h>

Map *mapCreate(size_t dsz, int len, int (*hash)(void *key), 
              int (*keyComp)(void *a, void *b)){
    Map *m = malloc(sizeof(Map));
    m->dsz = dsz;

    m->bs = malloc(sizeof(LL *) * len);
    for (int i = 0; i < len; i++){
        m->bs[i] = llCreate();
    }

    m->hash = hash;
    m->keyComp = keyComp;

    return m; 
}

void mapDestroy(Map *m, void (*freeData)(void *data)){
    for (int i = 0; i < m->len; i++){
        llDestroy(m->bs[i], free, freeData);
    }
    free(m->bs);
    free(m);
}

void mapInsert(Map *m, void *key, size_t ksz, void *data){
    LL *b = m->bs[m->hash(key)]; // Bucket
    LLNode *n = llFind(b, key, m->keyComp);
    if (n != NULL){
        memcpy(n->data, data, m->dsz);
    } else {
        llInsert(b, key, ksz, data, m->dsz);
    }
}

int mapGet(Map *m, void *key, void *out){
    LL *b = m->bs[m->hash(key)]; // Bucket
    LLNode *n = llFind(b, key, m->keyComp);
    if (n != NULL){
        memcpy(out, n->data, m->dsz);
        return 0;
    }
    return 1; // No such key
}

int mapPop(Map *m, void *key, void *out, void (*freeData)(void *data)){
    LL *b = m->bs[m->hash(key)]; // Bucket
    LLNode *n = llFind(b, key, m->keyComp);
    if (n != NULL){
        memcpy(out, n->data, m->dsz);
        llRemove(b, n, free, freeData);
        return 0;
    }
    return 1; // No such key
}