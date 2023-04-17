/* block.c: Block Structure */

#include "malloc/block.h"
#include "malloc/counters.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Allocate a new block on the heap using sbrk:
 * @param   size    Number of bytes to allocate.
 * @return  Pointer to data portion of newly allocate block.
 **/
Block* block_allocate(size_t size) {
    // Allocate block
    intptr_t allocated = sizeof(Block) + ALIGN(size);
    Block* block = sbrk(allocated);
    if (block == SBRK_FAILURE) {
        return NULL;
    }

    // Record block information
    block->capacity = ALIGN(size);
    block->size     = size;
    block->prev     = block;
    block->next     = block;

    // Update counters
    Counters[HEAP_SIZE] += allocated;
    Counters[BLOCKS]++;
    Counters[GROWS]++;
    return block;
}

/**
 * Attempt to release memory used by block to heap:
 * @param   block   Pointer to block to release.
 * @return  Whether or not the release completed successfully.
 **/
bool block_release(Block *block) {
    if (!block) return false;

    //convert heap and block ptrs to intptrs for comparison
    intptr_t heap_pointer = (intptr_t)sbrk(0); 
    intptr_t end = (intptr_t)block + sizeof(Block) + block->capacity;

    if (end == heap_pointer && block->capacity >= TRIM_THRESHOLD){ 
        Counters[SHRINKS]++;
        Counters[BLOCKS]--;
        Counters[HEAP_SIZE] -= block->capacity + sizeof(Block);
        
        //update heap pointer and return true
        sbrk(-1 * (block->capacity + sizeof(Block)));
        return true;
    }
    return false;
}

/**
 * Detach specified block from its neighbors.
 *
 * @param   block   Pointer to block to detach.
 * @return  Pointer to detached block.
 **/
Block* block_detach(Block *block) {
    if (block){
        Block *prev = block->prev;
        Block *next = block->next;
        prev->next = block->next;
        next->prev = block->prev;
        block->next = block;
        block->prev = block;
    }

    return block;
}

/**
 * Attempt to merge source block into destination.
 * @param   dst     Destination block we are merging into.
 * @param   src     Source block we are merging from.
 * @return  Whether or not the merge completed successfully.
 **/
bool block_merge(Block *dst, Block *src) {
    //convert end of dst and start of src to inptrs for comparion    
    intptr_t end = (intptr_t)dst + sizeof(Block) + dst->capacity;
    intptr_t source = (intptr_t)src;

    if (end == source){
        block_detach(src);
        dst->capacity = dst->capacity + sizeof(Block) + src->capacity;
        Counters[MERGES]++;
        Counters[BLOCKS]--;
        return true;
    }

    return false;
}

/**
 * Attempt to split block with the specified size:
 * @param   block   Pointer to block to split into two separate blocks.
 * @param   size    Desired size of the first block after split.
 * @return  Pointer to original block (regardless if it was split or not).
 **/
Block* block_split(Block *block, size_t size) {
    size_t capacity = ALIGN(size);
    //original capacity must have room for header and aligned size
    if (block->capacity <= sizeof(Block) + capacity){
        block->size = size;
        return block;
    }

    //create new block to be inserted in free list
    Block *new = (Block *)((intptr_t)block + sizeof(Block) + capacity);

    //insert new block into freelist and update capacity/size
    new->next = block->next;
    new->prev = block;
    new->capacity = block->capacity - capacity - sizeof(Block);
    new->size = 0;

    //change pointer relations of block after split
    block->next->prev = new;
    block->next = new;
    block->capacity = capacity;
    block->size = size;
    
    Counters[SPLITS]++;
    Counters[BLOCKS]++;
    return block;
}
