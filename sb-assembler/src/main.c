#include <stdio.h>
#include "assembler.h"

int main(int argc, char const *argv[])
{
    // TODO: Map iteration (verbose prints the symbols table)
    
    const char *src_addr = argv[1];
    const char *dest_addr = argv[2];

    if (argc == 4){
        fprintf(stderr, "Verbose tbd\n");
    }

    asmAssemble(src_addr, dest_addr);

    return 0;
}