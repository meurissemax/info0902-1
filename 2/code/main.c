#include "Signal.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv) {
    // ------------------- Parse command line arguments --------------------- //
    if (argc != 3 && argc != 5) {
        fprintf(stderr, "Wrong number of parameters\n");
        fprintf(stderr, "Usage: %s [-o outputFile] inputFile width\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (argc == 5 && strcmp("-o", argv[1]) != 0) {
        fprintf(stderr, "Unrecognized parameter '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    const char* inputFile = (argc == 5)? argv[3]: argv[1];
    const char* outputFile = (argc == 5)? argv[2]: NULL;
    const char* windowWidthAsStr = (argc == 5)? argv[4]: argv[2];
    size_t windowWidth = 0;
    if (sscanf(windowWidthAsStr, "%zu", &windowWidth) != 1
        || windowWidth % 2 == 0) {
        fprintf(stderr, "Aborting; Width should be a odd "
                        "unsigned integer. Got '%s'.\n", windowWidthAsStr);
        return EXIT_FAILURE;
    }


    // ---------------------------- Load signal ----------------------------- //
	Signal* signal = signalFromFile(inputFile);
    if (!signal) {
        int errnum = errno;
        fprintf(stderr, "Could not parse the signal (%s)... Aborting.\n",
                (errnum==0)? "Unknown cause": strerror(errnum));
        return EXIT_FAILURE;
    }

    // --------------------------- Filter signal ---------------------------- //
	Signal* filtered = signalMedian(signal, windowWidth);
    if (!filtered) {
        int errnum = errno;
        fprintf(stderr, "Could not filter the signal (%s)... Aborting.\n",
                (errnum==0)? "Unknown cause": strerror(errnum));
        signalFree(signal);
        return EXIT_FAILURE;
    }


    // --------------------------- Write signal ---------------------------- //
    if (!signalToFile(filtered, outputFile)) {
        int errnum = errno;
        fprintf(stderr, "Could not write the signal (%s)... Aborting \n",
                (errnum==0)? "Unknown cause": strerror(errnum));
        signalFree(signal);
        signalFree(filtered);
        return EXIT_FAILURE;
    }

    signalFree(signal);
    signalFree(filtered);
	return EXIT_SUCCESS;
}
