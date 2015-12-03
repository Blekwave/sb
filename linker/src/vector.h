#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h> // size_t

#define VECTOR_DEFAULT_LENGTH 4
#define VECTOR_REALLOCATION_FACTOR 1.5

typedef struct {
    void *arr;
    size_t dsz;
    int len;
    int arr_len;
    void (*freeData)(void *);
} Vector;

typedef struct {
    void *data;
    int index;
} VIter;

/**
 * Creates a new empty Vector for data of a given size. Optionally, a freeData
 * function can be defined to delete elements whenever they're replaced or re-
 * moved.
 * @param dsz      Size of each element, in bytes.
 * @param freeData Auxiliary function to which the address of the freed data
 *                 will be passed. (stdlib's free is a valid parameter)
 */
Vector *vCreate(size_t dsz, void(*freeData)(void *));

/**
 * Deletes an existing Vector and frees the memory associated with it (and its
 * elements, using the function which was provided on creation).
 * @param v Vector to be deleted.
 */
void vDestroy(Vector *v);

/**
 * Adds a new element to the end of the Vector.
 * @param v    Vector to which the new element will be added.
 * @param data Address to the data of size dsz which should be copied to the
 *             Vector.
 */
void vPush(Vector *v, void *data);

/**
 * Replaces an element in a Vector with another data value. Frees the old data
 * value with freeData, if provided.
 * @param v     Vector
 * @param index Index of the element.
 * @param data  Address of the new data.
 */
void vReplace(Vector *v, int index, void *data);

/**
 * Gets an element's value from a Vector, given an index.
 * @param v     Vector
 * @param index Index of the element
 * @param out   Address to which the data of size dsz should be copied.
 */
void vGet(Vector *v, int index, void *out);

/**
 * Pops the element at the end of the Vector, removing it, freeing it with
 * freeData (if applicable) and outputting its value.
 * @param v   Vector
 * @param out Address to which the data of size dsz should be copied (or NULL)
 */
void vPop(Vector *v, void *out);

/**
 * Returns the number of elements in a Vector.
 * @param  v Vector
 * @return   Number of elements in it.
 */
int vLen(Vector *v);

/**
 * Returns the address to the data an iterator points to, in a Vector.
 * @param  i Iterator
 * @return   Address to its data, which is of size dsz.
 */
void *viData(VIter *i);

/**
 * Return the index of an iterator in its Vector.
 * @param  i Iterator
 * @return   Index of the element it points to.
 */
int viIndex(VIter *i);

/**
 * Returns an iterator to the first element in a Vector.
 * @param  v Vector
 * @return   Iterator to its first element.
 */
VIter vBegin(Vector *v);

/**
 * Return an iterator to the element past the last one in a Vector.
 * @param  v Vector
 * @return   Iterator to the element right past the end (its data is NULL)
 */
VIter vEnd(Vector *v);

/**
 * Increments a Vector iterator.
 * @param v Vector
 * @param i Iterator
 */
void vNext(Vector *v, VIter *i);

#endif
