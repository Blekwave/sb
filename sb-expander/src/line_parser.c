#include "line_parser.h"
#include <string.h>

void parseLine(char *in, Line *out){
    *out = (Line){NULL, NULL, NULL, NULL};

    // Ignores comments by cutting the string off at the comment delimiter
    char *comment_delim = strchr(in, ';');
    if (comment_delim)
        *comment_delim = '\0';

    char *pch = strtok(in, " \t\n");
    if (!pch)
        return;

    char *colon = strchr(pch, ':');
    if (colon){
        *colon = '\0';
        out->label = pch;
        pch = strtok(NULL, " \t\n");
        if (!pch)
            return;
    }

    out->instr = pch;
    pch = strtok(NULL, " \t\n");
    if (!pch)
        return;
    out->op1 = pch;

    pch = strtok(NULL, " \t\n");
    if (!pch)
        return;
    out->op2 = pch;
}
