#ifndef STR_AUX_H
#define STR_AUX_H

#include "vector.h"

/**
 * Copies a string to a newly allocated string.
 * @param  str String to be copied
 * @return     Address to a new string, of the same size, containing the same
 *             content. Must be freed.
 */
char *strCopyToNew(char *str);

/**
 * Splits a string into a Vector of strings (char *), given a string of delimi-
 * ters. 
 * @param  str    String to be split
 * @param  delims String containing all the delimiters for splitting (same for-
 *                mat as string's strtok)
 * @return        Address of the new Vector. Must be destroyed.
 */
Vector *strToVector(char *str, char *delims);

#endif
