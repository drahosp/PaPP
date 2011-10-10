/*
 Condition variable example
 Synchronize threads using a range condition
 Thread 1 will count to COUNT_HALT1
 Thread 2 will count from COUNT_HALT1 to COUNT_HALT2
 Thread 1 will count to COUNT_DONE from there
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;        // condition mutex
pthread_cond_t  condition_cond  = PTHREAD_COND_INITIALIZER;         // condition
pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;        // count_mutex (protecting count variable)

int countFitsInterval = 0;                                          // boolean predicate
int countingIsDone = 0;

int  count = 0;                                                     // shared resource
#define COUNT_DONE  10
#define COUNT_HALT1  3
#define COUNT_HALT2  6

// Thread 1 Body
// Increase count according to condition
void *functionCount1()
{
	for(;;) {
		// While checking the condition use a mutex
		pthread_mutex_lock( &condition_mutex );                     // condition mutex must be locked before cond_wait is called

		//  Wait for the next signal
		while (countFitsInterval && !countingIsDone)                // never access count variable if count fits interval
		{
		    pthread_cond_signal( &condition_cond );                 // awakes thread2 so that boolean predicate can be changed (avoids deadlock)
		    pthread_cond_wait( &condition_cond, &condition_mutex ); // unlock condition_mutex and sleep, wait for signal
		}

		// Unlock the condition mutex
		pthread_mutex_unlock( &condition_mutex );

		if (countingIsDone)                                         // if another thread has finished counting already
		    pthread_exit(0);

		pthread_mutex_lock( &count_mutex );                         // we make sure that only this thread will access count variable

		// Increment counter and print its value
		// Only values < COUNT_HALT1 and > COUNT_HALT2 will be printed
		count++;
		printf("Counter value functionCount1: %d\n",count);

		if( count >= COUNT_HALT1 && count <= COUNT_HALT2 )
		    countFitsInterval = 1;                                  // sets the boolean predicate to 1 so that thread2 doesn't call cond_wait again

		// Exit if counting is done
		if (count >= COUNT_DONE)
		    countingIsDone = 1;                                     // we store this information so that we don't access count variable after unlocking count_mutex

		pthread_mutex_unlock( &count_mutex );                       // all threads can access 'count' variable after this call

		if (countingIsDone)
		{
		    pthread_cond_signal( &condition_cond );                 // awakes thread2 (signal that counting is done)
		    pthread_exit(0);
		}
	}
}

// Thread 2 Body
// Same as Thread 1 but with apropriate condition change
void *functionCount2() {
	for(;;) {
		// While checking the condition use a mutex
		pthread_mutex_lock( &condition_mutex );                     // condition mutex must be locked before cond_wait is called

		//  Wait for the next signal
		while (!countFitsInterval && !countingIsDone)               // never access count variable if count is out of interval
		{
		    pthread_cond_signal( &condition_cond );                 // awakes thread1 so that boolean predicate can be changed (avoids deadlock)
		    pthread_cond_wait( &condition_cond, &condition_mutex ); // unlock condition_mutex and sleep, wait for signal
		}

		// Unlock the condition mutex
		pthread_mutex_unlock( &condition_mutex );

		if (countingIsDone)                                         // if another thread has finished counting already
		    pthread_exit(0);

		pthread_mutex_lock( &count_mutex );                         // we make sure that only this thread will access count variable

		// Increnent counter and print its value
		// Only values < COUNT_HALT1 and > COUNT_HALT2 will be printed
		count++;
		printf("Counter value functionCount2: %d\n",count);

		if ( count > COUNT_HALT2 )                                  // if incrementing of counter "pushed" count out of interval
		    countFitsInterval = 0;                                  // sets boolean predicate back to 0 if condition is not met

		// Exit if counting is done
		if (count >= COUNT_DONE)
		    countingIsDone = 1;                                     // we store this information so that we don't access count variable after unlocking count_mutex

		pthread_mutex_unlock( &count_mutex );                       // all threads can access 'count' variable after this call

		if (countingIsDone)
		{
		    pthread_cond_signal( &condition_cond );                 // awakes thread1 (signal that counting is done)
		    pthread_exit(0);
		}
	}
}

// Spawn threads
int main(void) {
	pthread_t thread1, thread2;

	pthread_create( &thread1, NULL, functionCount1, NULL);
	pthread_create( &thread2, NULL, functionCount2, NULL);
	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);

	exit(EXIT_SUCCESS);
}
