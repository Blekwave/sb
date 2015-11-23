/**
 * IDT.h
 * Instruction data table, which contains data about all real and pseudo ins-
 * tructions supported by the assembler. Built using Map.h and accessible via
 * this library's methods.
 */

#ifndef IDT_H
#define IDT_H

#include "real_md.h"
#include "pseudo_instr.h"
#include "assembler.h"
#include "map.h"

/**
 * Enum which differentiates real and pseudo instructions.
 */
typedef enum {
    ins_t_real,
    ins_t_pseudo
} ins_type;

/**
 * Contains either a real instruction's metadata or a pseudo instruction.
 */
typedef union {
    RealMD real;
    PseudoInstr pseudo;
} InstrData;

/**
 * An instruction.
 * type - whether this is a real or a pseudo instruction.
 * data - instruction's data (or metadata)
 */
typedef struct {
    ins_type type;
    InstrData data;
} Instr;

/**
 * Creates an IDT based on real_md.h and pseudo_instr.h's instructions.
 * @return Address to the new map.
 */
Map *idtCreate();

/**
 * Destroys an existing IDT.
 * @param idt IDT to be destroyed.
 */
void idtDestroy(Map *idt);

#endif
