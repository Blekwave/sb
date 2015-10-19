#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h> // FILE type
#include "map.h"

typedef struct {
    int ilc;
    FILE *out;
} AsmData;

int asmAssemble(const char *src_addr, const char *dest_addr);

#endif