#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "ImageQuantizer.h"
#include "Reduction.h"

PortableGrayMap* quantizeGrayImage(const PortableGrayMap* image, size_t numLevels) {
	if(!image || numLevels == 0)
		return NULL;

	PortableGrayMap* compressedImg = createEmptyImage(image->width, image->height, image->maxValue);

	if(!compressedImg)
		return NULL;

	size_t* thresholds = malloc(numLevels * sizeof(size_t));

	if(!thresholds) {
		deleteImage(compressedImg);

		return NULL;
	}

	uint16_t* levels = malloc(numLevels * sizeof(uint16_t));

	if(!levels) {
		free(thresholds);
		deleteImage(compressedImg);

		return NULL;
	}

	size_t histogramLength = image->maxValue + 1, histogram[histogramLength];

	for(size_t i = 0; i < histogramLength; i++)
		histogram[i] = 0;

	for(size_t i = 0; i < image->height; i++)
		for(size_t j = 0; j < image->width; j++)
			histogram[image->array[i][j]]++;

	if(computeReduction(histogram, histogramLength, numLevels, thresholds, levels)) {
		for(size_t i = 0; i < image->height; i++) {
			for(size_t j = 0; j < image->width; j++) {
				if(thresholds[0] > image->array[i][j])
					compressedImg->array[i][j] = levels[0];

				if(thresholds[numLevels - 2] <= image->array[i][j])
					compressedImg->array[i][j] = levels[numLevels - 1];

				for(size_t k = 1; k < numLevels - 1; k++)
					if(thresholds[k - 1] <= image->array[i][j] && thresholds[k] > image->array[i][j])
						compressedImg->array[i][j] = levels[k];
			}
		}
	} else {
		free(levels);
		free(thresholds);
		deleteImage(compressedImg);

		return NULL;
	}

	return compressedImg;
}
