#ifndef HEAP_HPP
#define HEAP_HPP

#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------------- *
 * Opaque structure
 *
 * Stores all necessary information for maintaining the heap structure.
 * ------------------------------------------------------------------------- */
typedef struct heap_t Heap;

/* ------------------------------------------------------------------------- *
 * Opaque structure
 *
 * A heap reference contains all the necessary information to efficiently
 * retrieve an element from the heap. In this case, efficiently means O(1).
 * ------------------------------------------------------------------------- */
typedef struct heap_ref_t HeapReference;

/* ------------------------------------------------------------------------- *
 * Create an empty max- or min-heap for storing double values.
 *
 * PARAMETERS
 * maxHeap         Must be set to true for a max-heap, to false for a min-heap.
 *
 * RETURN
 * Heap            A pointer to the created heap.
 *
 * NOTE
 * The returned structure should be cleaned with $heapFree$ after usage.
 * ------------------------------------------------------------------------- */
Heap* heapCreate(bool maxHeap);


/* ------------------------------------------------------------------------- *
 * Return the size of the heap.
 *
 * PARAMETERS
 * heap            A valid pointer the heap.
 *
 * RETURN
 * size            The size of the Heap.
 * ------------------------------------------------------------------------- */
size_t heapSize(const Heap* heap);

/* ------------------------------------------------------------------------- *
 * Obtain the double value associated with the given reference.
 *
 * PARAMETERS
 * heap            A valid pointer the heap
 * reference       A pointer to a valid reference to the element of which 
 *				   the value must be returned.
 *
 * RETURN
 * value           The double value corresponding to the reference.
 * ------------------------------------------------------------------------- */
double heapGet(const Heap* heap, const HeapReference* reference);

/* ------------------------------------------------------------------------- *
 * Obtain the double value stored at the top of the heap.
 *
 * PARAMETERS
 * heap            A valid pointer the heap
 *
 * RETURN
 * value           The double value corresponding to the element at the top
 *                 of the heap.
 *
 * NOTE
 * Calling this function on an empty heap results in an undefined behavior.
 * ------------------------------------------------------------------------- */
double heapTop(const Heap* heap);

/* ------------------------------------------------------------------------- *
 * Return the reference to the element at the top of the heap.
 *
 * PARAMETERS
 * heap            A valid pointer the heap
 *
 * RETURN
 * reference       A pointer to the reference to the element at the top of 
 *				   the heap. Must be freed after usage (with 'free()')
 *
 * NOTE
 * Calling this function on an empty heap results in an undefined behavior.
 * ------------------------------------------------------------------------- */
HeapReference* heapTopReference(const Heap* heap);

/* ------------------------------------------------------------------------- *
 * Add the given double value in the heap and return the reference to the
 * newly added element.
 *
 * PARAMETERS
 * heap            A valid pointer the heap
 * value           The double value to add to the heap.
 * dest            (by-pointer) The reference to fill with the reference of the
 *                 newly added element.
 *
 * RETURN
 * success         True for success, false for error
 *
 * NOTE
 * Adding a value should not invalidate any existing heap reference.
 * ------------------------------------------------------------------------- */
bool heapAdd(Heap* heap, double value, HeapReference* dest);

/* ------------------------------------------------------------------------- *
 * Efficiently remove the element referenced by $toReplace$ and replace its
 * value by $value$.
 *
 * PARAMETERS
 * heap            A valid pointer the heap
 * toReplace       A pointer to a valid reference to the element to replace.
 * value           The new double value to add to the heap.
 *
 * NOTE
 * The reference $toReplace$ is used to store the reference of the newly added
 * element.
 * ------------------------------------------------------------------------- */
void heapReplace(Heap* heap, HeapReference* toReplace, double value);

/* ------------------------------------------------------------------------- *
 * Free a heap
 *
 * PARAMETERS
 * heap           A pointer to the heap to free
 * ------------------------------------------------------------------------- */
void heapFree(Heap* heap);

/* ------------------------------------------------------------------------- *
 * Allocate an array of HeapReference of a given size. The heap references
 * are not initialized.
 *
 * PARAMETERS
 * size            The size of the array
 *
 * RETURN
 * array           A pointer to the array, NULL on error. Must be freed after 
 *				   usage (with 'free()').
 * ------------------------------------------------------------------------- */
HeapReference* referenceArrayAlloc(size_t size);

/* ------------------------------------------------------------------------- *
 * Get a pointer to the heap reference at index $n$ in the heap references 
 * $array$.
 *
 * PARAMETERS
 * array          A pointer to the heap reference array
 * n              Index of the element to return
 *
 * RETURN
 * pointer        Pointer to the element at index $n$ in $array$.
 * ------------------------------------------------------------------------- */
HeapReference* referenceArrayIndex(HeapReference* array, size_t n);


#endif // HEAP_HPP
