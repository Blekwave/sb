#ifndef PSEUDO_INSTR_H
#define PSEUDO_INSTR_H

#include "assembler.h"

#define NUM_PSEUDO_INSTR 2

int psinWord(AsmData *ad, char *val);

int psinEnd(AsmData *ad);

typedef struct {
    int num_ops;
    int ilc_inc;
    union {
        int (*zero)(AsmData *ad);
        int (*one)(AsmData *ad, char *op1);
        int (*two)(AsmData *ad, char *op1, char *op2);
    } call;
} PseudoInstr;

extern char *pseudo_symbols[];

extern PseudoInstr pseudo_instr[];

#endif
