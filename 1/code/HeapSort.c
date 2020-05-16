/* ========================================================================= *
 * HeapSort
 * Implementation of the HeapSort algorithm
 * ========================================================================= */

#include <stddef.h>
#include <math.h>
#include "Sort.h"

static void Swap(int* a, int* b) {
	int tmp;

	tmp = *b;
	*b = *a;
	*a = tmp;
}

static void MaxHeapify(int* array, size_t node, size_t heapSize) {
	size_t left, right, largest;

	left = 2*node + 1;
	right = 2*node + 2;

	if(left <= heapSize && array[left] > array[node])
		largest = left;
	else
		largest = node;

	if(right <= heapSize && array[right] > array[largest])
		largest = right;

	if(largest != node){
		Swap(&array[node], &array[largest]);
		MaxHeapify(array, largest, heapSize);
	}
}

static void BuildMaxHeap(int* array, size_t last) {
	size_t heapSize;

	heapSize = last;

	for(int i = floor(last/2); i >= 0; i--) {
		MaxHeapify(array, i, heapSize);
	}
}

static void HeapSort(int* array, size_t length) {
	size_t last;

	last = length - 1;

	if(last > 0) {
		size_t heapSize;

		heapSize = last;

		BuildMaxHeap(array, heapSize);

		for(size_t i = last; i > 0; i--) {
			Swap(&array[i], &array[0]);
			heapSize--;
			MaxHeapify(array, 0, heapSize);
		}
	}
}

void sort(int* array, size_t length) {
	if(!array)
		return;

	if(length > 1) {
		HeapSort(array, length);
	}
}
