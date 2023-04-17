/* block.h: Block Structure */

#ifndef BLOCK_H
#define BLOCK_H

#include "malloc/block.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


#define ALIGNMENT       (sizeof(double))
#define ALIGN(size)     (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define SBRK_FAILURE    ((void *)(-1))
#define TRIM_THRESHOLD  (1<<10)


typedef struct block Block;
struct block {
    size_t   capacity;	/* Number of bytes allocated to block (aligned) */
    size_t   size;	/* Number of bytes used by block */
    Block *  prev;	/* Pointer to previous block structure */
    Block *  next;	/* Pointer to next block structure */
    char     data[];	/* Label for user accessible block data */
};


#define BLOCK_FROM_POINTER(ptr) \
    (Block *)((intptr_t)(ptr) - sizeof(Block))



Block * block_allocate(size_t size);
bool    block_release(Block *block);

Block * block_detach(Block *block);

bool    block_merge(Block *dst, Block *src);
Block * block_split(Block *block, size_t size);

#endif