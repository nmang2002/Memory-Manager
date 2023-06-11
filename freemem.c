/* freemem.c
   implements freemem (free) for memory system
   Navya Mangipudi & Emma Tran 
   CSE 374 HW6
   06/1/23
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"
#include <stdlib.h>

// Frees memory pointed to by the given pointer p.
// void*p is the pointer to the memory block being deallocated
void freemem(void* p) {
    check_heap();
    if (p == NULL) {
        return;
    }
    // Calculate starting address of the free block and set next 
    // pointer of free block to null
    freeNode* sizePtr = (freeNode*)((uintptr_t)p - NODESIZE); 
    sizePtr->next = NULL;
    insertBlock(sizePtr);
    makeBlock();
    check_heap();
}
