#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

typedef struct MacroTable;

MacroTable* mtCreate();

void mtDestroy(MacroTable *mt);

void mtInsert(MacroTable *mt, char *name, char *macro, char *param);

int mtEval(MacroTable *mt, char *name, char *param, char **out);

#endif
