/* mem_utils.c utilities to support memory management system.
   Navya Mangipudi & Emma Tran 
   CSE 374 HW6
   06/1/23
*/

#include "mem.h"
#include "mem_impl.h"

// Checks the heap data structures to make sure they are uncorrupted
// Verifies heap conditions (block sizes and addresses)
void check_heap() {
  if (!freelist) return;
  freeNode* currentNode = freelist;
  uintptr_t minsize = currentNode->size;

  // Iterates through free list to check heap conditions
  while (currentNode != NULL) {
    if (currentNode->size < minsize) {
      minsize = currentNode->size;
    }
    if (currentNode->next != NULL) {
      // Checks current node's address is less than the next one's
      assert((uintptr_t)currentNode <(uintptr_t)(currentNode->next));
      assert((uintptr_t)currentNode + currentNode->size + NODESIZE
      <(uintptr_t)(currentNode->next));
    }
    currentNode = currentNode->next;
  }
  // Check if minimum size of block in free list is zero and print heap
  if (minsize == 0) print_heap( stdout);
  assert(minsize >= MINCHUNK);
}

// Retrieves memory statistics such as total size, total free memory,
// and number of free blocks
void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free,
  uintptr_t* n_free_blocks) {
  *total_size = totalmalloc;
  *total_free = 0;
  *n_free_blocks = 0;

  freeNode* currentNode = freelist;
  // Iterate through free lsit to calculate total free memory
  // and number of free blocks
  while (currentNode) {
    *n_free_blocks = *n_free_blocks + 1;
    *total_free = *total_free + (currentNode->size + NODESIZE);
    currentNode = currentNode->next;
  }
}

// Prints information about block in the free list.
// FILE *f is the pointer to the file where the heap information
// will be printed to
void print_heap(FILE *f) {
  printf("Printing the heap\n");
  freeNode* currentNode = freelist;
  // Iterate through free list and print information about each block
  while (currentNode !=NULL) {
    fprintf(f, "%" PRIuPTR, (uintptr_t)currentNode);
    fprintf(f, ", size: %" PRIuPTR, currentNode->size);
    fprintf(f, ", next: %" PRIuPTR, (uintptr_t)currentNode->next);
    fprintf(f, "\n");
    currentNode = currentNode->next;
  }
}

// Merges adjacent free blocks in the free list
void makeBlock() {
  freeNode* current = freelist;
  // Iterate through free list to merge adjacent blocks if possible
  while(current != NULL && current->next != NULL) {
    uintptr_t total = (uintptr_t)current + current->size + NODESIZE;
    if (total == (uintptr_t)current->next) {
      // Merge current and next block
        current->size += current->next->size + NODESIZE;
        current->next = current->next->next;
    } else {
      current = current->next;
    }
  }
}

// Inserts a free block into the free list at the needed position
void insertBlock(freeNode* block) {
  // If free lsit is empty, set block as head of free list
  if (freelist == NULL) {
    freelist = block;
    block->next = NULL;
    // If block should be inserted at beginning of free list, 
    // update the head
  } else if (block < freelist) {
    block->next = freelist;
    freelist = block;
  } else {
    freeNode* current = freelist;
    // Iterate through free lsit to find position to insert block
    while(current->next != NULL && current->next < block) {
      current = current->next;
    }
    block->next= current->next;
    current->next = block;
   }
}
