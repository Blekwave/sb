#include "minunit.h"
#include "map.h"
#include <stdio.h>
#include <string.h>

int tests_run = 0;

unsigned int djb2(unsigned char *str)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

static char *map_test(){
    int map_len = 8;
    
    // Initialization test
    Map *m = mapCreate(sizeof(int), map_len, (unsigned int (*)(void *))djb2,
                      (int(*)(void *a, void *b))strcmp);
    mu_assert("m == NULL", m != NULL);
    mu_assert("m->len != 8", m->len == 8);

    // Insertion tests
    char *key = "sprite";
    int val = 33;

    mapInsert(m, key, strlen(key) + 1, &val);

    unsigned int bucket_id = djb2((unsigned char *)key) % map_len;
    Bucket *b = m->bs[bucket_id];

    mu_assert("bucket is empty", b->tail != b->head);
    mu_assert("bucket->last->data != val", *(int *)b->tail->data == val);

    // Retrieval tests
    int x = 12, get_status;

    get_status = mapGet(m, key, &x);
    mu_assert("x has the wrong value", x == val);
    mu_assert("get_status != 0", get_status == 0);

    // Removal tests
    int y = 44, pop_status;

    pop_status = mapPop(m, key, &y, free);
    mu_assert("c has the wrong value", y == val);
    mu_assert("pop_status != 0", pop_status == 0);
    mu_assert("bucket is not empty", b->tail == b->head);

    mapDestroy(m, free);
    return 0;
}

static char *run_tests(){
    mu_run_test(map_test);
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