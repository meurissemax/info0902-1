#ifndef MEDIAN_QUEUE_H
#define MEDIAN_QUEUE_H

#include <stdlib.h>

/* ------------------------------------------------------------------------- *
 * Opaque structure
 * 
 * A median queue is a fixed-size FIFO structure that enables to compute 
 * efficiently the median of its stored elements. The number of elements 
 * is fixed at the construction of the object.
 * The structure MedianQueue should contain all necessary information for 
 * maintaining a median queue. 
 * ------------------------------------------------------------------------- */
typedef struct median_queue_t MedianQueue;

/* ------------------------------------------------------------------------- *
 * Create a median queue of $size$ elements initialized with the values 
 * stored in the array $values$.
 *
 * PARAMETERS
 * values         Array of size $size$ containing the initial values to store 
 *                in the median queue.
 * size           The number of elements in the median queue.
 *
 * NOTE
 * The returned structure should be cleaned with $mqFree$ after usage.
 *
 * RETURN
 * medianQueue    A pointer to the created median queue.
 * ------------------------------------------------------------------------- */
MedianQueue* mqCreate(const double* values, size_t size);

/* ------------------------------------------------------------------------- *
 * Free a median queue.
 *
 * PARAMETERS
 * mq             A pointer to the median queue to free.
 * ------------------------------------------------------------------------- */
void mqFree(MedianQueue* mq);

/* ------------------------------------------------------------------------- *
 * Replace the oldest value in the median queue by $value$.
 *
 * PARAMETERS
 * mq             A median queue object.
 * value          The double value to add to the structure.
 * ------------------------------------------------------------------------- */
void mqUpdate(MedianQueue* mq, double value);

/* ------------------------------------------------------------------------- *
 * Returns the median of the elements currently stored in the median queue. 
 *
 * PARAMETERS
 * mq             A median queue object.
 *
 * RETURNS
 * median         The median value. INFINITY on error.
 * ------------------------------------------------------------------------- */
double mqMedian(const MedianQueue* mq);

#endif // MEDIAN_QUEUE_H
