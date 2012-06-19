/*
  This example demonstrates how to use pthread_join and how to pass results
  back from the treads. Threads are explicitly created as joinable. Each
  thread generates number between 0 and 500000 that is returned back to the
  main thread.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREADS 4

// Using rand() simultaneously in multiple threads leads to undefined behavior !
pthread_mutex_t randMutex = PTHREAD_MUTEX_INITIALIZER;

/*
   This is the start routine that all threads execute. It generates random
   number between 0 and 500000, stores it into dynamically allocated variable
   and returns pointer to memory, where this variable resides.
*/
void *work(void *t) {
  double *result = malloc(sizeof(double));
  int tid = (int)t; // get thread number

  // rand() is not thread-safe !!!
  pthread_mutex_lock(&randMutex);

  // seed to some thread-specific value (use thread id and addresses of local variables):
  srand(time(0) + (unsigned int)result - (unsigned int)&tid * 5 * (tid+1));
  *result = ((double)rand() / ((double)RAND_MAX + 1.0)) * 500000;

  pthread_mutex_unlock(&randMutex);

  /* printf("Thread %d done. Result = %7.2f\n",tid, *result); */
  return (void*) result;  /* Pass the pointer to result back */
}

int main(int argc, char *argv[]) {
  pthread_t thread[NUM_THREADS];
  pthread_attr_t attr;
  int rc, i;
  double res, total=0.0;
  void *status;

/* Initialize and set thread joinable attribute */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for(i=0;i<NUM_THREADS;i++) {
    //printf("Main: creating thread %d\n", i);
    rc = pthread_create(&thread[i], &attr, work, (void *)i);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

/* Free attribute and join with worker threads */
  pthread_attr_destroy(&attr);
  for(i=0; i<NUM_THREADS; i++) {
    rc = pthread_join(thread[i], &status);
    if (rc) {
      printf("ERROR return code from pthread_join() is %d\n", rc);
      exit(-1);
    }
    res = *(double *)status;
    total += res;
    printf("Joined with thread %d, result = %6.2f\n", i, res);
  }

  printf("\nProgram completed, total = %6.2f\n", total);
  exit(EXIT_SUCCESS);
}
