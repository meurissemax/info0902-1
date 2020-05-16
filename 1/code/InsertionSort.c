/* ========================================================================= *
 * InsertionSort
 * Implementation of the InsertionSort algorithm.
 * ========================================================================= */

#include <stddef.h>
#include "Sort.h"


void sort(int* array, size_t length)
{
    if (!array)
        return;

    int j;
    int tmp;
    for (size_t i = 0; i < length; i++)
    {
        tmp = array[i];
        j = i;
        while(j > 0 && array[j-1] > tmp)
        {
            array[j] = array[j-1];
            j--;
        }
        array[j]=tmp;
    }
}
