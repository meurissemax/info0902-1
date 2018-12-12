#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- *
 * Structure
 *
 * A structure for storing a floating point signal.
 * ------------------------------------------------------------------------- */
typedef struct signal_t {
    double* values;
    size_t size;
} Signal;

/* ------------------------------------------------------------------------- *
 * Create a signal from a file.
 *
 * PARAMETERS
 * filepath       Path to the file containing the signal.
 *
 * NOTE
 * The returned structure should be cleaned with $signalFree$ after usage.
 *
 * RETURN
 * signal         A pointer to the created signal.
 * ------------------------------------------------------------------------- */
Signal* signalFromFile(const char* filepath);


/* ------------------------------------------------------------------------- *
 * Write the signal to a file or print it on the standard output.
 *
 * PARAMETERS
 * s              A valid pointer to the signal.
 * filepath       Path to the file containing the signal or NULL.
 *
 *
 * RETURN
 * Wehther the signal was correctly written
 * ------------------------------------------------------------------------- */
bool signalToFile(const Signal* s, const char* filepath);

/* ------------------------------------------------------------------------- *
 * Free a signal.
 *
 * PARAMETERS
 * signal         A valid pointer to the signal to free or NULL.
 * ------------------------------------------------------------------------- */
void signalFree(Signal* s);

/* ------------------------------------------------------------------------- *
 * Apply a median filter with window $window$ over the signal $signal$.
 *
 * PARAMETERS
 * signal         A valid pointer to the signal.
 * window         Size of the median window.
 *
 * RETURNS
 * signal         The signal resulting from the median filtering.
 * ------------------------------------------------------------------------- */
Signal* signalMedian(const Signal* s, size_t window);

#endif // SIGNAL_H
