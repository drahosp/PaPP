/*
  OpenMP example program Hello World. The master thread forks a parallel region.
  All threads in the team obtain their thread number and print it.
  The master thread only prints the total number of threads.
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
  
  int nthreads, tid;

  /* Fork a team of threads giving them their own copies of variables */
#pragma omp parallel private(nthreads, tid)
  {
    /* Get thread number */
    tid = omp_get_thread_num();
    printf("Hello World from thread = %d\n", tid);
    
    /* Only master thread does this */
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d\n", nthreads);
    }
  }  /* All threads join master thread and disband */
  exit(0);
}

