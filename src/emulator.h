#pragma once

#define NUM_REGS 16
#define MEM_SIZE 2048

typedef enum {
    om_simple,
    om_verbose
} output_mode;

typedef enum {
    def,
    zero,
    negative
} psw;

typedef struct {
    int pc;
    int sp;
    psw psw;
    int regs[NUM_REGS];
    int mem[MEM_SIZE];
    output_mode om;
    FILE *in;
    FILE *out;
} Emulator;

Emulator *emuFromFile(const char *in_path, int load_pos);

void emuRun(Emulator *e, int pc, int sp, output_mode om, FILE* in, FILE *out);