#ifndef EXPANDER_H
#define EXPANDER_H

/**
 * Performs the expansion procedure, given a source file and an address to be
 * written.
 * @param  src_addr  Address of the source file.
 * @param  dest_addr Address of the output file (to be written)
 * @return           0 if everything went okay.
 */
int expExpand(const char *src_addr, const char *dest_addr);

#endif
