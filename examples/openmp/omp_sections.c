/*
  OpenMP example program demonstrating the use of the sections construct
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N     50

int main (int argc, char *argv[]) {
  int i, nthreads, tid;
  float a[N], b[N], c[N], d[N];
  
  /* Some initializations */
  for (i=0; i<N; i++) {
    a[i] = i * 1.5;
    b[i] = i + 42.0;
    c[i] = d[i] = 0.0;
  }
  
#pragma omp parallel shared(a,b,c,d,nthreads) private(i,tid)
  {
    tid = omp_get_thread_num();
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d\n", nthreads);
    }
    printf("Thread %d starting...\n",tid);
    
#pragma omp sections nowait
    {
#pragma omp section
      {
	printf("Thread %d doing section 1\n",tid);
	for (i=0; i<N; i++) {
	  c[i] = a[i] + b[i];
	}
      } /* End of first section */
      
#pragma omp section
      {
	printf("Thread %d doing section 2\n",tid);
	for (i=0; i<N; i++) {
	  d[i] = a[i] * b[i];
	}
      } /* End of second section */
      
    }  /* end of sections */
    
    printf("Thread %d done.\n",tid); 

  }  /* end of parallel section */

  /* Print the results */
  printf("c:  ");
  for (i=0; i<N; i++) {
    printf("%.2f ", c[i]);
  }
  
  printf("\n\nd:  ");
  for (i=0; i<N; i++) {
    printf("%.2f ", d[i]);
  }
  printf("\n");

  exit(0);
}
