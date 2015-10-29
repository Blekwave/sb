/**
 * BUCKET.h
 * Linked list whose nodes have two fields, for usage as buckets of my Map
 * implementation.
 */
#ifndef BUCKET_H
#define BUCKET_H

#include <stdlib.h> // size_t

typedef struct bucket_node {
    void *key;
    void *data;
    struct bucket_node *next;
} BNode;

/**
 * Initializes a single Bucket node, with two fields: a key value and a data
 * value.
 * @param  key  Address of key's data, to be copied to the new node.
 * @param  ksz  Size of key's data, in bytes.
 * @param  data Address of data's data, to be copied to the new node.
 * @param  dsz  Size of data's data, in bytes.
 * @param  next Pointer to the next node in the list.
 * @return      Address to the new node. Must be destroyed in order to avoid me-
 *              mory leaks.
 */
BNode *bNodeCreate(void *key, size_t ksz, void *data, size_t dsz,
                     BNode *next);

/**
 * Destroys a Bucket node, freeing its key and data values as well as the node
 * itself.
 * @param n        Node to be deleted.
 * @param freeKey  Function to be used in order to free the key value.
 * @param freeData Function to be used in order to free the data value.
 *
 * Note: stdlib's free is a valid function for both last parameters.
 */
void bNodeDestroy(BNode *n, void (*freeKey)(void *key),
                   void (*freeData)(void *data));

typedef struct {
    BNode *head;
    BNode *tail;
} Bucket;

/**
 * Initializes a new, empty bucket.
 * @return Address to the new bucket. Must be destroyed in order to avoid memory
 *         leaks.
 */
Bucket *bCreate();

/**
 * Destroys a Bucket and all of its nodes, freeing their key and data values as
 * well as the nodes themselves and the Bucket's structure.
 * @param b        Bucket to be deleted.
 * @param freeKey  Function to be used in order to free the key values.
 * @param freeData Function to be used in order to free the data values.
 *
 * Note: stdlib's free is a valid function for both last parameters.
 */
void bDestroy(Bucket *b, void (*freeKey)(void *key),
               void (*freeData)(void *data));

/**
 * Finds a node in the bucket by its key value, using a provided key comparison
 * function.
 * @param  b    Bucket in which the search will be performed.
 * @param  key  Key to be searched for.
 * @param  comp Comparison function which should return 0 only if the two ele-
 *              ments are equivalent.
 * @return      Address to the node with the given key or NULL if no nodes
 *              matched the search key.
 */
BNode *bFind(Bucket *b, void *key, int (*comp)(void *a, void *b));

/**
 * Given a node and the Bucket it belongs to, finds the previous node in the
 * list.
 * @param  b Bucket to which n belongs.
 * @param  n Node whose predecessor is desired.
 * @return   Address to n's predecessor or NULL if it can't be found. (in that 
 *           case, n likely does not belong to the list)
 */
BNode *bPrevNode(Bucket *b, BNode *n);

/**
 * Appends a new node to the end of a Bucket.
 * @param b    Bucket to which the new element shall be added.
 * @param key  Address to the key of the new element.
 * @param ksz  Size of the element key points to, in bytes.
 * @param data Address to the data of the new element.
 * @param dsz  Size of the element data points to, in bytes.
 */
void bInsert(Bucket *b, void *key, size_t ksz, void *data, size_t dsz);

/**
 * Removes a node from a Bucket and deletes it, freeing its key and data values
 * as well as the node itself.
 * @param b        Bucket n belongs to.
 * @param n        Node to be deleted.
 * @param freeKey  Function to be used in order to free the key value.
 * @param freeData Function to be used in order to free the data value.
 *
 * Note: stdlib's free is a valid function for both last parameters.
 */
int bRemove(Bucket *b, BNode *n, void (*freeKey)(void *key),
              void (*freeData)(void *data));

/**
 * Returns a Bucket's first actual node. (Disregards the head empty node.)
 * @param  b Bucket whose first element is desired.
 * @return   Address to b's first node.
 */
BNode *bBegin(Bucket *b);

/**
 * Returns the address of the node which comes after last b's last node.
 * Actually returns NULL every time, used to mimic regular C++ iterators.
 * @param  b Bucket whose end is desired.
 * @return   Address to b's end.
 */
BNode *bEnd(Bucket *b);

/**
 * Given a Bucket's node, returns its successor in the bucket.
 * @param  n A bucket's node.
 * @return   n's successor.
 */
BNode *bNext(BNode *n);

#endif