/*
 Basic use of the pthread library
 This example creates two threads, each thread prints a message
 obtained from a passed argument.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *function( void *ptr ) {
	char *message;
	message = (char *) ptr;
	printf("Hello from %s \n", message);
	pthread_exit(0);
}

int main(void) {
	pthread_t thread1, thread2;
	char *message1 = "Thread 1";
	char *message2 = "Thread 2";
	int  iret1, iret2;

	/* Create independent threads each of which will execute function */

	iret1 = pthread_create( &thread1, NULL, &function, (void*) message1);
	iret2 = pthread_create( &thread2, NULL, &function, (void*) message2);

	/* Wait until threads are complete before main continues. Unless we */
	/* wait we run the risk of executing an exit which will terminate   */
	/* the process and all threads before the threads have completed.   */

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);

	printf("Thread 1 returns: %d\n",iret1);
	printf("Thread 2 returns: %d\n",iret2);
	exit(EXIT_SUCCESS);
}
