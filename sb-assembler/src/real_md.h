#ifndef REAL_MD_H
#define REAL_MD_H

#define NUM_REAL_INSTR 23

typedef enum {
    op_t_none = 0,
    op_t_reg,
    op_t_imm
} op_type;

typedef struct {
    int opcode;
    int num_ops;
    op_type op1;
    op_type op2;
} RealMD; // Real instruction metadata

extern char *real_symbols[];

extern RealMD real_md[];

#endif