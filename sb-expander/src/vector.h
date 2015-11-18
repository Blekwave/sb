#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h> // size_t

#define VECTOR_DEFAULT_LENGTH 4
#define VECTOR_REALLOCATION_FACTOR 1.5

typedef struct {
    void *arr;
    size_t dsz;
    int len;
    int arr_len;
    void (*freeData)(void *);
} Vector;

typedef struct {
    void *data;
    int index;
} VIter;

Vector *vCreate(size_t dsz, void(*freeData)(void *));

void vDestroy(Vector *v);

void vPush(Vector *v, void *data);

void vReplace(Vector *v, int index, void *data);

void vGet(Vector *v, int index, void *out);

void vPop(Vector *v, void *out);

int vLen(Vector *v);

void *viData(VIter *i);

VIter vBegin(Vector *v);

VIter vEnd(Vector *v);

void vNext(Vector *v, VIter *i);

#endif
