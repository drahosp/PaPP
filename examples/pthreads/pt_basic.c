/*
 Basic use of the pthread library
 This example creates two threads, each thread prints a message
 obtained from a passed argument.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// struct used for returning data from threads:
typedef struct {
	char* message;
	int retval;
} ThreadData;

void *function(void *arg) {
	char *message = arg;
	printf("Hello from %s \n", message);

	// Allocate memory for thread data object and return its address:
	ThreadData* td = malloc(sizeof(ThreadData));
	td->message = message;
	td->retval = message[7] - '0' + 10;
	return (void*) td;
}

int main(void) {
	pthread_t thread1, thread2;
	char *message1 = "Thread 1";
	char *message2 = "Thread 2";

	// Create independent threads each of which will execute function:
	pthread_create(&thread1, NULL, &function, (void*) message1);
	pthread_create(&thread2, NULL, &function, (void*) message2);

	ThreadData *ret1, *ret2;

	// Wait for threads termination:
	pthread_join(thread1, (void**) &ret1); // casts ret1 to (void*) and assigns thread1's return value to it
	pthread_join(thread2, (void**) &ret2); // casts ret2 to (void*) and assigns thread2's return value to it

	// Main thread continues only after both threads have been terminated

	// print content of structures returned by threads:
	printf("%s returns: %d\n", ret1->message, ret1->retval);
	printf("%s returns: %d\n", ret2->message, ret2->retval);

	// Free memory that has been allocated by threads:
	free(ret1);
	free(ret2);

	// When the main thread calls exit, it terminates the whole process, which
	// causes all threads to terminate (even if they haven't finished yet).
	exit(EXIT_SUCCESS);
}
