#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include "map.h"

typedef struct {
    Map *m;
} SymTable;

SymTable *stCreate();

void stDestroy(SymTable *st);

void stInsert(SymTable *st, char *label, int pos);

int stGet(SymTable *st, char *label);

#endif
