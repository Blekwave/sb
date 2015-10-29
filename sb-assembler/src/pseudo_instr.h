/**
 * PSEUDO_INSTR.h
 * Pseudo-instructions defined by the assembler.
 */
#ifndef PSEUDO_INSTR_H
#define PSEUDO_INSTR_H

#include "assembler.h"

#define NUM_PSEUDO_INSTR 2

/**
 * Struct which contains data for execution of a pseudo-instruction.
 *
 * num_ops - Number of operands for the pseudo-instruction
 * ilc_inc - For how much the instruction location counter should be increased
 *           after executing this pseudo-instruction.
 * call - Function to be called when the pseudo-instruction is called (union
 *        field depends on its number of operands)
 */
typedef struct {
    int num_ops;
    int ilc_inc;
    union {
        int (*zero)(AsmData *ad);
        int (*one)(AsmData *ad, char *op1);
        int (*two)(AsmData *ad, char *op1, char *op2);
    } call;
} PseudoInstr;

/**
 * Array of pseudo-instruction names
 */
extern char *pseudo_symbols[];

/**
 * Array of PseudoInstr structs, indexed by the same index as the instruction
 * names above.
 */
extern PseudoInstr pseudo_instr[];

#endif
