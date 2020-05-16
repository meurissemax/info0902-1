/***********************************************************************
 * ImageQuantizerNaive
 * Implementation of a naive algorithm that quantizes an image.
 ***********************************************************************/

#include "ImageQuantizer.h"

PortableGrayMap* quantizeGrayImage(const PortableGrayMap* image, size_t numLevels)
{
  if (image == NULL || numLevels == 0)
    return NULL;

  PortableGrayMap* res = createEmptyImage(image->width, image->height, image->maxValue);
  if (res == NULL)
    return NULL;

  const double sizeInterval = (image->maxValue + 1) / (double)numLevels;
  const double halfSizeInterval = sizeInterval / 2.0;
  for (size_t i = 0; i < res->height; i++)
    for(size_t j = 0; j < res->width; j++)
      res->array[i][j] = (uint16_t)(image->array[i][j] / sizeInterval) * sizeInterval + halfSizeInterval;

  return res;
}

