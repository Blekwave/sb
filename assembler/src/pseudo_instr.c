#include "pseudo_instr.h"

// Function documentation is available at the header file

int psinWord(AsmData *ad, char *val){
    fprintf(ad->out, "%s\n", val);
    return 0;
}

int psinEnd(AsmData *ad __attribute__((unused))){
    return -1; // Signals the assembler to stop
}

char *pseudo_symbols[] = {
    "WORD",
    "END"
};

PseudoInstr pseudo_instr[] = {
    {
        .num_ops = 1,
        .ilc_inc = 1,
        .call.one = psinWord
    },
    {
        .num_ops = 0,
        .ilc_inc = 0,
        .call.zero = psinEnd
    }
};
