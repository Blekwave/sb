#include "idt.h"
#include "real_md.h"
#include "hash_aux.h"
#include <string.h>
#include <stdlib.h>

// Function documentation is available at the header file

#define IDT_NUM_BUCKETS 32


Map *idtCreate(){
    Map *idt = mapCreate(sizeof(Instr), IDT_NUM_BUCKETS,
        (unsigned (*)(void *))djb2, // Hash function
        (int (*)(void *a, void *b))strcmp); // Comparison function

    // Real instructions
    Instr real_ins;
    real_ins.type = ins_t_real;

    for (int i = 0; i < NUM_REAL_INSTR; i++){
        real_ins.data.real = real_md[i];
        mapInsert(idt, real_symbols[i], strlen(real_symbols[i]) + 1, &real_ins);
    }

    // Pseudo instructions
    Instr pseudo_ins;
    pseudo_ins.type = ins_t_pseudo;

    for (int i = 0; i < NUM_PSEUDO_INSTR; i++){
        pseudo_ins.data.pseudo = pseudo_instr[i];
        mapInsert(idt, pseudo_symbols[i], strlen(pseudo_symbols[i]) + 1,
                  &pseudo_ins);
    }

    return idt;
}

void idtDestroy(Map *imdt){
    mapDestroy(imdt, free);
}
