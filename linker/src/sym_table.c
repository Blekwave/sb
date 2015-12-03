#include "sym_table.h"
#include "hash_aux.h"
#include <stdlib.h>
#include <string.h>

#define ST_BUCKETS 128

SymTable *stCreate(){
    SymTable *st = malloc(sizeof(*st));
    st->m = mapCreate(sizeof(int), ST_BUCKETS, (unsigned int(*)(void *))djb2,
                      (int(*)(void *, void *))strcmp);
    return st;
}

void stDestroy(SymTable *st){
    mapDestroy(st->m, free);
    free(st);
}

void stInsert(SymTable *st, char *label, int pos){
    mapInsert(st->m, label, strlen(label) + 1, &pos);
}

int stGet(SymTable *st, char *label){
    int out;
    return mapGet(st->m, label, &out) ? -1 : out;
}
