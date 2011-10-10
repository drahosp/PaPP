/*
  OpenMP example program demonstrating threadprivate variables
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int a, b, i, x, y, tid;
#pragma omp threadprivate(a,x)  /* a and x are threadprivate */


int main (int argc, char *argv[]) {

  /* Initialize the variables */
  a = b = x = y = 0;

  /* Fork a team of threads */
#pragma omp parallel private(b, tid) 
  {
    tid = omp_get_thread_num();
    a = b = tid;     /* a and b gets the value of the thread id */
    x = tid+10;      /* x is 10 plus the value of the thread id */
  }
  
  /* This section is now executed serially */
  for (i=0; i< 1000; i++) {
    y += i;
  }

  /* Fork a new team of threads */
#pragma omp parallel private(tid) 
  {
    tid = omp_get_thread_num();
    /* The variables a and x will keep their values */
    printf("Thread %d:  a = %d  b = %d  x = %d \n", tid, a, b, x);
  }  /* All threads join master thread */

  exit(0);
}

