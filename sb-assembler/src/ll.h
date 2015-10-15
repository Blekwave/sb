#pragma once

#include <stdlib.h>

typedef struct ll_node {
    void *key;
    void *data;
    struct ll_node *next;
} LLNode;

LLNode *llNodeCreate(void *key, size_t ksz, void *data, size_t dsz,
                     LLNode *next);

void llNodeDestroy(LLNode *n, void (*freeKey)(void *key),
                   void (*freeData)(void *data));

typedef struct {
    LLNode *head;
    LLNode *tail;
} LL;

LL *llCreate();

void llDestroy(LL *l, void (*freeKey)(void *key),
               void (*freeData)(void *data));

LLNode *llFind(LL *l, void *key, int (*comp)(void *a, void *b));

LLNode *llPrevNode(LL *l, LLNode *n);

void llInsert(LL *l, void *key, size_t ksz, void *data, size_t dsz);

int llRemove(LL *l, LLNode *n, void (*freeKey)(void *key),
              void (*freeData)(void *data));

LLNode *llBegin(LL *l);

LLNode *llEnd(LL *l);

LLNode *llNext(LLNode *n);
