#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lz4_link_stream.h"
#include "page_rank.h"

int 
main(int argc, char **argv) {
     PageRank *pr;
     if (page_rank_new(&pr, "./test_pr", 1000000, 0.85) != 0)
	  return -1;

     printf("Initialized PageRank\n");

     LZ4LinkStream *link_stream;
     if (lz4_link_stream_new(&link_stream, argv[1]) != 0)
	  return -1;
	  
     clock_t t = clock();
     if (page_rank_compute(pr, link_stream, lz4_link_stream_next, lz4_link_stream_reset) != 0)
	  return -1;
     t = clock() - t;
     printf("PageRank loop: %.2f\n", ((float)t)/CLOCKS_PER_SEC/30.0);
     
#ifdef PRINT_RESULTS // Print results 
     for (size_t i=0; i<pr->n_pages; ++i) {
	  printf("% 12zu PR: %.3e\n", i, *(float*)mmap_array_idx(pr->value1, i));
     }
#endif

     page_rank_delete(pr);
     return 0;
}