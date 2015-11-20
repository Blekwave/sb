#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#include "map.h"
#include "vector.h"

typedef struct {
    Map *m;
} MacroTable;

/**
 * Creates a new MacroTable
 * @return Address to the MacroTable
 */
MacroTable* mtCreate();

/**
 * Destroys an existing MacroTable, freeing all memory associated with it.
 * @param mt Address to a MacroTable
 */
void mtDestroy(MacroTable *mt);

/**
 * Inserts a macro into a MacroTable.
 * @param mt    MacroTable
 * @param name  Name of the macro (label field)
 * @param macro Text content of the macro (without BEGINMACRO and ENDMACRO,
 *              with everything inside it, including the line breaks after every
 *              instruction)
 * @param param Name of its parameter (or NULL, if it takes no parameters)
 */
void mtInsert(MacroTable *mt, char *name, Vector *macro, char *param);

/**
 * Evaluates a macro in the MacroTable, given its name and the value its para-
 * meter should assume (or NULL, if it takes no parameters). Returns a status 
 * value and the string corresponding to the evaluated macro.
 * @param  mt    MacroTable
 * @param  name  Name of the macro
 * @param  param Value its parameter should assume (or NULL)
 * @param  out   Pointer to a string, where the address to the string contai-
 *               ning the macro's evaluated text should be saved. This string
 *               is allocated dynamically, and must be freed afterwards if it
 *               gets generated correctly (nothing gets allocated if the call
 *               fails and returns 1, NULL is assigned to out in this case)
 * @return       0 if there is a macro with such a name in mt, 1 if there's no
 *               such macro.
 */
int mtEval(MacroTable *mt, char *name, char *param, char **out);

#endif
