#pragma once

#define NUM_REGS 16
#define MEM_SIZE 2048

// Used to define the output mode of the program
typedef enum {
    om_simple,
    om_verbose
} output_mode;

// Used to characterize the output of the last operation
typedef enum {
    def = 0,
    zero = 1,
    negative = 2
} psw;

// The Emulator itself
typedef struct {
    int pc;
    int sp;
    psw psw;
    int regs[NUM_REGS]; // Register file
    int mem[MEM_SIZE];
    output_mode om;
    FILE *in; // Input stream
    FILE *out; // Output stream (also used for verbose logs)
} Emulator;

/**
 * Makes a new emulator and loads a program from a file to its memory.
 * @param  in_path  Path of the input file.
 * @param  load_pos Memory position where the input will begin to be written.
 * @return          Address of the new emulator.
 */
Emulator *emuFromFile(const char *in_path, int load_pos);

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
void emuRun(Emulator *e, int pc, int sp, output_mode om, FILE* in, FILE *out);