#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Array.h"
#include "Sort.h"

/* Prototypes */

/* ------------------------------------------------------------------------- *
 * Compute the CPU time (in seconds) used by the Sort function.
 *
 * PARAMETERS
 * array        Array to sort
 * length       Number of elements in the array
 *
 * RETURN
 * seconds      The number of seconds used by Sort
 * ------------------------------------------------------------------------- */
static double cpuTimeUsedToSort(int* array, size_t length)
{
    clock_t start = clock();
    sort(array, length);
    return ((double) (clock() - start)) / CLOCKS_PER_SEC;
}



/* ------------------------------------------------------------------------- *
 * Main
 * ------------------------------------------------------------------------- */
int main(void)
{
    srand(time(NULL));//Use an integer seed to get a fix sequence

    size_t sizeArray = 10000;
    printf("Size of the array: %d\n", (int) sizeArray);
    int* array = createRandomArray(sizeArray);

    const double sec = cpuTimeUsedToSort(array, sizeArray);
    printf("CPU Time: %f\n", sec);

    free(array);

    return 0;
}
