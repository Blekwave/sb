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

void asmDestroySymTable(Map *sym_table){
    mapDestroy(sym_table, free);
}

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

static void assembleOperand(char *op, op_type t, AsmData *ad, Map *sym_table){
    if (t == op_t_reg){
        ad->ilc++;
        fprintf(ad->out, "%s\n", op + 1);
    } else if (t == op_t_imm){
        ad->ilc++;
        int pos;
        int get_status = mapGet(sym_table, op, &pos);
        if (get_status == 0)
            fprintf(ad->out, "%d\n", pos - ad->ilc);
        else
            fprintf(ad->out, "%s\n", op);
    }
}

int asmReplaceAndSave(FILE *in, FILE *out, Map *idt, Map *sym_table){
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
            assembleOperand(l.op1, ins.data.real.op1, &ad, sym_table);
            assembleOperand(l.op2, ins.data.real.op2, &ad, sym_table);
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

int asmAssemble(const char *src_addr, const char *dest_addr){
    Map *idt = idtCreate();
    FILE *in = fopen(src_addr, "r");

    Map *sym_table = asmBuildSymTable(in, idt);

    rewind(in);

    FILE *out = fopen(dest_addr, "w");

    int status = asmReplaceAndSave(in, out, idt, sym_table);

    asmPrintSymTable(sym_table);

    fclose(in);
    fclose(out);
    asmDestroySymTable(sym_table);
    idtDestroy(idt);

    return status;
}