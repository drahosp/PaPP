#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;

void *function() {
   pthread_mutex_lock( &mutex );
   counter++;
   printf("Counter value: %d\n",counter);
   pthread_mutex_unlock( &mutex );
   pthread_exit(0);
}

int main(void) {
   int rc1, rc2;
   pthread_t thread1, thread2;

   /* Create independent threads each of which will execute function */

   if( (rc1=pthread_create( &thread1, NULL, function, NULL)) ) {
      printf("Thread creation failed: %d\n", rc1);
   }

   if( (rc2=pthread_create( &thread2, NULL, function, NULL)) ) {
      printf("Thread creation failed: %d\n", rc2);
   }

   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */

   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL);

   exit(EXIT_SUCCESS);
}
