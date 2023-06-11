/* getmem.c
   implements getmem (malloc) for memory system
   Navya Mangipudi & Emma Tran 
   CSE 374 HW6
   06/1/23
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "mem_impl.h"


freeNode* freelist = NULL; 
uintptr_t totalmalloc = 0; 

void* split(uintptr_t size, freeNode* node, freeNode* prev);

// Gets (allocates) a block of memory of the specified size
// Returns pointer to the allocated new block
void* getmem(uintptr_t size) {
    check_heap();
    // Returns NULL if the requested size is less or equal to zero
    if (size <= 0) {
        return NULL;
    }

    // Align the size to the nearest multiple of 16
    size = ((size + 15) / 16 ) * 16 + NODESIZE; 

    freeNode* current = freelist;
    freeNode* prev = NULL;
    while (current != NULL) {
        if (current->size >= size) {
            if (current->size > (size + NODESIZE + MINCHUNK)) {
                return split(size, current, prev);
            }
            if (prev == NULL) {
                freelist = current->next;
            } else {
                prev->next = current->next;
            }
            // Updates total allocated memory count, size, and pointer
            totalmalloc += current->size;
            current->size = size;
            return(void*)(current + 1);
        }
        prev = current;
        current = current->next;
    }
    check_heap();
    uintptr_t blocksize;
    if (size > BIGCHUNK) {
        blocksize = size + NODESIZE;
    } else {
        blocksize = BIGCHUNK;
    }
    
    // Allocates new block from system
    freeNode* newBlock = (freeNode*)malloc(blocksize);
    if (newBlock == NULL) {
        return NULL;
    }

    newBlock->size = blocksize;
    totalmalloc += size; 
    check_heap();
    // Returns a pointer to allocated block adjusting for the header size
    return (void*)(newBlock + 1);
}

// Splits a free block into two parts by adjusting pointers and inserting
// blocks into freelist. Returns updated pointer
void* split(uintptr_t size, freeNode* node, freeNode* prev) {
    freeNode* node2 = (freeNode*)((uintptr_t)node + NODESIZE + size);
    node2->size = node->size - size - NODESIZE;
    node->size = size;
    node2->next = node->next;
    if (prev == NULL) {
        freelist = node2;
    } else {
        prev->next = node2;
    }
    totalmalloc += size;
    return (void*)(node + 1);
}
