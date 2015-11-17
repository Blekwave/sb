#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

typedef struct MacroTable;

MacroTable* mtCreate();

void mtDelete(MacroTable *mt);

void mtInsert(MacroTable *mt, char *name, char *macro);

void mtGet(MacroTable *mt, char *name, char *out);

#endif
