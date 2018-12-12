#include <stdlib.h>
#include <math.h>

#include "MedianQueue.h"

struct median_queue_t {
	double* circular;
	double* sorted;
	size_t size;
	size_t start;
};

/* ----------------- *
 * Added functions
 * ----------------- */
static void swap(double* a, double* b) {
	double tmp;

	tmp = *b;
	*b = *a;
	*a = tmp;
}

/* ----------------- *
 * Basic functions
 * ----------------- */
// Double comparison function
static int compareDouble(const void* d1, const void* d2);

static int compareDouble(const void* d1, const void* d2) {
	double diff = (*(double*) d2 - *(double*) d1);

	return diff > 0 ? -1 : (diff < 0 ? 1 : 0);
}

MedianQueue* mqCreate(const double* values, size_t size) {
	// create the median queue
	MedianQueue* mq = malloc(sizeof(MedianQueue));

	if(!mq)
		return NULL;

	mq->size = size;
	mq->start = 0;

	// allocate circular array
	mq->circular = malloc(sizeof(double)*mq->size);

	if(!mq->circular) {
		free(mq);

		return NULL;
	}

	// allocated sorted array
	mq->sorted = malloc(sizeof(double)*mq->size);

	if(!mq->sorted) {
		free(mq->circular);
		free(mq);

		return NULL;
	}

	for(size_t i = 0; i < mq->size; i++) {
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

// Modified function
void mqUpdate(MedianQueue* mq, double value) {
	if(!mq)
		return;

	size_t i = 0;

	while(mq->sorted[i] != mq->circular[mq->start])
		i++;

	mq->sorted[i] = value;
	mq->circular[mq->start] = value;
	mq->start = (mq->start+1)%mq->size;

	if(i > 0)
		if(mq->sorted[i-1] > mq->sorted[i])
			for(size_t j = i; j > 0; j--)
				if(mq->sorted[j-1] > mq->sorted[j])
					swap(&mq->sorted[j-1], &mq->sorted[j]);

	if(i < mq->size-1)
		if(mq->sorted[i+1] < mq->sorted[i])
			for(size_t j = i; j < mq->size-1; j++)
				if(mq->sorted[j+1] < mq->sorted[j])
					swap(&mq->sorted[j+1], &mq->sorted[j]);
}

double mqMedian(const MedianQueue* mq) {
	if(!mq)
		return INFINITY;

	return mq->sorted[mq->size/2];
}
