#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "Reduction.h"

static unsigned long computeError(const size_t* histogram, size_t begin, size_t end);
static uint16_t computeLevel(const size_t* histogram, size_t begin, size_t end);
static void sort(size_t* array, size_t length);

typedef struct interval_t {
	size_t begin, end;
	unsigned long error;
} Interval;

bool computeReduction(const size_t* histogram, size_t histogramLength, size_t nLevels, size_t* thresholds, uint16_t* levels) {
	if(!histogram || histogramLength == 0 || nLevels == 0 || !thresholds || !levels)
		return false;

	size_t begin = 0, end = histogramLength - 1, toReplace = 0, next = 1, middlePos, maxError;
	unsigned long errorLeft, errorRight;
	bool isUpdate;
	Interval* tabInterval = malloc(nLevels * sizeof(Interval));

	if(!tabInterval)
		return false;

	if(nLevels >= histogramLength) {
		for(size_t i = 0; i < nLevels; i++)
			thresholds[i] = i + 1;
	} else {
		thresholds[nLevels - 1] = histogramLength;

		for(size_t i = 0; i < nLevels - 1; i++) {
			middlePos = begin + (end - begin) / 2;

			thresholds[i] = middlePos;

			errorLeft = computeError(histogram, begin, middlePos);
			errorRight = computeError(histogram, middlePos + 1, end);

			tabInterval[toReplace].begin = begin;
			tabInterval[toReplace].end = middlePos;
			tabInterval[toReplace].error = errorLeft;

			tabInterval[next].begin = middlePos + 1;
			tabInterval[next].end = end;
			tabInterval[next].error = errorRight;

			next++;

			isUpdate = false;
			maxError = 0;

			for(size_t j = 0; j < nLevels; j++) {
				if(tabInterval[j].error > maxError) {
					maxError = tabInterval[j].error;
					toReplace = j;

					isUpdate = true;
				}
			}

			if(isUpdate) {
				begin = tabInterval[toReplace].begin;
				end = tabInterval[toReplace].end;
			} else {
				for(size_t j = 0; j < nLevels; j++) {
					if((tabInterval[j].end - tabInterval[j].begin) > 0) {
						begin = tabInterval[j].begin;
						end = tabInterval[j].end;

						toReplace = j;
					}
				}
			}
		}

		sort(thresholds, nLevels);
	}

	levels[0] = computeLevel(histogram, 0, thresholds[0]);

	for(size_t i = 1; i < nLevels; i++)
		levels[i] = computeLevel(histogram, thresholds[i - 1], thresholds[i]);

	return true;
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

static void sort(size_t* array, size_t length) {
	if(!array)
		return;

	int j;
	size_t tmp;

	for(size_t i = 0; i < length; i++) {
		tmp = array[i];
		j = i;

		while(j > 0 && array[j - 1] > tmp) {
			array[j] = array[j - 1];
			j--;
		}

		array[j] = tmp;
	}
}
