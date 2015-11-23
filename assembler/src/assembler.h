/**
 * ASSEMBLER.h
 * Khattab Machine program assembler. Receives an assembly language program and
 * outputs the corresponding machine code.
 */
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h> // FILE type
#include "map.h"

typedef enum {
    om_simple,
    om_verbose
} output_mode;

typedef struct {
    int ilc;
    FILE *out;
} AsmData;

/**
 * Assembles an existing assembly language program.
 * @param  src_addr  Path to the assembly file.
 * @param  dest_addr Path to the newly created machine code file.
 * @param  om        Output mode: simple or verbose (prints symbol table)
 * @return           Whether or not the assembly was successful. (Returns 0 if
 *                   everything went okay.)
 */
int asmAssemble(const char *src_addr, const char *dest_addr, output_mode om);

#endif