// Instruction data table

#ifndef IDT_H
#define IDT_H

#include "real_md.h"
#include "pseudo_instr.h"
#include "assembler.h"
#include "map.h"

typedef enum {
    ins_t_real,
    ins_t_pseudo
} ins_type;

typedef union {
    RealMD real;
    PseudoInstr pseudo;
} InstrData;

typedef struct {
    ins_type type;
    InstrData data;
} Instr;

Map *idtCreate();

void idtDestroy(Map *idt);

#endif
