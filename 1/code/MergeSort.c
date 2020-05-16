/* ========================================================================= *
 * MergeSort
 * Implementation of the MergeSort algorithm
 * ========================================================================= */

#include <stddef.h>
#include <math.h>
#include <limits.h>
#include "Sort.h"

static void Merge(int* array, size_t first, size_t posMedian, size_t last) {
	size_t subLast1, subLast2, i, j;

	subLast1 = posMedian - first + 1;
	subLast2 = last - posMedian;

	int subTab1[subLast1+1], subTab2[subLast2+1];

	for(i = 0; i < subLast1; i++) {
		subTab1[i] = array[first+i];
	}

	for(j = 0; j < subLast2; j++) {
		subTab2[j] = array[posMedian+j+1];
	}

	subTab1[subLast1] = INT_MAX;
	subTab2[subLast2] = INT_MAX;

	i = 0;
	j = 0;

	for(size_t k = first; k <= last; k++) {
		if(subTab1[i] <= subTab2[j] && (i < subLast1 || j < subLast2)) {
			array[k] = subTab1[i];
			i++;
		} else {
			array[k] = subTab2[j];
			j++;
		}
	}
}

static void MergeSort(int* array, size_t first, size_t last) {
	if(first < last) {
		size_t posMedian;

		posMedian = floor((first+last)/2);

		MergeSort(array, first, posMedian);
		MergeSort(array, posMedian+1, last);
		Merge(array, first, posMedian, last);
	}
}

void sort(int* array, size_t length) {
	if(!array)
		return;

	if(length > 1) {
		size_t first, last;

		first = 0;
		last = length - 1;

		MergeSort(array, first, last);
	}
}
