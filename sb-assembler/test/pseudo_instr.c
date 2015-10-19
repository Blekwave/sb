#include "minunit.h"
#include "pseudo_instr.h"
#include "assembler.h"
#include <stdio.h>
#include <string.h>

int tests_run = 0;

static char *functions_test(){
    AsmData ad = {.ilc = 3, .out = stdout};

    int ret_val = pseudo_instr[0].call.one(&ad, "33022033");

    mu_assert("psinWord's ret_val != 0", ret_val == 0);
    mu_assert("ilc after psinWord != 3", ad.ilc == 3);

    ret_val = pseudo_instr[1].call.zero(&ad);

    mu_assert("psinEnd's ret_val != -1", ret_val == -1);

    return 0;
}

static char *array_test(){
    mu_assert("pseudo_symbols[0] != WORD", !strcmp(pseudo_symbols[0], "WORD"));
    mu_assert("pseudo_symbols[1] != END", !strcmp(pseudo_symbols[1], "END"));

    mu_assert("pseudo_instr[0].num_ops != 1", pseudo_instr[0].num_ops == 1);
    mu_assert("pseudo_instr[1].num_ops != 0", pseudo_instr[1].num_ops == 0);

    return 0;
}

static char *run_tests(){
    mu_run_test(functions_test);
    mu_run_test(array_test);
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