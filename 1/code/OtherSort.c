/* ========================================================================= *
 * OtherSort
 * Implementation of the OtherSort algorithm
 * ========================================================================= */

#include <stddef.h>
#include "Sort.h"

static void OtherSort(int* array, size_t first, size_t last) {
	size_t size, third;
	int tmp;

	size = last - first + 1;

	if(size > 1) {
		if(array[first] > array[last]) {
			tmp = array[last];
			array[last] = array[first];
			array[first] = tmp;
		}

		if(size > 2) {
			third = size / 3;

			OtherSort(array, first, last - third);
			OtherSort(array, first + third, last);
			OtherSort(array, first, last - third);
		}
	}
}

void sort(int* array, size_t length) {
	if(!array)
		return;

	if(length > 1) {
		size_t first, last;

		first = 0;
		last = length - 1;

		OtherSort(array, first, last);
	}
}
