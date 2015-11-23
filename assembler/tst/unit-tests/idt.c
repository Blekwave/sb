#include "minunit.h"
#include "idt.h"
#include "hash_aux.h"
#include "map.h"

#include <stdio.h>
#include <string.h>

int tests_run = 0;

static char *idt_test(){
    Map *idt = idtCreate();

    mu_assert("idt == NULL", idt != NULL);

    Instr a;

    fprintf(stderr, "%d\n", (int)djb2((void *)"ADD") % 32);

    char *key = "ADD";

    int get_status = mapGet(idt, key, &a);

    mu_assert("couldn't find ADD in IDT", get_status == 0);

    mu_assert("ins_type != real (ADD)", a.type == ins_t_real);
    mu_assert("ADD's opcode != 8", a.data.real.opcode == 8);

    get_status = mapGet(idt, "WORD", &a);

    mu_assert("couldn't find WORD in IDT", get_status == 0);

    mu_assert("ins_type != pseudo (WORD)", a.type == ins_t_pseudo);
    mu_assert("num_ops != 1 (WORD)", a.data.pseudo.num_ops == 1);

    idtDestroy(idt);
    return 0;
}

static char *run_tests(){
    mu_run_test(idt_test);
    return 0;
}

int main(){
    char *result = run_tests();
    if (result){
        printf("%s\n", result);
    } else {
        printf("All tests passed successfully.\n");
    }

    return 0;
}