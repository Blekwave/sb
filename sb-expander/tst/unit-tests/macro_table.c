#include "minunit.h"
#include "macro_table.h"
#include <stdio.h>
#include <string.h>

int tests_run = 0;

static char *macro_table_test(){
    MacroTable *mt = mtCreate();

    mu_assert("mt is null", mt != NULL);

    char macro_a[] = "LABEL: ADD R13 R11\n"
                     "LOAD R1 LABEL\n";

    char a_expected[] = "LABEL__MACRO0000000000: ADD R13 R11\n"
                        "LOAD R1 LABEL__MACRO0000000000\n";

    mtInsert(mt, "MACRO_A", &macro_a[0], NULL);

    char *a_out;

    mtEval(mt, "MACRO_A", NULL, &a_out);

    printf("%s\n%s\n", a_out, a_expected);
    mu_assert("Expected output differs from actual output", !strcmp(a_out, a_expected));

    mtDestroy(mt);
    
    // mu_assert("Iterator has wrong value (2)", *c == 13);        

    return 0;
}

static char *run_tests(){
    mu_run_test(macro_table_test);
    return 0;
}

int main(){
    char *result = run_tests();
    if (result){
        printf("%s\n", result);
        return 1;
    } else {
        printf("All tests passed successfully.\n");
    }

    return 0;
}