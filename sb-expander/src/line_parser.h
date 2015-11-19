#ifndef LINE_PARSER_H
#define LINE_PARSER_H

typedef struct {
    char *label;
    char *instr;
    char *op1;
    char *op2;
} Line;

/**
 * Parses a line of assembly code, splitting it in its components: label, ope-
 * rator and operands. Does NOT keep the original string intact.
 * @param in  Line of assembly code.
 * @param out Address to a Line struct, to which the line's components will be
 *            saved.
 */
void parseLine(char *in, Line *out);

#endif