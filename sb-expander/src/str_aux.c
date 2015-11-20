#include "str_aux.h"
#include <stdlib.h>
#include <string.h>

char *strCopyToNew(char *str){
    char *new = malloc(strlen(str) + 1);
    strcpy(new, str);
    return new;
}


Vector *strToVector(char *str, char *delims){
    Vector *v = vCreate(sizeof(char *), free);
    char *mangled_buffer = strCopyToNew(str);
    char *pch = strtok(mangled_buffer, delims);
    while (pch){
        char *line_copy = strCopyToNew(pch);
        vPush(v, &line_copy);
        pch = strtok(NULL, delims);
    }
    free(mangled_buffer);
    return v;
}

