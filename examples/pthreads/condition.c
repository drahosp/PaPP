#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
 Condition variable example
 Synchronize threads using a range condition
 Thread 1 will count to COUNT_HALT1
 Thread 2 will count from COUNT_HALT1 to COUNT_HALT2
 Thread 1 will count to COUNT_DONE from there
*/

pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_cond  = PTHREAD_COND_INITIALIZER;

int  count = 0;
#define COUNT_DONE  10
#define COUNT_HALT1  3
#define COUNT_HALT2  6

// Thread 1 Body
// Increase count according to condition
void *functionCount1()
{
    for(;;) {
        // While checking the condition use a mutex
        pthread_mutex_lock( &condition_mutex );

        //  Wait for the next signal
        if( count >= COUNT_HALT1 && count <= COUNT_HALT2 )
            pthread_cond_wait( &condition_cond, &condition_mutex );

        // Increnent counter and print its value
        // Only values < COUNT_HALT1 and > COUNT_HALT2 will be printed
        printf("Counter value functionCount1: %d\n",count);
        count++;

        // Signal another thread to check its condition
        pthread_cond_signal( &condition_cond);

        // Unlock the condition mutex
        pthread_mutex_unlock( &condition_mutex );

        // Exit if counting is done
        if (count >= COUNT_DONE)pthread_exit(0);
    }
}

// Thread 2 Body
// Same as Thread 1 but with apropriate condition change
void *functionCount2() {
    for(;;) {
        // While checking the condition use a mutex
        pthread_mutex_lock( &condition_mutex );

        //  Wait for the next signal
        if( count < COUNT_HALT1 || count > COUNT_HALT2 )
            pthread_cond_wait( &condition_cond, &condition_mutex );

        // Increnent counter and print its value
        // Only values < COUNT_HALT1 and > COUNT_HALT2 will be printed
        printf("Counter value functionCount2: %d\n",count);
        count++;

        // Signal another thread to check its condition
        pthread_cond_signal( &condition_cond);

        // Unlock the condition mutex
        pthread_mutex_unlock( &condition_mutex );

        // Exit if no longer needed
        if (count >= COUNT_HALT2) pthread_exit(0);
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
