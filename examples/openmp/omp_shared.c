/*
  OpenMP example program Hello World demonstrating shared variables
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
  
  int nthreads, tid;
  int x=0;   /* Initialize x to zero */

  /* Fork a team of threads with private copies of the variables nthreads and tid */
#pragma omp parallel shared(x) private(nthreads, tid) 
  {
    /* Get thread number */
    tid = omp_get_thread_num();
    printf("Hello World from thread = %d\n", tid);
    
    /* Only master thread does this */
    if (tid == 0) {  /* Thread zero sets x to 5 */
      x=5;
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d\n", nthreads);
    }
  }
#pragma omp parallel shared(x) private(tid) 
  {
    tid = omp_get_thread_num();
    printf("In thread %d x is %d\n", tid, x);
  }
  /* All threads join master thread */
  exit(0);
}

