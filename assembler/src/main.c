#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

int main(int argc, char const *argv[])
{
    if (argc < 3 || argc > 4){
        fprintf(stderr, "ERROR: Wrong call format or number of parameters.\n"
                "FORMAT: <assembler exec.> in_file out_file [s|l|v]\n"
                "(s: simple, l: linker output, v: verbose)\n");
        exit(1);
    }

    const char *src_addr = argv[1];
    const char *dest_addr = argv[2];

    output_mode om;

    if (argc == 4 && argv[3][0] == 'v'){
        om = om_verbose;
    } if (argc == 4 && argv[3][0] == 's'){
        om = om_simple;
    } else {
        om = om_linker;
    }

    asmAssemble(src_addr, dest_addr, om);

    return 0;
}
