#include "hash_aux.h"

// Function documentation is available at the header file

unsigned int djb2(unsigned char *str){
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}