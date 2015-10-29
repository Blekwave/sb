/**
 * REAL_MD.h
 * Real instruction metadata.
 */
#ifndef REAL_MD_H
#define REAL_MD_H

#define NUM_REAL_INSTR 23

/**
 * Enum which defines the type of an operand in an instruction.
 * op_t_none - this operand doesn't exist
 * op_t_reg - this operand is a register
 * op_t_imm - this operand is an immediate value defined by a label
 */
typedef enum {
    op_t_none = 0,
    op_t_reg,
    op_t_imm
} op_type;

/**
 * Struct which contains metadata about an instruction.
 * opcode - instruction's opcode
 * num_ops - number of operands in an instruction
 * op1, op2 - types of the first and second operands, respectively.
 */
typedef struct {
    int opcode;
    int num_ops;
    op_type op1;
    op_type op2;
} RealMD; // Real instruction metadata

/**
 * Array of real instruction names.
 */
extern char *real_symbols[];

/**
 * Array of RealMD structs, indexed in parallel with the array of names above.
 */
extern RealMD real_md[];

#endif