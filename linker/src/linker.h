#ifndef LINKER_H
#define LINKER_H

#include "vector.h"

void linkModules(char *main_addr, Vector *modules_addr, char *out_addr);

#endif

