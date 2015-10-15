#ifndef BUCKET_H
#define BUCKET_H

#include <stdlib.h> // size_t

typedef struct bucket_node {
    void *key;
    void *data;
    struct bucket_node *next;
} BNode;

BNode *bNodeCreate(void *key, size_t ksz, void *data, size_t dsz,
                     BNode *next);

void bNodeDestroy(BNode *n, void (*freeKey)(void *key),
                   void (*freeData)(void *data));

typedef struct {
    BNode *head;
    BNode *tail;
} Bucket;

Bucket *bCreate();

void bDestroy(Bucket *b, void (*freeKey)(void *key),
               void (*freeData)(void *data));

BNode *bFind(Bucket *b, void *key, int (*comp)(void *a, void *b));

BNode *bPrevNode(Bucket *b, BNode *n);

void bInsert(Bucket *b, void *key, size_t ksz, void *data, size_t dsz);

int bRemove(Bucket *b, BNode *n, void (*freeKey)(void *key),
              void (*freeData)(void *data));

BNode *bBegin(Bucket *b);

BNode *bEnd(Bucket *b);

BNode *bNext(BNode *n);

#endif