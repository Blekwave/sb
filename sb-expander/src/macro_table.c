#include "hash_aux.h"
#include "line_parser.h"
#include "macro_table.h"
#include "map.h"
#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MT_LEN 256
#define MAX_LINE_LEN 4096

typedef struct {
    Map *m;
} MacroTable;

typedef struct {
    char *str;
    char *param;
    int calls;
    bool ready;
    Vector *labels;
} Macro;

static void macroDestroy(Macro *macro){
    free(macro->str);
    free(macro->param);
    if (macro->labels)
        vDestroy(macro->labels);
    free(macro);
}

MacroTable* mtCreate(){
    MacroTable *mt = malloc(sizeof(MacroTable));
    mt->m = mapCreate(sizeof(Macro), MT_LEN, ((unsigned int)(*)(void *))djb2,
                      ((int)(*)(void *, void *))strcmp);
    return mt;
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

static char *vectorToString(Vector *v, char *sep){
    // First pass: get length of the output string
    int len = 1; // \0 at the end
    int sep_len = strlen(sep);
    for (VIter it = vBegin(v); vIndex(it) != vLen(v); vNext(v, &it)){
        len += strlen(vData(it)) + sep_len;
    }

    // Second pass: actually make the string
    char *out = malloc(len * sizeof(char));
    char *cur_out = out;
    for (VIter it = vBegin(v); vIndex(it) != vLen(v); vNext(v, &it)){
        const char *it_str = vData(it);
        strcpy(cur_out, it_str);
        cur_out += strlen(it_str);
        strcpy(cur_out, sep);
        cur_out += sep_len;
    }

    return out;
}

void mtInsert(MacroTable *mt, char *name, char *macro, char *param){
    mapInsert(mt->m, name, strlen(name), &(Macro){
        .str = strCopyToNew(macro),
        .param = param ? strCopyToNew(param) : NULL,
        .calls = 0,
        .ready = false,
        .labels = NULL
    });
}

/**
 * Passes through every line in the macro looking for internal macro calls. If
 * found, the macro inside is evaluated and its evaluated body is added to the
 * original macro, replacing the call pseudo-instruction.
 * @param mt    MacroTable to which macro belongs
 * @param macro Macro to pre-process.
 */
static void macroPreProcess(MacroTable *mt, Macro *macro){
    Vector *v = vCreate(sizeof(char *), NULL);

    macro->labels = vCreate(sizeof(char *), free);

    char line_buffer[MAX_LINE_LEN];
    Line l;

    for (char *pch = strtok(macro->str, "\n"); pch; pch = strtok(NULL, "\n")){
        strcpy(line_buffer, pch);
        parseLine(line_buffer, &l);

        if (l.label){ // Saves all label definitions inside the macro
            vPush(macro->labels, strCopyToNew(l.label));
        }

        // Checks whether l.instr corresponds to a macro call
        // In case it does, it evaluates the internal call and appends it to the
        // current macro's line vector.
        char *call_body;
        int ret_val = mtEval(mt, l.instr, l.op1, &call_body);

        if (ret_val == 0){ // Instruction was a macro call!
            for (char *qch = strtok(call_body, "\n"); qch;
                 qch = strtok(NULL, "\n")){
                vPush(v, strCopyToNew(qch));
            }
        } else if (l.label || l.instr) { // Doesn't push empty lines
            // Label-only lines are okay, though (it's undefined behaviour which
            // I chose to support.)
            vPush(v, strCopyToNew(pch));
        }
    }

    free(macro->str);

    macro->str = vectorToString(v, "\n");

    // Macros only need to be pre-processed once.
    macro->ready = true;

    vDestroy(v);
}

int mtEval(MacroTable *mt, char *name, char *param, char **out){
    Macro macro;
    int get_status = mapGet(mt->m, name, &macro);
    if (get_status)
        return get_status;

    if (!macro.ready){ // Macros only need to be pre-processed once.
        macroPreProcess(mt, &macro);
        mapInsert(mt->m, name, strlen(name), &macro);
    }

    macroPerformReplacements(&macro, param, out);

    return 0;
}