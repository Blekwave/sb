#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"
#include "instr.h"

/**
 * Loads data from a file to an Emulator's memory.
 * @param  e        Emulator to which the data will be written.
 * @param  in_path  Path to the input file.
 * @param  load_pos Memory position where the data will begin to be written.
 * @return          0 if everything fit, 1 if the program didn't entirely fit.
 */
int emuLoadData(Emulator *e, const char *in_path, int load_pos){
    int mem_pos = 0;

    while (mem_pos < load_pos){
        e->mem[mem_pos] = 0;
        mem_pos++;
    }

    FILE *program = fopen(in_path, "r"); 
    int line;

    while (fscanf(program, "%d", &line) != EOF && mem_pos < MEM_SIZE){
        e->mem[mem_pos] = line;
        mem_pos++;
    }

    fclose(program);

    if (mem_pos == MEM_SIZE){ // Previous loop aborted before reaching EOF
        return 1; // Error: program might not have been read in its entirety.
    } else while (mem_pos < load_pos){
        e->mem[mem_pos] = 0;
        mem_pos++;
    }

    return 0;
}

/**
 * Makes a new emulator and loads a program from a file to its memory.
 * @param  in_path  Path of the input file.
 * @param  load_pos Memory position where the input will begin to be written.
 * @return          Address of the new emulator.
 */
Emulator *emuFromFile(const char *in_path, int load_pos){
    Emulator *e = malloc(sizeof(Emulator));

    if (emuLoadData(e, in_path, load_pos))
        fprintf(stderr, "WARNING: Input program might not have fit in memory properly.\n");

    return e;
}

void printRegisters(int *regs, int num_regs, FILE *out){
    int i;
    for (i = 0; i < num_regs; i++){
        if (i % 8 != 7)
            fprintf(out, "R%02d: %8d | ", i, regs[i]);
        else
            fprintf(out, "R%02d: %8d\n", i, regs[i]);
    }
}

/**
 * Runs the instruction pointed at by the PC and adjusts the PC for the next
 * instruction accordingly.
 * @param  e Emulator for which the instruction shall be run.
 * @return   identifier of the run instruction.
 */
instr runNext(Emulator *e){
    instr ins = (instr) e->mem[e->pc];
    int num_operands = instr_operands[ins];

    if (e->om == om_verbose){
        fprintf(e->out, "-----------------------------------------\n");
        fprintf(e->out, "PC: %d (%d), SP: %d, PSW: %d\n", e->pc,
                e->mem[e->pc], e->sp, e->psw);
        printRegisters(e->regs, NUM_REGS, e->out);
    }

    if (num_operands == 0){
        fetchInstr(ins).zero(e);
        e->pc++;
    } else if (num_operands == 1){
        int first_operand = e->mem[e->pc + 1];
        fetchInstr(ins).one(e, first_operand);
        e->pc += 2;
    } else { // num_operands == 2
        int first_operand = e->mem[e->pc + 1];
        int second_operand = e->mem[e->pc + 2];
        fetchInstr(ins).two(e, first_operand, second_operand);
        e->pc += 3;
    }
    return ins;
}

/**
 * Runs instructions in an Emulator until a halt instruction is executed.
 * @param e   Emulator to be run.
 * @param pc  Initial PC.
 * @param sp  Initial SP.
 * @param om  Output mode: simple/verbose
 * @param in  Input stream for the READ instruction.
 * @param out Output stream for the WRITE instruction and, if output mode is
 *            set to verbose, where the detailed runtime logs will be written.
 *            
 */
void emuRun(Emulator *e, int pc, int sp, output_mode om, FILE *in, FILE *out){
    e->pc = pc;
    e->sp = sp;
    e->om = om;
    e->psw = def;
    e->in = in;
    e->out = out;

    while (runNext(e) != instr_halt);
}