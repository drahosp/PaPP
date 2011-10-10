/*
  OpenMP example program demonstrating the use of the num_threads clause
  The master thread forks a parallel region with T threads.
  The value of T is given as an argument on the command line with the -t switch
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_usage(char* name) {
  printf("Usage: %s -t <nr of threads>\n", name);
  exit(0);
}

int main (int argc, char *argv[]) {
  
  int nthreads, tid;
  int T=2;   /* Default number of threads */
  char c;

  /* Check that we have at least one argument */
  if (argc <=1 ) {
    print_usage(argv[0]);
  }
  else {
    /* Get the value of T from the command line */
    while ((c=getopt(argc, argv, "ht:")) != EOF) {
      switch (c) {
      case 'h':
	print_usage(argv[0]);
      case 't':
	T = atoi(optarg);    /* Get number of threads  */
	break;
      default: 
	print_usage(argv[0]);
      }
    }
  }
  
  /* Fork a team of T threads */
#pragma omp parallel private(nthreads, tid) num_threads(T)
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
