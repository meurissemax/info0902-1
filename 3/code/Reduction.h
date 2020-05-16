/***********************************************************************
 * ImageQuantizer
 * Interface related to the quantization an image.
 ***********************************************************************/

#ifndef _REDUCTION_H_
#define _REDUCTION_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


/***********************************************************************
 * Given an histogram h of size n, where n is the number of gray levels,
 * this function computes k thresholds (p_1, ..., p_{k-1}, p_k = n) and
 * k levels (v_1, ... v_k), with k <= n, such that the resulting mapping
 * function g(i) (tries to) minimize(s) the squared error
 * \sum_{i=0}^{n-1} h[i](i-g(i))^2.
 *
 * PARAMETERS
 * histogram          The histogram vector (h)
 * histogramLength    Size of the histogram vector (n)
 * nLevels            The number of levels after compression (k)
 * thresholds         An allocated vector of size k where the computed
 *                    thresholds (p_1, ..., p_k) will be stored
 * levels             An allocated vector of size k where the computed levels
 *                    (v_1, ..., v_k) will be stored
 *
 * RETURN
 * wentFine           A boolean stating whether no error occured
 ***********************************************************************/
bool computeReduction(const size_t* histogram, size_t histogramLength,
                      size_t nLevels, size_t* thresholds, uint16_t* levels);

#endif // !_REDUCTION_H_

