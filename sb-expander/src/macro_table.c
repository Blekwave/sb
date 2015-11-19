#include "hash_aux.h"
#include "line_parser.h"
#include "macro_table.h"
#include "map.h"
#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> // sprintf

#define MT_LEN 256
#define MAX_LINE_LEN 4096
#define MACRO_LABEL_MAP_LEN 8
#define LABEL_SUFFIX_LEN 17

typedef struct {
    char *str;
    Vector *vector;
    char *param;
    int calls;
    bool ready;
    Map *labels;
} Macro;

static void macroDestroy(Macro *macro){
    if (macro->str)
        free(macro->str);
    if (macro->vector)
        vDestroy(macro->vector);
    free(macro->param);
    if (macro->labels)
        mapDestroy(macro->labels, free);
    free(macro);
}

MacroTable* mtCreate(){
    MacroTable *mt = malloc(sizeof(MacroTable));
    mt->m = mapCreate(sizeof(Macro), MT_LEN, (unsigned int(*)(void *))djb2,
                      (int(*)(void *, void *))strcmp);
    return mt;
}

void mtDestroy(MacroTable *mt){
    mapDestroy(mt->m, (void(*)(void *))macroDestroy);
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
        .vector = NULL,
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
    Vector *v = vCreate(sizeof(char *), free);

    macro->labels = mapCreate(0, MACRO_LABEL_MAP_LEN,
                              (unsigned int(*)(void *))djb2,
                              (int(*)(void *, void *))strcmp);

    char line_buffer[MAX_LINE_LEN];
    Line l;

    for (char *pch = strtok(macro->str, "\n"); pch; pch = strtok(NULL, "\n")){
        strcpy(line_buffer, pch);
        parseLine(line_buffer, &l);

        if (l.label){ // Saves all label definitions inside the macro
            mapInsert(macro->labels, l.label, strlen(l.label), NULL);
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
            free(call_body);
        } else if (l.label || l.instr) { // Doesn't push empty lines
            // Label-only lines are okay, though (it's undefined behaviour which
            // I chose to support.)
            vPush(v, strCopyToNew(pch));
        }
    }

    free(macro->str);
    macro->str = NULL;

    macro->vector = v;

    // Macros only need to be pre-processed once.
    macro->ready = true;
}

/**
 * Iterates through a Macro's pre-processed line vector in order to compute the
 * length of its string form, taking into account label suffixing and parameter
 * substitution.
 * @param  v          Macro's line vector
 * @param  labels     Macro's label map
 * @param  orig_param macro->param
 * @param  new_param  The value the parameter should assume (or NULL)
 * @return            Length of the string representation.
 */
static int macroVectorLen(Vector *v, Map *labels, char *orig_param,
                          char *new_param){
    Line l;
    char line_buffer[MAX_LINE_LEN];

    int param_len = new_param ? strlen(new_param) : 0;

    int out_len = 1; // \0 at the end

    for (VIter it = vBegin(v); viIndex(&it) != vLen(v); vNext(v, &it)){
        strcpy(line_buffer, viData(&it));
        parseLine(line_buffer, &l);

        if (l.label){
            // Checks if the label corresponds to the parameter
            if (orig_param && strcmp(l.label, orig_param) == 0){ // It does
                out_len += param_len + 2; // +2: colon and space after label
                if (mapGet(labels, new_param, NULL) == 0)
                    out_len += LABEL_SUFFIX_LEN;
            } else { // Nope, just a regular label
                out_len += strlen(l.label) + 2;
                if (mapGet(labels, l.label, NULL) == 0)
                    out_len += LABEL_SUFFIX_LEN;
            }
        }

        if (l.instr){
            if (orig_param && strcmp(l.instr, orig_param) == 0){
                out_len += param_len + 1; // +1: Space or line break
            } else {
                out_len += strlen(l.instr) + 1;
            }
        }

        if (l.op1){
            if (orig_param && strcmp(l.op1, orig_param) == 0){
                out_len += param_len + 1;
            } else {
                out_len += strlen(l.op1) + 1;
            }

            // There can't be a second operand without a first one
            if (l.op2){
                if (orig_param && strcmp(l.op2, orig_param) == 0){
                    out_len += param_len + 1;
                } else {
                    out_len += strlen(l.op2) + 1;
                }
            }
        }
    }

    return out_len;
}

/**
 * Outputs a string corresponding to the Macro, substituting its parameter pla-
 * ceholders by the value they should assume, as well as placing suffixes in the
 * labels which were declared inside the macro to avoid conflict.
 * @param macro Macro to be output
 * @param param Value to be assumed by its parameter (if there's a parameter,
 *              otherwise anything goes, including NULL)
 * @param out   Pointer to a string, where the new string's address should be
 *              saved (and freed afterwards!)
 */
static void macroReplace(Macro *macro, char *param, char **out){

    // Label suffixes are necessary because labels can be declared inside macro
    // calls. If a macro were to be instantiated more than once, then, there
    // would be conflict about which address the label points to. Suffixes solve
    // the issue by simply appending a unique value to each call to the macro's
    // output. The unique value is defined by the call's ID, saved inside the
    // macro struct and incremented each time.
    char label_suffix[LABEL_SUFFIX_LEN + 1]; // \0
    sprintf(label_suffix, "__MACRO%010d", macro->calls++);

    // Aliases for readability
    Vector *v = macro->vector;
    Map *labels = macro->labels;

    // Computes the output string's length.
    int out_len = macroVectorLen(v, labels, macro->param, param);

    int param_len = param ? strlen(param) : 0;

    char *out_str = malloc(out_len * sizeof(char));

    // out_str's original address is saved beforehands because out_str is going
    // to be manipulated to print the macro.
    *out = out_str;

    Line l;
    char line_buffer[MAX_LINE_LEN];

    for (VIter it = vBegin(v); viIndex(&it) != vLen(v); vNext(v, &it)){
        strcpy(line_buffer, viData(&it));
        parseLine(line_buffer, &l);

        if (l.label){
            if (macro->param && strcmp(l.label, macro->param) == 0){
                strcpy(out_str, param);
                out_str += param_len;
                if (mapGet(labels, param, NULL) == 0){
                    strcpy(out_str, label_suffix);
                    out_str += LABEL_SUFFIX_LEN;
                }
            } else {
                strcpy(out_str, l.label);
                out_str += strlen(l.label);
                if (mapGet(labels, l.label, NULL) == 0){
                    strcpy(out_str, label_suffix);
                    out_str += LABEL_SUFFIX_LEN;
                }
            }
            *out_str++ = ':';
        }

        if (l.instr){
            if (l.label){ // If a label came before the instruction, there must
                          // be a space before it. It didn't get placed inside
                          // the label section because lines with lone labels
                          // are supported (extra-officially).
                *out_str++ = ' ';
            }

            if (macro->param && strcmp(l.instr, macro->param) == 0){
                strcpy(out_str, param);
                out_str += param_len;
            } else {
                strcpy(out_str, l.instr);
                out_str += strlen(l.instr);
            }
        }

        if (l.op1){
            *out_str++ = ' ';

            if (macro->param && strcmp(l.op1, macro->param) == 0){
                strcpy(out_str, param);
                out_str += param_len;
            } else {
                strcpy(out_str, l.op1);
                out_str += strlen(l.op1);
            }

            // There can't be a second operand without a first one
            if (l.op2){
                *out_str++ = ' ';

                if (macro->param && strcmp(l.op2, macro->param) == 0){
                    strcpy(out_str, param);
                    out_str += param_len;
                } else {
                    strcpy(out_str, l.op2);
                    out_str += strlen(l.op2);
                }

            }
        }

        *out_str++ = '\n';
    }

    *out_str = '\0';
}

int mtEval(MacroTable *mt, char *name, char *param, char **out){
    Macro macro;
    int get_status = mapGet(mt->m, name, &macro);
    if (get_status){
        *out = NULL;
        return get_status;
    }

    if (!macro.ready){ // Macros only need to be pre-processed once.
        macroPreProcess(mt, &macro);
    }

    macroReplace(&macro, param, out);

    // Macro is overwritten regardless of whether it was preprocessed at this
    // call or not: its number of calls must be incremented either way.
    mapInsert(mt->m, name, strlen(name), &macro);

    return 0;
}