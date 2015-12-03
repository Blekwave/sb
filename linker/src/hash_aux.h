#ifndef HASH_AUX_H
#define HASH_AUX_H

/**
 * String hash function. Returns a consistent, arbitrary unsigned integer asso-
 * ciated to a given string.
 * @param  str String.
 * @return     Arbitrary unsigned integer value.
 */
unsigned int djb2(unsigned char *str);

#endif