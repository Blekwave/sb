#include "bucket.h"
#include <stdlib.h>
#include <string.h>  // memcpy

// Function documentation is available at the header file

BNode *bNodeCreate(void *key, size_t ksz, void *data, size_t dsz,
                     BNode *next){
    BNode *n = malloc(sizeof(BNode));
    n->key = malloc(ksz);
    memcpy(n->key, key, ksz);
    n->data = malloc(dsz);
    memcpy(n->data, data, dsz);
    n->next = next;
    return n;
}

void bNodeDestroy(BNode *n, void (*freeKey)(void *key),
                   void (*freeData)(void *data)){
    freeKey(n->key);
    freeData(n->data);
    free(n);
}

Bucket *bCreate(){
   Bucket *l = malloc(sizeof(Bucket));
    l->head = l->tail = bNodeCreate(NULL, 0, NULL, 0, NULL);
    return l;
}

void bDestroy(Bucket *b, void (*freeKey)(void *key),
               void (*freeData)(void *data)){
    BNode *next;
    for (BNode *it = b->head->next; it != NULL; it = next){
        next = it->next;
        bNodeDestroy(it, freeKey, freeData);
    }
    bNodeDestroy(b->head, free, free);
    free(b);
}

BNode *bFind(Bucket *b, void *key, int (*comp)(void *a, void *b)){
    BNode *cur = b->head->next;
    while (cur != NULL && comp(key, cur->key) != 0)
        cur = cur->next;
    return cur;
}

BNode *bPrevNode(Bucket *b, BNode *n){
    BNode *prev = b->head;
    while (prev != NULL && prev->next != n)
        prev = prev->next;
    return prev;
}
void bInsert(Bucket *b, void *key, size_t ksz, void *data, size_t dsz){
    BNode *new = bNodeCreate(key, ksz, data, dsz, NULL);
    b->tail = b->tail->next = new;
}

int bRemove(Bucket *b, BNode *n, void (*freeKey)(void *key),
              void (*freeData)(void *data)){
    BNode *prev = bPrevNode(b, n);
    if (prev == NULL){
        return 1;
    }
    prev->next = n->next;
    if ((b->tail = n))
        b->tail = prev;
    bNodeDestroy(n, freeKey, freeData);
    return 0;
}

BNode *bBegin(Bucket *b){
    return b->head->next;
}

BNode *bEnd(Bucket *b __attribute__((unused))){
    return NULL;
}

BNode *bNext(BNode *n){
    return n->next;
}