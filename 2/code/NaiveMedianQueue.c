#include <stdlib.h>
#include <math.h>

#include "MedianQueue.h"



struct median_queue_t {
	double* circular;
	double* sorted;
	size_t size;
	size_t start;
};

/* ------------------------------------------------------------------------- *
 * Double comparison function.
 * ------------------------------------------------------------------------- */
static int compareDouble(const void* d1, const void* d2);
static int compareDouble(const void* d1, const void* d2) {
	double diff = (*(double*) d2 - *(double*) d1);
	return diff > 0 ? -1 : (diff < 0 ? 1 : 0);
}

MedianQueue* mqCreate(const double* values, size_t size) {
	// create the median queue
	MedianQueue* mq = malloc(sizeof(MedianQueue));
	if (!mq)
		return NULL;

	mq->size = size;
	mq->start = 0;

	// allocate circular array
	mq->circular = malloc(sizeof(double) * mq->size);
	if (!mq->circular) {
		free(mq);
		return NULL;
	}

	// allocated sorted array
	mq->sorted = malloc(sizeof(double) * mq->size);
	if (!mq->circular) {
		free(mq->circular);
		free(mq);
		return NULL;
	}

	for (size_t i = 0; i < mq->size; ++i) {
		mq->circular[i] = values[i];
		mq->sorted[i] = values[i];
	}

	qsort(mq->sorted, mq->size, sizeof(double), compareDouble);

	return mq;
}

void mqFree(MedianQueue* mq) {
	free(mq->sorted);
	free(mq->circular);
	free(mq);
}

void mqUpdate(MedianQueue* mq, double value) {
	if (!mq) { return; }
	mq->circular[mq->start] = value;
	mq->start = (mq->start + 1) % mq->size;
	for (size_t i = 0; i < mq->size; ++i) {
		mq->sorted[i] = mq->circular[(mq->start + i) % mq->size];
	}
	qsort(mq->sorted, mq->size, sizeof(double), compareDouble);
}

double mqMedian(const MedianQueue* mq) {
	if (!mq) { return INFINITY; }
	return mq->sorted[mq->size / 2];
}
