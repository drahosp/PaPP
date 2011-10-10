/*
  This example demonstrates how to explicitly create a thread in a
  detached state. This might be done to conserve some system resources
  if the thread never needs to join later. Note that we don't pass any
  results back from the threads, for that we need to synchronize the
  theads with the main program.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREADS	4
#define ITERATIONS 1000000

void *BusyWork(void *t) {
  int i, tid;
  double result=0.0;
  tid = (int)t;
  printf("Thread %d starting...\n",tid);
  for (i=0; i<ITERATIONS; i++) {
    result += (double)rand()/RAND_MAX;
  }
  printf("Thread %d done. Result = %6.2f\n",tid, result);
}

int main(int argc, char *argv[]) {
  pthread_t thread[NUM_THREADS];
  pthread_attr_t attr;
  int rc, t;

  /* Initialize and set thread detached attribute */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  for(t=0;t<NUM_THREADS;t++) {
    printf("Main: creating thread %d\n", t);
    rc = pthread_create(&thread[t], &attr, BusyWork, (void *)t);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

  /* We're done with the attribute object, so we can destroy it */
  pthread_attr_destroy(&attr);

  /* The main thread is done, so we need to call pthread_exit explicitly to
   *  permit the working threads to continue even after main completes.
   */
  printf("Main: program completed. Exiting.\n");
  pthread_exit(NULL);
}

