#include "minunit.h"
#include "real_md.h"

#include <stdio.h>
#include <string.h>

int tests_run = 0;

static char *array_test(){
    mu_assert("real_symbols[0] != NOP", !strcmp(real_symbols[0], "NOP"));
    mu_assert("real_symbols[22] != HALT", !strcmp(real_symbols[22], "HALT"));


    mu_assert("0.num_ops != 0", real_md[0].num_ops == 0);
    mu_assert("0.opcode != 0", real_md[0].opcode == 0);
    mu_assert("0.opcode.op1 != op_t_none", real_md[0].op1 == op_t_none);

    mu_assert("1.num_ops != 2", real_md[1].num_ops == 2);
    mu_assert("1.opcode != 1", real_md[1].opcode == 1);
    mu_assert("1.opcode.op1 != op_t_reg", real_md[1].op1 == op_t_reg);
    mu_assert("1.opcode.op2 != op_t_imm", real_md[1].op2 == op_t_imm);

    return 0;
}

static char *run_tests(){
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