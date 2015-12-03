#include "linker.h"
#include "sym_table.h"
#include "vector.h"
#include "str_aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 4096

char begin_sym_table[] = "BEGINSM";
char end_sym_table[] = "ENDSM";

int gatherFromFile(char *addr, Vector *instrs, SymTable *st, int ilc){
    char buf[MAX_LINE_LEN];
    FILE *in = fopen(addr, "r");
    // Reads the input file's symbol table and adds the symbols to st
    fgetsNoNewline(buf, MAX_LINE_LEN - 1, in);

    // Sanity check: makes sure the program is receiving the modified assem-
    // bled file, with the symbol table at the beginning
    if (strcmp(buf, begin_sym_table) != 0){
        fprintf(stderr, "ERROR: Input file %s has no symbol table.", addr);
        exit(1);
    }
    while (fgetsNoNewline(buf, MAX_LINE_LEN - 1, in)){
        if (strcmp(buf, end_sym_table) == 0){
            break;
        }
        // Lines contain the label and its ilc, separated by a single space
        char *pch = strtok(buf, " ");
        char *label = pch;
        pch = strtok(NULL, " ");
        int pos = atoi(pch) + ilc; // relative position + ilc of the begin-
                                   // ning of the module in the linked file.

        stInsert(st, label, pos);
    }

    while (fgetsNoNewline(buf, MAX_LINE_LEN - 1, in)){
        char *instr = strCopyToNew(buf);
        vPush(instrs, &instr);
        ilc++;
    }

    return ilc;
}

void gatherProgramsAndLabels(char *main_addr, Vector *modules_addr, Vector
        *instrs, SymTable *st){
    int ilc = gatherFromFile(main_addr, instrs, st, 0);
    for (VIter it = vBegin(modules_addr);
         viIndex(&it) < vLen(modules_addr);
         vNext(modules_addr, &it)){
        ilc = gatherFromFile(*(char **)viData(&it), instrs, st, ilc);
    }
}

void processLabelsAndPrint(Vector *instrs, SymTable *st, char *out_addr){
    FILE *out = fopen(out_addr, "w");

    int ilc = 0;

    for (VIter it = vBegin(instrs);
         viIndex(&it) < vLen(instrs);
         vNext(instrs, &it)){
        char *instr = *(char **)viData(&it);

        int label_pos = stGet(st, instr);

        if (label_pos == -1){ // regular instruction, not a label
            if (strcmp("", instr) != 0) // Not an empty line
                fprintf(out, "%s\n", instr);
        } else {
            label_pos -= ilc; // all addresses are relative
            fprintf(out, "%d\n", label_pos);
        }

        ilc++;
    }

    fclose(out);
}

void linkModules(char *main_addr, Vector *modules_addr, char *out_addr){
    Vector *instrs = vCreate(sizeof(char *), free);
    SymTable *st = stCreate();
    gatherProgramsAndLabels(main_addr, modules_addr, instrs, st);
    processLabelsAndPrint(instrs, st, out_addr);
    stDestroy(st);
    vDestroy(instrs);
}
