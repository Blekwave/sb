/**
 * MAP.h
 * Generic associative array library.
 */
#ifndef MAP_H
#define MAP_H

#include "bucket.h"

typedef struct {
    Bucket **bs; // Buckets
    int len; // # of buckets
    size_t dsz; // Size of data
    unsigned int (*hash)(void *key);
    int (*keyComp)(void *a, void *b);
} Map;

typedef struct {
    BNode *n;
    int b; // Index of the bucket to which this belongs
} MapIter;

/**
 * Initializes and allocates a new Map.
 * @param  dsz  Size of the data chunks
 * @param  len  Number of buckets
 * @param  hash Hash function, which accepts a pointer to a key and returns an
 *              arbitrary unsigned integer.
 * @return      Address to the new Map. Must be freed later via mapDestroy.
 */
Map *mapCreate(size_t dsz, int len, unsigned int (*hash)(void *key), 
              int (*keyComp)(void *a, void *b));

/**
 * Deletes an existing map, freeing its memory and all the data inside of it.
 * @param m        Map to be deleted
 * @param freeData Auxiliary function. Each element's data pointer will be pas-
 *                 sed to it. (stdlib's free is a valid parameter, if there's
 *                 no underlying structures to the data).
 */
void mapDestroy(Map *m, void (*freeData)(void *data));

/**
 * Inserts an element in a map for a given key, replaces data if such key al-
 * ready exists.
 * @param m    Map in which the element should be inserted.
 * @param key  Key in which the element should be inserted.
 * @param ksz  Size of the key, in bytes.
 * @param data Pointer to the data to be copied. (Exactly `dsz` bytes shall be
 *             copied, as defined in mapCreate.)
 */
void mapInsert(Map *m, void *key, size_t ksz, void *data);

/**
 * Retrieves the saved data for a given key, if such key exists in the table.
 * Returns a status which denotes whether the data was successfully retrieved
 * or not.
 * @param  m   Map from which the element should be gotten.
 * @param  key Key for which the retrieval should be done.
 * @param  out Address to which the data shall be copied, if the element exists
 *             in the map.
 * @return     0 if the operation has been successful, 1 otherwise.
 */
int mapGet(Map *m, void *key, void *out);

/**
 * Pops an element from the Map, deleting it from the structure and optionally
 * saving its value to an address.
 * @param  m   Map from which the element should be gotten.
 * @param  key Key for which the retrieval should be done.
 * @param  out Address to which the data shall be copied, if the element exists
 *             in the map. May be set as NULL if the saving the value is irre-
 *             levant.
 * @return     0 if the operation has been successful, 1 otherwise.
 */
int mapPop(Map *m, void *key, void *out, void (*freeData)(void *data));

/**
 * Returns address to the data in the node of an iterator.
 * @param  i Address to the iterator.
 * @return   Address to its data.
 */
void *miData(MapIter *i);

/**
 * Returns address to the key in the node of an iterator.
 * @param  i Address to the iterator.
 * @return   Address to its key.
 */
void *miKey(MapIter *i);

/**
 * Returns iterator to the first element of the Map.
 * @param  m Map
 * @return   Iterator to its first node.
 */
MapIter mapBegin(Map *m);

/**
 * Returns the iterator to the element past the last node in the map. This means
 * an iterator which points to a NULL node in the (m->len + 1)th bucket.
 * @param  m Map
 * @return   Iterator to a NULL node past the end of the map.
 */
MapIter mapEnd(Map *m);

/**
 * Sets an iterator to the next element in the iteration.
 * @param m Map to which the iterator belongs.
 * @param i Address to the iterator.
 */
void mapNext(Map *m, MapIter *i);

#endif