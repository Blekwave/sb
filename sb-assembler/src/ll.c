#include "ll.h"
#include <stdlib.h>
#include <string.h>  // memcpy

LLNode *llNodeCreate(void *key, size_t ksz, void *data, size_t dsz,
                     LLNode *next){
    LLNode *n = malloc(sizeof(LLNode));
    n->key = malloc(ksz);
    memcpy(n->key, key, ksz);
    n->data = malloc(dsz);
    memcpy(n->data, data, dsz);
    n->next = next;
    return n;
}

void llNodeDestroy(LLNode *n, void (*freeKey)(void *key),
                   void (*freeData)(void *data)){
    freeKey(n->key);
    freeData(n->data);
    free(n);
}

LL *llCreate(){
    LL *l = malloc(sizeof(LL));
    l->head = l->tail = llNodeCreate(NULL, 0, NULL, 0, NULL);
    return l;
}

void llDestroy(LL *l, void (*freeKey)(void *key),
               void (*freeData)(void *data)){
    LLNode *next;
    for (LLNode *it = l->head->next; it != NULL; it = next){
        next = it->next;
        llNodeDestroy(it, freeKey, freeData);
    }
    llNodeDestroy(l->head, free, free);
    free(l);
}

LLNode *llFind(LL *l, void *key, int (*comp)(void *a, void *b)){
    LLNode *cur = l->head->next;
    while (cur != NULL && comp(key, cur->key) != 0)
        cur = cur->next;
    return cur;
}

LLNode *llPrevNode(LL *l, LLNode *n){
    LLNode *prev = l->head;
    while (prev != NULL && prev->next != n)
        prev = prev->next;
    return prev;
}
void llInsert(LL *l, void *key, size_t ksz, void *data, size_t dsz){
    LLNode *new = llNodeCreate(key, ksz, data, dsz, NULL);
    l->tail = l->tail->next = new;
}

int llRemove(LL *l, LLNode *n, void (*freeKey)(void *key),
              void (*freeData)(void *data)){
    LLNode *prev = llPrevNode(l, n);
    if (prev == NULL){
        return 1;
    }
    prev->next = n->next;
    if (l->tail = n)
        l->tail = prev;
    llNodeDestroy(n, freeKey, freeData);
    return 0;
}

LLNode *llBegin(LL *l){
    return l->head;
}

LLNode *llEnd(LL *l){
    return NULL;
}

LLNode *llNext(LLNode *n){
    return n->next;
}