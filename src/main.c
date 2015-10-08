#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"

#define MIN_ARGS 5
#define MAX_ARGS 6

int main(int argc, char const *argv[])
{
    if (argc < MIN_ARGS || argc > MAX_ARGS){
        fprintf(stderr, "ERROR: Too few/too many command line parameters.\n");
        fprintf(stderr, "Format: ./program PC SP load_pos input_path [s|v]\n");
        exit(1);
    }

    int pc = atoi(argv[1]);
    int sp = atoi(argv[2]);
    int load_pos = atoi(argv[3]);
    const char *input_path = argv[4];

    output_mode om;
    if (argc == MAX_ARGS && argv[5][0] == 'v'){
        om = om_verbose;
    } else {
        om = om_simple;
    }

    Emulator *e = emuFromFile(input_path, load_pos);

    emuRun(e, pc, sp, om, stdin, stdout);

    free(e);

    return 0;
}