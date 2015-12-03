#include "minunit.h"
#include "vector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int tests_run = 0;

static char *vector_test(){
    Vector *v = vCreate(sizeof(int), NULL);
    mu_assert("v is null", v);

    for (int i = 0; i < 200; i++){
        vPush(v, &i);
        mu_assert("wrong vector length", vLen(v) == i + 1);
    }

    // printf("Len: %d, arr_len: %d\n", v->len, v->arr_len);

    int a = 13;
    vReplace(v, 0, &a);
    mu_assert("First replacement failed", ((int *)v->arr)[0] == 13);

    vReplace(v, 10, &a);
    mu_assert("Second replacement failed", ((int *)v->arr)[10] == 13);

    int b;
    vPop(v, &b);
    mu_assert("Wrong value from vPop", b == 199);
    mu_assert("Wrong length after pop", vLen(v) == 199);

    vPop(v, NULL);
    mu_assert("Wrong length after second pop", vLen(v) == 198);

    vGet(v, 10, &b);
    mu_assert("Wrong value from vGet", b == 13);

    VIter it = vBegin(v);
    int *c = viData(&it);
    mu_assert("Iterator has wrong value (1)", *c == 13);

    for (int i = 0; i < 10; i++){
        vNext(v, &it);
        c = viData(&it);
    }

    mu_assert("Iterator has wrong value (2)", *c == 13);

    vDestroy(v);
    v = NULL;
    return 0;
}

char *strToCopy(char *src){
    char *new = malloc(strlen(src) + 1);
    strcpy(new, src);
    return new;
}

static char *vector_strings_test(){
    Vector *v = vCreate(sizeof(char *), free);

    char *strings[] = {
        "Some text",
        "here and there",
        "won't hurt",
        "anybody"
    };

    const int num_strings = 4;

    for (int i = 0; i < num_strings; i++){
        char *new_str = strToCopy(strings[i]);
        vPush(v, &new_str);
    }

    for (VIter it = vBegin(v); viIndex(&it) < vLen(v); vNext(v, &it)){
        fprintf(stderr, "%s\n", *(char **)viData(&it));
    }

    vDestroy(v);
    return 0;
}

static char *run_tests(){
    mu_run_test(vector_test);
    mu_run_test(vector_strings_test);
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
