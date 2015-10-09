#pragma once
#include "emulator.h"

typedef enum {
    instr_nop,
    instr_load,
    instr_store,
    instr_read,
    instr_write,
    instr_copy,
    instr_neg,
    instr_sub,
    instr_add,
    instr_and,
    instr_or,
    instr_xor,
    instr_not,
    instr_jmp,
    instr_jz,
    instr_jnz,
    instr_jn,
    instr_jnn,
    instr_push,
    instr_pop,
    instr_call,
    instr_ret,
    instr_halt
} instr;

// Used to contain a function which performs the OPERation for an instruction.
// The member in which it should be stored depends on the number of parameters.
typedef union {
    void(*zero)(Emulator *);
    void(*one)(Emulator *, int);
    void(*two)(Emulator *, int, int);
} oper;

/**
 * Returns the corresponding function for a given instruction specifier in a
 * union.
 * @param  ins Instruction specifier (instr enum)
 * @return     oper union containing the function. The function may be in ei-
 *             ther union.zero, .one or .two, depending on its number of para-
 *             meters.
 */
oper fetchInstr(instr ins);