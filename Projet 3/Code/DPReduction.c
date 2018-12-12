#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "Reduction.h"

static unsigned long errMin(size_t n, size_t k, unsigned long* error, size_t* intervals, const size_t* histogram, size_t histogramLength);
static size_t getIndex(size_t row, size_t column, size_t rowLength);
static unsigned long computeError(const size_t* histogram, size_t begin, size_t end);
static uint16_t computeLevel(const size_t* histogram, size_t begin, size_t end);

bool computeReduction(const size_t* histogram, size_t histogramLength, size_t nLevels, size_t* thresholds, uint16_t* levels) {
	if(!histogram || histogramLength == 0 || nLevels == 0 || !thresholds || !levels)
		return false;

	if(nLevels >= histogramLength) {
		for(size_t i = 0; i < nLevels; i++)
			thresholds[i] = i + 1;
	} else {
		size_t tabSize = histogramLength * nLevels, last;

		unsigned long* error = malloc(tabSize * sizeof(unsigned long));

		if(!error)
			return NULL;

		size_t* intervals = malloc(tabSize * sizeof(size_t));

		if(!intervals) {
			free(error);

			return NULL;
		}

		for(size_t i = 0; i < tabSize; i++) {
			error[i] = LONG_MAX;
			intervals[i] = UINT_MAX;
		}

		errMin(histogramLength - 1, nLevels - 1, error, intervals, histogram, nLevels);

		thresholds[0] = 0;

		last = histogramLength - 1;

		for(int i = nLevels - 1; i > 0; i--) {
			last -= intervals[getIndex(last, i, nLevels)];

			thresholds[i] = last + 1;
		}

		for(size_t i = 0; i < nLevels - 1; i++)
			thresholds[i] = thresholds[i + 1];

		thresholds[nLevels - 1] = histogramLength;
	}

	levels[0] = computeLevel(histogram, 0, thresholds[0]);

	for(size_t i = 1; i < nLevels; i++)
		levels[i] = computeLevel(histogram, thresholds[i - 1], thresholds[i]);

	return true;
}

static unsigned long errMin(size_t n, size_t k, unsigned long* error, size_t* intervals, const size_t* histogram, size_t nLevels) {
	unsigned long err = 0, minErr = LONG_MAX;
	size_t index = getIndex(n, k, nLevels);

	if(k == 0) {
		error[index] = computeError(histogram, 0, n);
		intervals[index] = n;

		return error[index];
	}

	if(k == n) {
		error[index] = 0;

		return error[index];
	}

	if(error[index] != LONG_MAX)
		return error[index];

	for(size_t i = n - 1; i > k - 1; i--) {
		err = computeError(histogram, i, n) + errMin(i, k - 1, error, intervals, histogram, nLevels);

		if(err < minErr) {
			minErr = err;
			intervals[index] = n - i + 1;
		}
	}

	error[index] = minErr;

	return minErr;
}

static size_t getIndex(size_t row, size_t column, size_t rowLength) {
	return column + (rowLength * row);
}

static unsigned long computeError(const size_t* histogram, size_t begin, size_t end) {
	if(!histogram || begin >= end)
		return 0;

	unsigned long error = 0;
	uint16_t level = computeLevel(histogram, begin, end);

	for(size_t i = begin; i < end; i++)
		error += histogram[i] * (i - level) * (i - level);

	return error;
}

static uint16_t computeLevel(const size_t* histogram, size_t begin, size_t end) {
	if(!histogram || begin >= end)
		return begin;

	size_t num = 0, denom = 0, level;

	for(size_t i = begin; i < end; i++) {
		num += histogram[i] * i;
		denom += histogram[i];
	}

	if(denom == 0)
		level = begin;
	else
		level = round((double)num / (double)denom);

	return level;
}
