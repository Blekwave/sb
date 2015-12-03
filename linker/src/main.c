#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linker.h"
#include "vector.h"
#include "str_aux.h"

char main_flag[] = "-m";
char out_flag[] = "-o";

int main(int argc, char const* argv[])
{
    if (argc < 5){
        fprintf(stderr, "ERROR: Not enough arguments or wrong format.\n"
                "FORMAT: <bin> [file1 file2 ...] -m main_file -o out_file\n");
        exit(1);
    }
    Vector *modules_addr = vCreate(sizeof(char *), free);
    char *main_addr;
    char *out_addr;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], main_flag) == 0){
            main_addr = (char *)argv[++i];
        } else if (strcmp(argv[i], out_flag) == 0){
            out_addr = (char *)argv[++i];
        } else {
            char *addr = strCopyToNew((char *)argv[i]);
            vPush(modules_addr, &addr);
        }
    }
    linkModules(main_addr, modules_addr, out_addr);
    vDestroy(modules_addr);
    return 0;
}
