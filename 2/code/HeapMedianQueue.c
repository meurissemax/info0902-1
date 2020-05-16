#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "MedianQueue.h"
#include "Heap.h"

struct median_queue_t {
	double* circular;
	double* sorted;
	size_t size;
	size_t start;
	Heap* maxHeap;
	Heap* minHeap;
	HeapReference* referenceMaxHeap;
	HeapReference* referenceMinHeap;
};

/* ------------------------------------------------------------------------- *
 * Double comparison function.
 * ------------------------------------------------------------------------- */
static int compareDouble(const void* d1, const void* d2);

static int compareDouble(const void* d1, const void* d2) {
	double diff = (*(double*) d2 - *(double*) d1);

	return diff > 0 ? -1 : (diff < 0 ? 1 : 0);
}

// Modified function
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

	mq->maxHeap = heapCreate(true);

	if(!mq->maxHeap) {
		free(mq->sorted);
		free(mq->circular);
		free(mq);

		return NULL;
	}

	mq->minHeap = heapCreate(false);

	if(!mq->minHeap) {
		free(mq->maxHeap);
		free(mq->sorted);
		free(mq->circular);
		free(mq);

		return NULL;
	}

	mq->referenceMaxHeap = referenceArrayAlloc((mq->size/2)+1);

	if(!mq->referenceMaxHeap) {
		free(mq->minHeap);
		free(mq->maxHeap);
		free(mq->sorted);
		free(mq->circular);
		free(mq);

		return NULL;
	}

	mq->referenceMinHeap = referenceArrayAlloc(mq->size/2);

	if(!mq->referenceMinHeap) {
		free(mq->referenceMaxHeap);
		free(mq->minHeap);
		free(mq->maxHeap);
		free(mq->sorted);
		free(mq->circular);
		free(mq);

		return NULL;
	}

	for(size_t i = 0; i <= mq->size/2; i++) {
		if(!heapAdd(mq->maxHeap, mq->sorted[i], referenceArrayIndex(mq->referenceMaxHeap, i))) {
			mqFree(mq);

			return NULL;
		}
	}

	size_t j = 0;

	for(size_t i = mq->size/2+1; i < mq->size; i++) {
		if(heapAdd(mq->minHeap, mq->sorted[i], referenceArrayIndex(mq->referenceMinHeap, j))) {
			j++;
		} else {
			mqFree(mq);

			return NULL;
		}
	}

	return mq;
}

void mqFree(MedianQueue* mq) {
	heapFree(mq->maxHeap);
	heapFree(mq->minHeap);

	free(mq->sorted);
	free(mq->circular);
	free(mq);
}

void mqUpdate(MedianQueue* mq, double value) {
	if(!mq)
		return;

	double valueToReplace = mq->circular[mq->start];
	bool done = false;

	for(size_t i = 0; i < heapSize(mq->maxHeap); i++) {
		if(heapGet(mq->maxHeap, referenceArrayIndex(mq->referenceMaxHeap, i)) == valueToReplace) {
			heapReplace(mq->maxHeap, referenceArrayIndex(mq->referenceMaxHeap, i), value);

			done = true;
			break;
		}
	}

	if(!done) {
		for(size_t i = 0; i < heapSize(mq->minHeap); i++) {
			if(heapGet(mq->minHeap, referenceArrayIndex(mq->referenceMinHeap, i)) == valueToReplace) {
				heapReplace(mq->minHeap, referenceArrayIndex(mq->referenceMinHeap, i), value);

				break;
			}
		}
	}

	double topMax = heapTop(mq->maxHeap), topMin = heapTop(mq->minHeap);

	if(topMax > topMin) {
		size_t k = 0, m = 0;

		while(heapGet(mq->maxHeap, referenceArrayIndex(mq->referenceMaxHeap, k)) != topMax)
			k++;

		while(heapGet(mq->minHeap, referenceArrayIndex(mq->referenceMinHeap, m)) != topMin)
			m++;

		heapReplace(mq->maxHeap, referenceArrayIndex(mq->referenceMaxHeap, k), topMin);
		heapReplace(mq->minHeap, referenceArrayIndex(mq->referenceMinHeap, m), topMax);
	}

	mq->circular[mq->start] = value;
	mq->start = (mq->start+1)%mq->size;
}

double mqMedian(const MedianQueue* mq) {
	if(!mq)
		return INFINITY;

	return heapTop(mq->maxHeap);
}
