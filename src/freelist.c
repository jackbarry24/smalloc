/* freelist.c: Free List Implementation
 *
 * The FreeList is an unordered doubly-linked circular list containing all the
 * available memory allocations (memory that has been previous allocated and
 * can be re-used).
 **/

#include "malloc/counters.h"
#include "malloc/freelist.h"
#include <limits.h>

Block FreeList = {-1, -1, &FreeList, &FreeList};

/**
 * Search for an existing block in free list with at least the specified size
 * using the first fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block* free_list_search_ff(size_t size) {
    // TODO: Implement first fit algorithm
    for(Block* curr = FreeList.next; curr != &FreeList; curr = curr->next){
        if (curr->capacity >= size) return curr;
    }
    return NULL;
}

/**
 * Search for an existing block in free list with at least the specified size
 * using the best fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block* free_list_search_bf(size_t size) {
    // TODO: Implement best fit algorithm
    ssize_t best = INT_MAX;
    ssize_t current = 0;
    Block* best_block = NULL;
    for(Block* curr = FreeList.next; curr != &FreeList; curr = curr->next){
        if(curr->capacity >= size){
            current = curr->capacity - size;
            if (current < best){
                best = current;
                best_block = curr;
            }
        }
    }
    return best_block;
}

/**
 * Search for an existing block in free list with at least the specified size
 * using the worst fit algorithm.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block* free_list_search_wf(size_t size) {
    // TODO: Implement worst fit algorithm
    ssize_t worst = INT_MIN;
    ssize_t current = 0;
    Block* worst_block = NULL;
    for(Block* curr = FreeList.next; curr != &FreeList; curr = curr->next){
        if(curr->capacity >= size){
            current = curr->capacity - size;
            if (current > worst){
                worst = current;
                worst_block = curr;
            }
        }
    }
    return worst_block;
}

/**
 * Search for an existing block in free list with at least the specified size.
 * @param   size    Amount of memory required.
 * @return  Pointer to existing block (otherwise NULL if none are available).
 **/
Block* free_list_search(size_t size) {
    Block * block = NULL;
#if	defined FIT && FIT == 0
    block = free_list_search_ff(size);
#elif	defined FIT && FIT == 1
    block = free_list_search_wf(size);
#elif	defined FIT && FIT == 2
    block = free_list_search_bf(size);
#endif

    if (block) {
        Counters[REUSES]++;
    }
    return block;
}

/**
 * Insert specified block into free list.
 * @param   block   Pointer to block to insert into free list.
 **/
void	free_list_insert(Block *block) {
    // TODO: Implement free list insertion
    intptr_t block_int = (intptr_t)block;
    Block* curr;
    intptr_t curr_int;
 
    for(curr = FreeList.next; curr != &FreeList; curr = curr->next){
        curr_int = (intptr_t)curr;
        if(curr_int >= block_int){
            break;
        }
    }

    //insert block into freelist by updating ptr relations
    block->next = curr;
    block->prev = curr->prev;
    curr->prev->next = block;
    curr->prev = block;

    block_merge(block, block->next);
    block_merge(block->prev, block);

    return;
}

/**
 * Return length of free list.
 * @return  Length of the free list.
 **/
size_t  free_list_length() {
    // TODO: Implement free list length
    size_t length = 0;
    for(Block *curr = FreeList.next; curr != &FreeList; curr = curr->next){
        length++;
    }
    return length; 
}
