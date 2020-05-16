/* ========================================================================= *
 * File parsing and Main Function

 * ------------------------------------------------------------------------- *
 * NOM
 *      quantizer
 * SYNOPSIS
 *      quantizer inputImg k outputName
 * DESCIRPTION
 *      Quantizes the input image on k levels and save it.
 * USAGE
 *      ./quantizer lena.pgm 4 lena_4.pgm
 *          Will compress the image lena.pgm on 4 levels and save it under
 *          the name "lena_4.pgm".
 * ------------------------------------------------------------------------- *
 * ========================================================================= */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "PortableGrayMap.h"
#include "ImageQuantizer.h"



int main(int argc, char** argv)
{
    // Checking arguments
    if (argc != 4)
    {
        /*
         * argv[0]: name of the executable
         * argv[1]: name of the input file
         * argv[2]: number of levels
         * argv[3]: name of the output file
         */
        fprintf(stderr, "Usage: %s <PGM input image> <unsgined int> "
                        "<PGM output name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parsing arguments
    size_t nbLevels = 0;
    if(sscanf(argv[2], "%zu", &nbLevels) != 1)
    {
        fprintf(stderr, "Aborting; number of levels should be unsigned int. "
                        "Got '%s'.\n", argv[2]);
        return EXIT_FAILURE;
    }

    // Loading input Image
    PortableGrayMap* inputImg = createImageFromFile(argv[1]);
    if(!inputImg)
    {
        fprintf(stderr, "Aborting; error while loading input image '%s'\n",
                argv[1]);
        return EXIT_FAILURE;
    }

    // Quantizing
    PortableGrayMap* outputImg = quantizeGrayImage(inputImg, nbLevels);
    if(!outputImg)
    {
        fprintf(stderr, "Aborting; error while computing the reduction\n");
        deleteImage(inputImg);
        return EXIT_FAILURE;
    }

    // Computing the error
    unsigned long error = 0;
    long errComp = 0;
    for (size_t i = 0; i < inputImg->height; i++)
    {
        for(size_t j = 0; j < inputImg->width; j++)
        {
            errComp = (long)(inputImg->array[i][j]) - \
                      (long)(outputImg->array[i][j]);
            error += (unsigned long)(errComp*errComp);
        }
    }
    fprintf(stdout, "Compression error: %lu\n", error);


    // Saving output image
    if(saveImageToFile(outputImg, argv[3]) != 0)
    {
        fprintf(stderr, "Aborting; error while saving output image in '%s'\n",
                argv[3]);
        deleteImage(inputImg);
        deleteImage(outputImg);
        return EXIT_FAILURE;
    }

    deleteImage(inputImg);
    deleteImage(outputImg);
    return EXIT_SUCCESS;
}
