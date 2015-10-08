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

typedef union {
    void(*zero)(Emulator *);
    void(*one)(Emulator *, int);
    void(*two)(Emulator *, int, int);
} oper;

oper fetchInstr(instr ins);