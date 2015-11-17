#include "macro_table.h"
#include "map.h"
#include "hash_aux.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MT_LEN 256

typedef struct {
    Map *m;
} MacroTable;

typedef struct {
    char *str;
    char *param;
    int calls;
    bool ready;
} Macro;

MacroTable* mtCreate(){
    MacroTable *mt = malloc(sizeof(MacroTable));
    mt->m = mapCreate(sizeof(Macro), MT_LEN, ((unsigned int)(*)(void *))djb2,
                      ((int)(*)(void *, void *))strcmp);
    return mt;
}

static void macroDestroy(Macro *macro){
    free(macro->str);
    free(macro->param);
    free(macro);
}

void mtDestroy(MacroTable *mt){
    mapDestroy(mt->m, macroDestroy);
    free(mt);
}

static char *strCopyToNew(char *str){
    char *new = malloc(strlen(str) + 1);
    strcpy(new, str);
    return new;
}

void mtInsert(MacroTable *mt, char *name, char *macro, char *param){
    mapInsert(mt->m, name, strlen(name), &(Macro){
        .str = strCopyToNew(macro),
        .param = param ? strCopyToNew(param) : NULL,
        .calls = 0,
        .ready = false
    });
}

// static void macroPreProcess(Macro *macro){
    
// }

int mtEval(MacroTable *mt, char *name, char *param, char *out){
    Macro macro;
    int get_status = mapGet(mt->m, name, &macro);
    if (get_status)
        return get_status;

    if (!macro.ready){
        macroPreProcess(&macro);
        mapInsert(mt->m, name, strlen(name), &macro);
    }

    macroSubParamAndPrint(&macro, param, out);

    return 0;
}