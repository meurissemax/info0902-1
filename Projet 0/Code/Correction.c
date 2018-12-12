#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "Median.h"

typedef unsigned int uint;
static const size_t LENGTH = 101;

/* ----------------------------------------------------------------- *
 * Draw uniformely an integer in the range [min, max[
 *
 * ARGUMENTS
 * min          The minimum value (inclusive)
 * max          The maximum value (exclusive)
 * seed         A pointer to the seed (will be modified in place)
 *
 * RETURN
 * rand         An integer in the range [min, max[
 * ----------------------------------------------------------------- */
int drawInt(int, int, uint*);

/* ----------------------------------------------------------------- *
 * Swap the value pointed at if the condition is fulfilled
 *
 * ARGUMENTS
 * a            Pointer to the first value
 * b            Pointer to the second value
 * swap         Wether to swap the value
 * ----------------------------------------------------------------- */
void condSwap(int*, int*, bool);
void bubbleSort(int*, size_t);

static size_t drawInt(int min, int max, uint* seed) {
	*seed ^= (uint)(*seed << 13);
	*seed ^= (uint)(*seed >> 17);
	*seed ^= (uint)(*seed << 5);
	return min + (*seed % (max-min));
}

static void condSwap(int* a, int* b, bool swap) {
	if(!swap)
		return;
	int tmp = *b;
	*b = *a;
	*a = tmp;
}

static void bubbleSort(int* array, size_t length) {
	for(size_t i = 0; i < length; i++) {
		for(size_t j = length-1; j > i; j--) {
			condSwap(array+i, array+j-1, array[j]<array[j-1]);
		}
	}
}

int median() {
	uint seed = (uint)time(NULL);

	int* array = (int*) malloc(sizeof(int) * LENGTH);

	for(size_t i = 0; i < LENGTH; i++)
		array[i] = drawInt(-10, -5, &seed);

	bubbleSort(array, LENGTH);
	int med = array[LENGTH/2];
	free(array);
	return med;
}