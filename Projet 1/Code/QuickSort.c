/* ========================================================================= *
 * QuickSort
 * Implementation of the QuickSort algorithm
 * ========================================================================= */

#include <stddef.h>
#include "Sort.h"

static void Swap(int* a, int* b) {
	int tmp;

	tmp = *b;
	*b = *a;
	*a = tmp;
}

static int Partition(int* array, int first, int last) {
	int pivot, i;

	pivot = array[last];
	i = first - 1;

	for(int j = first; j < last; j++) {
		if(array[j] <= pivot) {
			Swap(&array[++i], &array[j]);
		}
	}

	Swap(&array[i + 1], &array[last]);

	return i + 1;
}

static void QuickSort(int* array, int first, int last) {
	if(first < last) {
		int posPivot;

		posPivot = Partition(array, first, last);

		QuickSort(array, first, posPivot-1);
		QuickSort(array, posPivot+1, last);
	}
}

void sort(int* array, size_t length) {
	if(!array)
		return;

	if(length > 1) {
		int first, last;

		first = 0;
		last = length - 1;

		QuickSort(array, first, last);
	}
}
