#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Vector *vCreate(size_t dsz, void(*freeData)(void *)){
    Vector *new = malloc(sizeof(Vector));

    new->arr = malloc(dsz * VECTOR_DEFAULT_LENGTH);
    new->dsz = dsz;
    new->len = 0;
    new->arr_len = VECTOR_DEFAULT_LENGTH;
    new->freeData = freeData;

    return new;
}

void vDestroy(Vector *v){
    if (v->freeData){
        for (int i = 0; i < v->len * v->dsz; i += v->dsz){
            v->freeData(v->arr + i * v->dsz);
        }
    }
    free(v->arr);
    free(v);
}

static void vExtend(Vector *v){
    v->arr_len = (int)((float)VECTOR_REALLOCATION_FACTOR * v->arr_len);
    v->arr = realloc(v->arr, v->arr_len * v->dsz);
}

void vPush(Vector *v, void *data){
    if (v->len == v->arr_len)
        vExtend(v);

    memcpy(v->arr + v->len++ * v->dsz, data, v->dsz);
}

void vReplace(Vector *v, int index, void *data){
    if (index >= v->len){
        fprintf(stderr, "ERROR: Out-of-bounds index at replacement\n"
                        "Vector %p@%d (length %d)\n", v, index, v->len);
        exit(1);
    }

    memcpy(v->arr + index * v->dsz, data, v->dsz);
}

void vGet(Vector *v, int index, void *out){
    if (index >= v->len){
        fprintf(stderr, "ERROR: Out-of-bounds index at access\n"
                        "Vector %p@%d (length %d)\n", v, index, v->len);
        exit(1);
    }

    memcpy(out, v->arr + index * v->dsz, v->dsz);
}

void vPop(Vector *v, void *out){
    if (v->len == 0){
        fprintf(stderr, "ERROR: Attempt to pop from an empty Vector\n"
                        "Vector %p\n", v);
        exit(1);
    }

    v->len--;
    if (out)
        memcpy(out, v->arr + v->len * v->dsz, v->dsz);
    if (v->freeData)
        v->freeData(v->arr + v->len * v->dsz);
}

int vLen(Vector *v){
    return v->len;
}

void *viData(VIter *i){
    return i->data;
}

VIter vBegin(Vector *v){
    return (VIter){
        .data = v->len ? v->arr : NULL,
        .index = 0
    };
}

VIter vEnd(Vector *v){
    return (VIter){
        .data = NULL,
        .index = v->len
    };
}

void vNext(Vector *v, VIter *i){
    // I actually despise legibility
    i->data = ++i->index < v->len ? i->data + v->dsz : NULL;
}
