/* bench.c is the benchmarking /test program for mem memory management
   bench.c is implemented as part of HW6, CSE374 22WI
   Navya Mangipudi & Emma Tran 
   CSE 374 HW6
   06/1/23
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "mem.h"

void print_stats(clock_t start);
void fill_mem(void* ptr, uintptr_t size);

// Initialize the parameters
int main(int argc, char** argv) {
   int NTRIALS = 10000;
   int PCTGET = 50;
   int PCTLARGE = 10;
   int SMALL_L = 200;
   int LARGE_L = 20000;
   unsigned int random_seed = (unsigned int)time(NULL);

   /* Sets the above values based on the input arguments
      if there is not a corresponding input argument,
      default value in the spec is used. */
   if (argc > 1) {
      NTRIALS = atoi(argv[1]);
   }
   if (argc > 2) {
      PCTGET = atoi(argv[2]);
   }
   if (argc > 3) {
      PCTLARGE = atoi(argv[3]);
   }
   if (argc > 4) {
      SMALL_L = atoi(argv[4]);
   }
   if (argc > 5) {
      LARGE_L = atoi(argv[5]);
   }
   if (argc > 6) {
      random_seed = atoi(argv[6]);
   }

   // Prints the configuration parameters for the benchmark test
   printf("Running bench for %d trials, %d%% getmem calls.\n", NTRIALS, PCTGET);

   int size, count = 0;
   // Array to store memory adress
   void** mem_array = malloc(NTRIALS * sizeof(void*));

   if (!mem_array) {
      printf("Malloc failed for mem_array");
      return EXIT_FAILURE;
   }

   srand(random_seed);
   clock_t start = clock();
   clock_t prev_elapsed = 0;

   // Print progress every 10% of trials
   int progress_interval = NTRIALS / 5;

   // Perform NTRIALS mem operations
   // Perform memory allocation and deallocation with random
   // numbers generated below
   for (int i = 1; i <= NTRIALS; i++) {
      // Random number generated from 1 to 100
      int random_number = rand() % 100 + 1;

      // finds part of memory
      if (random_number <= PCTGET) {
         // random size of block
         int size_number = rand() % 100 + 1;

         // Large block:
         if (size_number <= PCTLARGE) {
            // Generate a random size for a large allocation
            size = rand() % (LARGE_L - SMALL_L + 1) + SMALL_L;
         // Small block:
         } else {
            // Generate a random size for a small allocation
            size = rand() % SMALL_L + 1;
         }
         // Allocate memory by using the getmem function
         void* mem = getmem(size);

         if (mem) {
            // Store the memory address in the array
            mem_array[count] = mem;
            count++;
            // Fill the allocated memory with hexadecimal values
            fill_mem(mem, size);
         }
      } else {
         if (count != 0) {
            int index = rand() % count;
            // Free the randomly selected previously allocated memory block
            freemem(mem_array[index]);
            // Move the last memory address in the array to the freed position
            mem_array[index] = mem_array[count-1];
            count--;
         }
      }

      if (i % progress_interval == 0) {
         clock_t elapsed = clock() - start;
         double progress = (double)i / NTRIALS * 100;
         double time_taken = (double)(elapsed - prev_elapsed) / CLOCKS_PER_SEC;
         prev_elapsed = elapsed;

         printf("Progress: %.1f%% (Elapsed time: %.3f seconds)\n", progress, time_taken);
         print_stats(start);
      }
   }

   free(mem_array);
   return EXIT_SUCCESS;
}

/* print_stats is a helper function that prints performance
   information about the memory system. Requires a clock_t start
   time, and prints elapsed time plus results from get_mem_stats():
   total amount of storage in bytes acquired by the memory manager,
   total amount of storage in bytes currently stored on the
   free list, and total number of individual blocks currently
   on the free list.
*/
void print_stats(clock_t start) {
   clock_t elapsed = clock() - start;
   size_t total_size, total_free, n_free_blocks;
   // Retrieve memory statistics
   get_mem_stats(&total_size, &total_free, &n_free_blocks);

   printf("Total CPU time used by benchmark test: %.3f seconds\n", (double)elapsed / CLOCKS_PER_SEC);
   printf("Total amount of storage acquired: %lu bytes\n", total_size);
   printf("Total amount of storage on the free list: %lu bytes\n", total_free);
   printf("Total number of blocks on the free storage list: %lu\n", n_free_blocks);
   printf("Average number of bytes in the free storage blocks: %.2f\n", (double)total_free / n_free_blocks);
}

/* fill_mem is a helper function that takes in an address and the
   size of the memory allocation at that address. It should fill
   the first 16 bytes (or up to size) with hexadecimal values.
*/
void fill_mem(void* ptr, uintptr_t size) {
   unsigned char* cast_ptr = (unsigned char*)ptr;
   for (uintptr_t i = 0; i < ((size < 16) ? size : 16); i++) {
      // Fill the memory with hexadecimal value 0xFE
      cast_ptr[i] = 0xFE;
   }
}
