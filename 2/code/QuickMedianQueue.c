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
 * Added functions.
 * ----------------- */
static void swap(double* a, double* b) {
	double tmp;

	tmp = *b;
	*b = *a;
	*a = tmp;
}

static size_t partition(double* array, size_t first, size_t last) {
	double pivot;
	int i;

	pivot = array[last];
	i = first - 1;

	for(size_t j = first; j < last; j++)
		if(array[j] <= pivot)
			swap(&array[++i], &array[j]);

	swap(&array[i+1], &array[last]);

	return i+1;
}

static void adaptedQuicksort(MedianQueue* mq, size_t first, size_t last) {
	if(first < last) {
		size_t posPivot, posMedian;

		posPivot = partition(mq->sorted, first, last);
		posMedian = mq->size/2;

		if(posPivot != posMedian) {
			if(posPivot > posMedian)
				adaptedQuicksort(mq, first, posPivot-1);
			else
				adaptedQuicksort(mq, posPivot+1, last);
		}
	}
}

/* ----------------- *
 * Basic functions.
 * ----------------- */
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

	adaptedQuicksort(mq, 0, mq->size-1);

	return mq;
}

void mqFree(MedianQueue* mq) {
	free(mq->sorted);
	free(mq->circular);
	free(mq);
}

void mqUpdate(MedianQueue* mq, double value) {
	if(!mq)
		return;

	mq->circular[mq->start] = value;
	mq->start = (mq->start+1)%mq->size;

	for(size_t i = 0; i < mq->size; i++)
		mq->sorted[i] = mq->circular[(mq->start+i)%mq->size];

	adaptedQuicksort(mq, 0, mq->size-1);
}

double mqMedian(const MedianQueue* mq) {
	if(!mq)
		return INFINITY;

	return mq->sorted[mq->size/2];
}
