/* counters.c: Counters */

#include "malloc/block.h"
#include "malloc/counters.h"
#include "malloc/freelist.h"

#include <assert.h>

extern Block FreeList;
size_t Counters[NCOUNTERS] = {0};
int    DumpFD              = -1;

/**
 * Initialize counters by doing the following:
 *  1. Register the dump_counters function to run when the program terminates.
 *  2. Duplicate standard output file descriptor to the DumpFD global variable.
 **/
void init_counters() {
    static bool initialized = false;
    if (!initialized) {
        assert(atexit(dump_counters) == 0);
        initialized = true;
        DumpFD      = dup(STDOUT_FILENO);
        assert(DumpFD >= 0);
    }
}

/**
 * Compute internal fragmentation in heap using the formula:
 *  FRAGMENTATION = Sum(internal fragments) / HeapSize * 100.0
 * @return  Percentage of internal fragmentation in heap.
 **/
double internal_fragmentation() {
    size_t frag = 0;
    float sz = (float)Counters[HEAP_SIZE];
    if(sz == 0) return 0;
    for(Block* curr = FreeList.next; curr != &FreeList; curr = curr->next){
        frag = frag + curr->capacity - curr->size;
    }
    return frag/sz * 100.0;   
}

/**
 * Compute external fragmentation in heap using the formula:
 *  FRAGMENTATION = (1 - (LARGEST_FREE_BLOCK / ALL_FREE_MEMORY)) * 100.0
 * @return  Percentage of external fragmentation in heap.
 **/
double external_fragmentation() {
    float biggest = 0;
    float  total = 0;
    for(Block* curr = FreeList.next; curr != &FreeList; curr = curr->next){
        total = total + curr->capacity;
        if(curr->capacity > biggest){
            biggest = curr->capacity;
        }
    }
    if (total == 0) return 0;
    return (1 - (biggest / total)) * 100.0;
}

/**
 * Display all counters to the DumpFD global file descriptor saved in
 * init_counters.
**/
void dump_counters() {
    char buffer[BUFSIZ];
    assert(DumpFD >= 0);

    fdprintf(DumpFD, buffer, "blocks:      %lu\n"   , Counters[BLOCKS]);
    fdprintf(DumpFD, buffer, "free blocks: %lu\n"   , free_list_length());
    fdprintf(DumpFD, buffer, "mallocs:     %lu\n"   , Counters[MALLOCS]);
    fdprintf(DumpFD, buffer, "frees:       %lu\n"   , Counters[FREES]);
    fdprintf(DumpFD, buffer, "callocs:     %lu\n"   , Counters[CALLOCS]);
    fdprintf(DumpFD, buffer, "reallocs:    %lu\n"   , Counters[REALLOCS]);
    fdprintf(DumpFD, buffer, "reuses:      %lu\n"   , Counters[REUSES]);
    fdprintf(DumpFD, buffer, "grows:       %lu\n"   , Counters[GROWS]);
    fdprintf(DumpFD, buffer, "shrinks:     %lu\n"   , Counters[SHRINKS]);
    fdprintf(DumpFD, buffer, "splits:      %lu\n"   , Counters[SPLITS]);
    fdprintf(DumpFD, buffer, "merges:      %lu\n"   , Counters[MERGES]);
    fdprintf(DumpFD, buffer, "requested:   %lu\n"   , Counters[REQUESTED]);
    fdprintf(DumpFD, buffer, "heap size:   %lu\n"   , Counters[HEAP_SIZE]);
    fdprintf(DumpFD, buffer, "internal:    %4.2lf\n", internal_fragmentation());
    fdprintf(DumpFD, buffer, "external:    %4.2lf\n", external_fragmentation());

    close(DumpFD);
}
