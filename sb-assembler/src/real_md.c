#include "real_md.h"

char *real_symbols[] = {
    "NOP",
    "LOAD",
    "STORE",
    "READ",
    "WRITE",
    "COPY",
    "NEG",
    "SUB",
    "ADD",
    "AND",
    "OR",
    "XOR",
    "NOT",
    "JMP",
    "JZ",
    "JNZ",
    "JN",
    "JNN",
    "PUSH",
    "POP",
    "CALL",
    "RET",
    "HALT"
};

RealMD real_md[] = {
    { // NOP
        .opcode = 0,
        .num_ops = 0,
        .op1 = op_t_none,
        .op2 = op_t_none
    },
    { // LOAD
        .opcode = 1,
        .num_ops = 2,
        .op1 = op_t_reg,
        .op2 = op_t_imm
    },
    { // STORE
        .opcode = 2,
        .num_ops = 2,
        .op1 = op_t_reg,
        .op2 = op_t_imm
    },
    { // READ
        .opcode = 3,
        .num_ops = 1,
        .op1 = op_t_reg,
        .op2 = op_t_none
    },
    { // WRITE
        .opcode = 4,
        .num_ops = 1,
        .op1 = op_t_reg,
        .op2 = op_t_none
    },
    { // COPY
        .opcode = 5,
        .num_ops = 2,
        .op1 = op_t_reg,
        .op2 = op_t_reg
    },
    { // NEG
        .opcode = 6,
        .num_ops = 1,
        .op1 = op_t_reg,
        .op2 = op_t_none
    },
    { // SUB
        .opcode = 7,
        .num_ops = 2,
        .op1 = op_t_reg,
        .op2 = op_t_reg
    },
    { // ADD
        .opcode = 8,
        .num_ops = 2,
        .op1 = op_t_reg,
        .op2 = op_t_reg
    },
    { // AND
        .opcode = 9,
        .num_ops = 2,
        .op1 = op_t_reg,
        .op2 = op_t_reg
    },
    { // OR
        .opcode = 10,
        .num_ops = 2,
        .op1 = op_t_reg,
        .op2 = op_t_reg
    },
    { // XOR
        .opcode = 11,
        .num_ops = 2,
        .op1 = op_t_reg,
        .op2 = op_t_reg
    },
    { // NOT
        .opcode = 12,
        .num_ops = 1,
        .op1 = op_t_reg,
        .op2 = op_t_none
    },
    { // JMP
        .opcode = 13,
        .num_ops = 1,
        .op1 = op_t_imm,
        .op2 = op_t_none
    },
    { // JZ
        .opcode = 14,
        .num_ops = 1,
        .op1 = op_t_imm,
        .op2 = op_t_none
    },
    { // JNZ
        .opcode = 15,
        .num_ops = 1,
        .op1 = op_t_imm,
        .op2 = op_t_none
    },
    { // JN
        .opcode = 16,
        .num_ops = 1,
        .op1 = op_t_imm,
        .op2 = op_t_none
    },
    { // JNN
        .opcode = 17,
        .num_ops = 1,
        .op1 = op_t_imm,
        .op2 = op_t_none
    },
    { // PUSH
        .opcode = 18,
        .num_ops = 1,
        .op1 = op_t_reg,
        .op2 = op_t_none
    },
    { // POP
        .opcode = 19,
        .num_ops = 1,
        .op1 = op_t_reg,
        .op2 = op_t_none
    },
    { // CALL
        .opcode = 20,
        .num_ops = 1,
        .op1 = op_t_imm,
        .op2 = op_t_none
    },
    { // RET
        .opcode = 21,
        .num_ops = 0,
        .op1 = op_t_none,
        .op2 = op_t_none
    },
    { // HALT
        .opcode = 22,
        .num_ops = 0,
        .op1 = op_t_none,
        .op2 = op_t_none
    }
};
