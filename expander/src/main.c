#include <stdio.h>
#include <stdlib.h>
#include "expander.h"

int main(int argc, char const* argv[])
{
    if (argc != 3){
        fprintf(stderr, "ERROR: wrong number of parameters\n"
                "Format: expander input_addr output_addr\n");
        exit(1);
    }

    const char *src_addr = argv[1];
    const char *dest_addr = argv[2];

    expExpand(src_addr, dest_addr);

    return 0;
}
