#include <stddef.h>
#include <stdio.h>
#include <ctype.h>

#include "Signal.h"
#include "MedianQueue.h"

static const size_t BUFFER_SIZE = 512;


/** ------------------------------------------------------------------------ *
 * Whether c is part of a number
 * PARAMETERS
 * c    The character
 *
 * RETURN
 * true if the next character is a float
 * ------------------------------------------------------------------------- */
static inline bool isNumber(char c) {
    return c == '.' || isdigit(c);
}

/** ------------------------------------------------------------------------ *
 * From the given file handle, read the next sequence of bytes until a character that cannot
 * be a digit is met and place it in the buffer. The buffer should be large enough to contain the sequence.
 * The cursor of the file handle is moved after the next sep after the execution of the
 * function.
 *
 * PARAMETERS
 * f        The file handle
 * buffer   A large enough buffer to store the null terminated read sequence
 * value    A float pointer where to store the next number
 *
 * RETURN
 * false if an error occured
 * ------------------------------------------------------------------------- */
static bool nextNumber(FILE* f, char* buffer, double* value) {
    char c = getc(f);
    while (!isNumber(c) && c != EOF) {
        c = getc(f);
    }

    size_t buff_idx = 0;
    while(c != EOF && isNumber(c)) {
        buffer[buff_idx++] = c;
        c = getc(f);
    }

    buffer[buff_idx] = '\0'; // add null terminator
    return sscanf(buffer, "%lf", value) != EOF;
}

Signal* signalFromFile(const char* filepath) {
    char buffer[BUFFER_SIZE]; // buffer for storing temporary strings
    FILE* file = fopen(filepath, "r");
    if (!file) {
        return NULL;
    }

    // ---------------------- Read size of the signal ----------------------- //
    size_t signalSize = 0;
    if (!fgets(buffer, BUFFER_SIZE, file) ||
        sscanf(buffer, "%zu", &signalSize) == EOF) {
        fclose(file);
        return NULL;
    }

    // -------------------- Create and allocate signal ---------------------- //
    double* values = calloc(signalSize, sizeof(double));
    if (!values) {
        fclose(file);
        return NULL;
    }

    Signal* signal = malloc(sizeof(Signal));
    if (!signal) {
        fclose(file);
        free(values);
        return NULL;
    }

    signal->values = values;
    signal->size = signalSize;

    // ------------------------- Read the signal --------------------------- //
    double value;
    for (size_t i = 0; i < signalSize; ++i) {
        if(!nextNumber(file, buffer, &value)) {
            fclose(file);
            signalFree(signal);
        }
        signal->values[i] = value;
    }

    fclose(file);
    return signal;
}


bool signalToFile(const Signal* s, const char* filepath) {
    if (!s || !s->values || s->size == 0)
        return false;

    FILE* file = (filepath == NULL)? stdout : fopen(filepath, "w");
    if (!file)
        return false;

    // Writing the size
    fprintf(file, "%zu\n", s->size);

    // Writing values
    for (size_t i = 0; i < s->size; ++i){
        fprintf(file, "%lf ", s->values[i]);
    }
    fprintf(file, "\n");

    return true;

}


Signal* signalMedian(const Signal* s, size_t window) {
    if (!s || !s->values || s->size < window || window <= 1 || window % 2 == 0) {
        return NULL;
    }

    // Create median queue
    MedianQueue* mq = mqCreate(s->values, window);
    if (!mq) {
        return NULL;
    }

    // Initialize output signal
    size_t outputSize = s->size - window + 1;
    double* output = malloc(sizeof(double) * outputSize);
    if (!output) {
        mqFree(mq);
        return NULL;
    }

    // Compute median over signal
    for (size_t i = window; i < s->size; ++i) {
        output[i - window] = mqMedian(mq);
        mqUpdate(mq, s->values[i]);
    }
    output[outputSize - 1] = mqMedian(mq);
    
    mqFree(mq);

    // Create output signal
    Signal* outputSignal = malloc(sizeof(Signal));
    if (!outputSignal) {
        free(output);
        return NULL;
    }

    outputSignal->size = outputSize;
    outputSignal->values = output;
    return outputSignal;
}


void signalFree(Signal* s) {
    if(s) {
        free(s->values);
        free(s);
    }
}
