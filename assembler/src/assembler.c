#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "map.h"
#include "hash_aux.h"
#include "idt.h"

#define BUF_LEN 2048
#define NUM_BUCKETS 128
#define INTERRUPT_SIGNAL -1

typedef struct {
    char *label;
    char *instr;
    char *op1;
    char *op2;
} Line;

/**
 * Parses a line of assembly code, splitting it in its components: label, ope-
 * rator and operands.
 * @param in  Line of assembly code.
 * @param out Address to a Line struct, to which the line's components will be
 *            saved.
 */
static void parseLine(char *in, Line *out){
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

/**
 * Performs the first pass of the assembly, reading the code and creating a sym-
 * bol table, containing each label in the code.
 * @param  in  Pointer to the file containing the code. It should already have
 *             been opened in "r" mode and should point to the beginning of the
 *             file.
 * @param  idt Instruction Data Table created by idtCreate (idt.h)
 * @return     Address to the newly created symbol table.
 */
Map *asmBuildSymTable(FILE *in, Map *idt){
    Map *sym_table = mapCreate(sizeof(int), NUM_BUCKETS,
        (unsigned (*)(void *))djb2, // Hash function
        (int (*)(void *a, void *b))strcmp // Comparison function
    );

    Line l;
    Instr ins;
    int ilc = 0;

    char buf[BUF_LEN + 1];
    while (fgets(buf, BUF_LEN, in)){
        parseLine(buf, &l);
        if (l.label){
            mapInsert(sym_table, l.label, strlen(l.label) + 1, &ilc);
        }
        if (l.instr){
            int get_status = mapGet(idt, l.instr, &ins);
            if (get_status == 1){
                fprintf(stderr, "Invalid instruction: %s "
                        "(ILC: %d)\n", l.instr, ilc);
                exit(1);
            }
            if (ins.type == ins_t_real){
                ilc += 1 + ins.data.real.num_ops;
            } else {
                ilc += ins.data.pseudo.ilc_inc;
            }
        }
    }

    return sym_table;
}

/**
 * Deletes the symbol table and frees memory allocated for it.
 * @param sym_table Address to the symbol table
 */
void asmDestroySymTable(Map *sym_table){
    mapDestroy(sym_table, free);
}

/**
 * Iterates through the symbol table and prints its elements in no particular
 * order.
 * @param sym_table Address to the symbol table
 */
void asmPrintSymTable(Map *sym_table){
    MapIter i = mapBegin(sym_table);
    printf("[Symbol table]\n"
           "Label: address\n");
    while (i.n){
        const char *label = miKey(&i);
        const int *addr = miData(&i);
        printf("%s: %d\n", label, *addr);
        mapNext(sym_table, &i);
    }
}

const char * linker_sym_table_begin = "BEGINSM";
const char * linker_sym_table_end = "ENDSM";

void asmOutputSymTable(Map *sym_table, FILE *out){
    MapIter i = mapBegin(sym_table);
    fprintf(out, "%s\n", linker_sym_table_begin);
    while (i.n){
        const char *label = miKey(&i);
        const int *addr = miData(&i);
        fprintf(out, "%s %d\n", label, *addr);
        mapNext(sym_table, &i);
    }
    fprintf(out, "%s\n", linker_sym_table_end);
}

/**
 * Prints the machine code for a given operand. Behaves differently whether it
 * is a register or an immediate value.
 * @param op        Operand to be printed.
 * @param t         Type of the operand.
 * @param ad        Address to an AsmData structure, which contains information
 *                  about the assembly process (including the ILC).
 * @param sym_table Symbol table.
 */
static void assembleOperand(char *op, op_type t, AsmData *ad, Map *sym_table,
                            output_mode om){
    if (t == op_t_reg){
        ad->ilc++;
        fprintf(ad->out, "%s\n", op + 1);
    } else if (t == op_t_imm){
        ad->ilc++;
        int pos;
        int get_status = mapGet(sym_table, op, &pos);
        if (om != om_linker && get_status == 0)
            fprintf(ad->out, "%d\n", pos - ad->ilc);
        else
            fprintf(ad->out, "%s\n", op);
    }
}

/**
 * Performs the second pass of the assembly procedure, which actually assembles
 * the instructions and operands in the program.
 * @param  in        Pointer to the file containing the code. It should already
 *                   have been opened in "r" mode and should point to the begin-
 *                   ning of the file.
 * @param  out       Pointer to the output file, where the corresponding machine
 *                   code should be saved. It should have already been opened in
 *                   "w" mode, and should point to the beginning of the file.
 * @param  idt       Instruction data table, created by idtCreate ("idt.h").
 * @param  sym_table Symbol table obtained in the first pass.
 * @return           0 if everything went alright.
 */
int asmReplaceAndSave(FILE *in, FILE *out, Map *idt, Map *sym_table,
                      output_mode om){
    Line l;
    Instr ins;
    char buf[BUF_LEN + 1];
    AsmData ad = {.ilc = 0, .out = out};

    while (fgets(buf, BUF_LEN, in)){
        parseLine(buf, &l);
        if (!l.instr)
            continue;
        int get_status = mapGet(idt, l.instr, &ins);
        if (get_status == 1){
            fprintf(stderr, "Invalid instruction: %s "
                    "(ILC: %d)\n", l.instr, ad.ilc);
            exit(1);
        }
        if (ins.type == ins_t_real){
            fprintf(out, "%d\n", ins.data.real.opcode);
            ad.ilc++;
            assembleOperand(l.op1, ins.data.real.op1, &ad, sym_table, om);
            assembleOperand(l.op2, ins.data.real.op2, &ad, sym_table, om);
        } else { // ins_t_pseudo
            int call_status;
            if (ins.data.pseudo.num_ops == 0){
                call_status = ins.data.pseudo.call.zero(&ad);
            } else if (ins.data.pseudo.num_ops == 1){
                call_status = ins.data.pseudo.call.one(&ad, l.op1);
            } else if (ins.data.pseudo.num_ops == 2){
                call_status = ins.data.pseudo.call.two(&ad, l.op1, l.op2);
            }
            ad.ilc += ins.data.pseudo.ilc_inc;
            if (call_status == INTERRUPT_SIGNAL)
                break;
        }
    }
    return 0;
}

/**
 * Assembles an existing assembly language program.
 * @param  src_addr  Path to the assembly file.
 * @param  dest_addr Path to the newly created machine code file.
 * @param  om        Output mode: simple or verbose (prints symbol table)
 * @return           Whether or not the assembly was successful. (Returns 0 if
 *                   everything went okay.)
 */
int asmAssemble(const char *src_addr, const char *dest_addr, output_mode om){
    Map *idt = idtCreate();
    FILE *in = fopen(src_addr, "r");

    Map *sym_table = asmBuildSymTable(in, idt);

    rewind(in);

    FILE *out = fopen(dest_addr, "w");

    if (om == om_verbose)
        asmPrintSymTable(sym_table);
    else if (om == om_linker)
        asmOutputSymTable(sym_table, out);

    int status = asmReplaceAndSave(in, out, idt, sym_table, om);

    fprintf(out, "\n");

    fclose(in);
    fclose(out);
    asmDestroySymTable(sym_table);
    idtDestroy(idt);

    return status;
}
