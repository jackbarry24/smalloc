/* counters.h: Acounting */

#ifndef COUNTERS_H
#define COUNTERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define fdprintf(fd, b, s, ...) \
    sprintf(b, s, ##__VA_ARGS__); write(fd, b, strlen(b));

enum {
    BLOCKS,	    /* Number of all blocks */
    MALLOCS,	    /* Number of successful calls to malloc */
    FREES,	    /* Number of successful calls to free */
    REALLOCS,	    /* Number of successful calls to realloc */
    CALLOCS,	    /* Number of successful calls to callocs */
    REUSES,	    /* Number of times a block was reused */
    GROWS,	    /* Number of times the heap was grown */
    SHRINKS,        /* Number of times the heap was shrunk */
    SPLITS,	    /* Number of times a block was split */
    MERGES,	    /* Number of times a block was merged */
    REQUESTED,	    /* Total number of bytes requested by user */
    HEAP_SIZE,	    /* Size of the heap */
    NCOUNTERS,	    /* Number of counters */
};

extern size_t Counters[NCOUNTERS];  /* Counters array */

void init_counters();
void dump_counters();

#endif
