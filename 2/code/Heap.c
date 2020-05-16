#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "Heap.h"

struct heap_t {
	bool type;
	size_t size;
	size_t nbElements;
	size_t* indexes;
	double* heapArray;
};

struct heap_ref_t {
	size_t index;
};

static void swap(Heap* heap, double* a, double* b) {
	if(!heap)
		return;

	size_t posA = 0, posB = 0;

	size_t tmpIndex;
	double tmpValue;

	for(size_t i = 0; i < heap->nbElements; i++) {
		if(heap->heapArray[heap->indexes[i]] == *a)
			posA = i;

		if(heap->heapArray[heap->indexes[i]] == *b)
			posB = i;
	}

	tmpIndex = heap->indexes[posB];
	heap->indexes[posB] = heap->indexes[posA];
	heap->indexes[posA] = tmpIndex;

	tmpValue = *b;
	*b = *a;
	*a = tmpValue;
}

static void maxHeapify(Heap* heap, size_t node, size_t heapSize) {
	size_t left, right, largest;

	left = 2*node+1;
	right = 2*node+2;

	if(left <= heapSize && heap->heapArray[left] > heap->heapArray[node])
		largest = left;
	else
		largest = node;

	if(right <= heapSize && heap->heapArray[right] > heap->heapArray[largest])
		largest = right;

	if(largest != node) {
		swap(heap, &heap->heapArray[node], &heap->heapArray[largest]);
		maxHeapify(heap, node, heapSize);
	}
}

static void minHeapify(Heap* heap, size_t node, size_t heapSize) {
	size_t left, right, smallest;

	left = 2*node+1;
	right = 2*node+2;

	if(left <= heapSize && heap->heapArray[left] < heap->heapArray[node])
		smallest = left;
	else
		smallest = node;

	if(right <= heapSize && heap->heapArray[right] < heap->heapArray[smallest])
		smallest = right;

	if(smallest != node) {
		swap(heap, &heap->heapArray[node], &heap->heapArray[smallest]);
		minHeapify(heap, node, heapSize);
	}
}

static bool heapExtends(Heap* heap) {
	size_t newSize = heap->size*2;

	size_t* indexes = malloc(newSize*sizeof(size_t));

	if(!indexes)
		return false;

	double* heapArray = malloc(newSize*sizeof(double));

	if(!heapArray) {
		free(indexes);

		return false;
	}

	for(size_t i = 0; i < newSize; i++) {
		indexes[i] = heap->indexes[i];
		heapArray[i] = heap->heapArray[i];
	}

	heap->indexes = indexes;
	heap->heapArray = heapArray;
	heap->size = newSize;

	return true;
}

Heap* heapCreate(bool maxHeap) {
	Heap* heap = malloc(sizeof(Heap));

	if(!heap)
		return NULL;

	heap->type = maxHeap;
	heap->size = 100;
	heap->nbElements = 0;
	heap->indexes = malloc(heap->size*sizeof(size_t));

	if(!heap->indexes) {
		free(heap);

		return NULL;
	}

	heap->heapArray = malloc(heap->size*sizeof(double));

	if(!heap->heapArray) {
		free(heap->heapArray);
		free(heap);

		return NULL;
	}

	for(size_t i = 0; i < heap->size; i++) {
		heap->indexes[i] = i;
		heap->heapArray[i] = i;
	}

	return heap;
}

size_t heapSize(const Heap* heap) {
	if(!heap)
		return 0;

	return heap->nbElements;
}

double heapGet(const Heap* heap, const HeapReference* reference) {
	if(!heap)
		return 0;

	if(!reference)
		return 0;

	return heap->heapArray[heap->indexes[reference->index]];
}

double heapTop(const Heap* heap) {
	if(!heap)
		return 0;

	return heap->heapArray[0];
}

HeapReference* heapTopReference(const Heap* heap) {
	if(!heap)
		return NULL;

	HeapReference* reference = malloc(sizeof(HeapReference));

	if(!reference)
		return NULL;

	for(size_t i = 0; i < heap->size; i++) {
		if(heap->indexes[i] == 0) {
			reference->index = i;
		} else {
			free(reference);

			return NULL;
		}
	}

	return reference;
}

bool heapAdd(Heap* heap, double value, HeapReference* dest) {
	if(!heap)
		return false;

	if(!dest)
		return false;

	if(heap->nbElements == heap->size)
		if(!heapExtends(heap))
			return false;

	size_t pos = heap->nbElements;

	heap->indexes[pos] = pos;
	heap->heapArray[pos] = value;

	dest->index = pos;

	heap->nbElements++;

	if(heap->type) {
		while(pos > 0 && heap->heapArray[(pos-1)/2] < heap->heapArray[pos]) {
			swap(heap, &heap->heapArray[(pos-1)/2], &heap->heapArray[pos]);

			pos = (pos-1)/2;
		}
	} else {
		while(pos > 0 && heap->heapArray[(pos-1)/2] > heap->heapArray[pos]) {
			swap(heap, &heap->heapArray[(pos-1)/2], &heap->heapArray[pos]);

			pos = (pos-1)/2;
		}
	}

	return true;
}

void heapReplace(Heap* heap, HeapReference* toReplace, double value) {
	if(!heap)
		return;

	if(!toReplace)
		return;

	size_t pos = toReplace->index;

	heap->heapArray[heap->indexes[pos]] = value;

	for(int i = (heap->nbElements-1)/2; i >= 0; i--) {
		if(heap->type)
			maxHeapify(heap, i, heap->nbElements-1);
		else
			minHeapify(heap, i, heap->nbElements-1);
	}
}

void heapFree(Heap* heap) {
	if(!heap)
		return;

	free(heap->indexes);
	free(heap->heapArray);
	free(heap);
}

HeapReference* referenceArrayAlloc(size_t size) {
	HeapReference* referenceArray = malloc(size*sizeof(HeapReference));

	if(!referenceArray)
		return NULL;

	for(size_t i = 0; i < size; i++)
		referenceArray[i].index = i;

	return referenceArray;
}

HeapReference* referenceArrayIndex(HeapReference* array, size_t n) {
	if(!array)
		return NULL;

	return &array[n];
}
