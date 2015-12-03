#include "minunit.h"
#include "bucket.h"
#include <stdio.h>
#include <string.h>

int tests_run = 0;

/**
 * Tests regarding the BNode structure.
 */
static char *bucket_node_test(){
    char *key = "Yoloswaggulas Cage";
    int val = 42;

    BNode *n = bNodeCreate(key, strlen(key) + 1, &val, sizeof(int), NULL);

    mu_assert("F: n == NULL", n != NULL);
    mu_assert("F: n->key == NULL", n->key != NULL);
    mu_assert("F: n->data == NULL", n->data != NULL);

    int *out = n->data;

    mu_assert("F: n->data != 42", *out == 42);

    bNodeDestroy(n, free, free);

    return 0;
}

/**
 * Tests regarding the Bucket structure.
 */
static char *bucket_test(){
    const int test_entries = 7;
    char *keys[] = {"yo", "lo", "swag", "gu", "las", "ca", "ge"};
    int vals[] =  { 7,    77,   777,    7777, 77777, -32,  12 };

    // Initialization tests
    Bucket *b = bCreate();
    mu_assert("F: b == NULL", b != NULL);
    mu_assert("F: b->head != b->tail", b->head == b->tail);

    // Payload insertion tests
    for (int i = 0; i < test_entries; i++){
        bInsert(b, keys[i], strlen(keys[i]) + 1, &vals[i], sizeof(int));
        mu_assert("F: b->tail->data is wrong",
                  *(int *)b->tail->data == vals[i]);
        mu_assert("F: b->tail->key is wrong",
                  strcmp((char *)b->tail->key, keys[i]) == 0);
    }

    // # of elements test
    int node_cnt = 0;
    for (BNode *it = bBegin(b); it != bEnd(b); it = bNext(it)){
        node_cnt++;
    }
    mu_assert("F: node_cnt != test_entries", node_cnt == test_entries);

    // Search test
    BNode *n = bFind(b, "swag", (int(*)(void *, void *))strcmp);
    mu_assert("F: n ('swag') == NULL", n != NULL);
    mu_assert("F: n->data != 777", *(int *)n->data == 777);

    // Removal test
    int remove_status = bRemove(b, n, free, free);
    mu_assert("remove_status != 0", remove_status == 0);

    // # of elements test - Revengeance
    node_cnt = 0;
    for (BNode *it = bBegin(b); it != bEnd(b); it = bNext(it)){
        node_cnt++;
    }
    mu_assert("F: node_cnt != test_entries", node_cnt == test_entries - 1);

    bDestroy(b, free, free);

    return 0;
}

static char *run_tests(){
    mu_run_test(bucket_node_test);
    mu_run_test(bucket_test);
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