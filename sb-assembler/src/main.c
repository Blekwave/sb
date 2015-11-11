#include <stdio.h>
#include "assembler.h"

int main(int argc, char const *argv[])
{
    // TODO: Map iteration (verbose prints the symbols table)
    
    const char *src_addr = argv[1];
    const char *dest_addr = argv[2];

    output_mode om;

    if (argc == 4 && argv[3][0] == 'v'){
        om = om_verbose;
    } else {
        om = om_simple;
    }

    asmAssemble(src_addr, dest_addr, om);

    return 0;
}