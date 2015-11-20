#include "expander.h"
#include "macro_table.h"
#include "line_parser.h"
#include "vector.h"
#include "str_aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LEN 4096
#define MAX_MACRO_NAME_LEN 256
#define MAX_MACRO_PARAM_LEN 256

const char begin_macro[] = "BEGINMACRO";
const char end_macro[] = "ENDMACRO";

static MacroTable *buildMacroTable(FILE *in){
    MacroTable *mt = mtCreate();

    Line l;
    char line_buffer[MAX_LINE_LEN];
    // parseLine mangles lines' strings, so another buffer is necessary to keep
    // the original line intact.
    char mangled_buffer[MAX_LINE_LEN];

    bool inside_macro = false;
    char macro_name[MAX_MACRO_NAME_LEN];
    char macro_param[MAX_MACRO_PARAM_LEN];
    bool takes_param = false;
    Vector *current_macro;

    while (fgets(line_buffer, MAX_LINE_LEN - 1, in)){
        strcpy(mangled_buffer, line_buffer);
        parseLine(mangled_buffer, &l);
        if (inside_macro){
            if (strcmp(l.instr, end_macro) == 0){ // ENDMACRO
                mtInsert(mt, macro_name, current_macro,
                         takes_param ? macro_param : NULL);
                inside_macro = false;
                current_macro = NULL; // just in case
            } else {
                char *line_copy = strCopyToNew(line_buffer);
                vPush(current_macro, &line_buffer);
            }
        } else {
            if (strcmp(l.instr, begin_macro) == 0){ // BEGINMACRO
                inside_macro = true;
                if (!l.label){
                    fprintf(stderr, "ERROR: Nameless macro definition.\n");
                    exit(1);
                }
                strcpy(macro_name, l.label);
                if (l.op1){
                    strcpy(macro_name, l.op1);
                    takes_param = true;
                } else {
                    takes_param = false;
                }
                current_macro = vCreate(sizeof(char *), free);
            }
        }
    }

    return mt;
}

static void replaceAndOutput(FILE *in, FILE *out, MacroTable *mt){
    Line l;
    char line_buffer[MAX_LINE_LEN];
    // parseLine mangles lines' strings, so another buffer is necessary to keep
    // the original line intact.
    char mangled_buffer[MAX_LINE_LEN];

    bool inside_macro = false;

    while (fgets(line_buffer, MAX_LINE_LEN - 1, in)){
        strcpy(mangled_buffer, line_buffer);
        parseLine(mangled_buffer, &l);

        if (inside_macro){
            if (strcmp(l.instr, end_macro) == 0){ // ENDMACRO
                inside_macro = false;
            }
        } else {
            if (strcmp(l.instr, begin_macro) == 0){ // BEGINMACRO
                inside_macro = true;
            } else {
                char *macro_out;
                int ret_val = 1;
                if (l.instr){
                    ret_val = mtEval(mt, l.instr, l.op1, &macro_out);
                }
                if (!ret_val){ // ret_val == 0 -> Instruction is a macro call
                    fprintf(out, "%s\n", macro_out);
                    free(macro_out);
                } else { // Just a regular instruction.
                    fprintf(out, "%s\n", line_buffer);
                }
            }
        }

    }
}

int expExpand(const char *src_addr, const char *dest_addr){
    FILE *in = fopen(src_addr, "r");

    MacroTable *mt = buildMacroTable(in);

    rewind(in);

    FILE *out = fopen(dest_addr, "w");

    replaceAndOutput(in, out, mt);

    fclose(out);

    mtDestroy(mt);

    fclose(in);
}
