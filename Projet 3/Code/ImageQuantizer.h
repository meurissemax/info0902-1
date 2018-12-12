/***********************************************************************
 * ImageQuantizer
 * Interface related to the quantization an image.
 ***********************************************************************/

#ifndef _IMAGE_QUANTIZER_H_
#define _IMAGE_QUANTIZER_H_

#include "PortableGrayMap.h"


/***********************************************************************
 * Quantize an image I in k levels of gray such that the quantized
 * image I* minimizes the squared error.
 * \sum_{i = 1}^height \sum_{j = 1}^width (I[i,j] - I*[i,j])^2
 *
 * This function does not affect the original image.
 *
 * PARAMETERS
 * image            - The image to quantize (with n levels)
 * numLevels        - The new number of gray levels (0 < k <= n)
 *
 * RETURN
 * NULL             - if any error
 * image            - The quantized image in k level
 ***********************************************************************/
PortableGrayMap* quantizeGrayImage(const PortableGrayMap* image,
                                   size_t numLevels);


#endif // !_IMAGE_QUANTIZER_H_

