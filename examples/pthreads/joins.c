#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NTHREADS 10
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;

void *function(void *null) {
   pthread_mutex_lock( &mutex1 );
   counter++;
   pthread_mutex_unlock( &mutex1 );
   pthread_exit(0);
}

int main(void) {
   pthread_t thread_id[NTHREADS];
   int i, j;

   for(i=0; i < NTHREADS; i++) {
      pthread_create( &thread_id[i], NULL, function, NULL );
   }

   for(j=0; j < NTHREADS; j++) {
      pthread_join( thread_id[j], NULL);
   }

   /* Now that all threads are complete I can print the final result.     */
   /* Without the join I could be printing a value before all the threads */
   /* have been completed.                                                */

   printf("Final counter value: %d\n", counter);
   exit(EXIT_SUCCESS);
}

