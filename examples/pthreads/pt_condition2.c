/*
 Condition variable example 2
 Two threads will count to MAX_COUNT by using same variable.
 Thread 1 will increase this variable only if it's odd.
 Thread 2 will increase it only if it's even.
 Synchronize threads using a condition variable.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;    // mutex protecting 'count' variable being accessed concurrently
/*
   To ensure that you do not lose wake up operations on condition variables, you
   should always use a boolean predicate and a mutex with the condition variable.
*/
pthread_cond_t odd_cond = PTHREAD_COND_INITIALIZER;         // condition variable
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;     // mutex connected with condition
int countIsOdd = 0;                                         // boolean predicate connected with condition

int count = 0;                                              // shared resource, critical section should be protected by count_mutex

#define MAX_COUNT 100

void* oddCounter(void* arg)
{
    int thread_should_exit = 0;
    int ID = (int)arg;

    for (;;)
    {

        pthread_mutex_lock(&cond_mutex);                    // lock condition mutex (this MUST be done before pthread_cond_wait is called!)
        while (!countIsOdd)
        {
            pthread_cond_wait(&odd_cond, &cond_mutex);      // wait for condition (unlock condition mutex and sleep)
        }
        pthread_mutex_unlock(&cond_mutex);                  // unlock condition mutex

        pthread_mutex_lock( &count_mutex );                 // only this thread will access 'count' variable after this call
        if (count < MAX_COUNT)
        {
            ++count;                                        // oddCounter increments count variable only if its value is odd
            printf("Thread %d: %d\n", ID, count);
            countIsOdd = 0;                                 // odd number increased by one is even
        }
        if (count >= MAX_COUNT)                             // using "else" here would cause dead lock, condition count could have changed already
        {
            thread_should_exit = 1;
        }
        pthread_mutex_unlock( &count_mutex );               // all threads can lock count mutex (so that they can access 'count' variable)

        if (thread_should_exit)                             // 'count' variable should not be accessed outside of "safe zone" protected by count mutex
            pthread_exit(0);
    }
}

void* evenCounter(void* arg)
{
    int thread_should_exit = 0;
    int ID = (int)arg;
	
    for (;;)
    {
        pthread_mutex_lock( &count_mutex );                 // only this thread will access 'count' variable after this call
        if (count < MAX_COUNT)
        {
            if (!countIsOdd)                                // make sure that only even numbers get incremented by this thread
            {
                ++count;
                printf("Thread %d: %d\n", ID, count);
                countIsOdd = 1;                             // even number increased by one is odd
                pthread_cond_signal(&odd_cond);             // wake up oddCounter to continue, this thread will fall into sleep when it tries to lock count mutex
            }
        }
        if (count >= MAX_COUNT)                             // using "else" here would cause dead lock, condition count could have changed already
        {
            thread_should_exit = 1;
        }
        pthread_mutex_unlock( &count_mutex );               // all threads can lock count mutex (so that they can access 'count' variable)

        if (thread_should_exit)                             // 'count' variable should not be accessed outside of "safe zone" protected by count mutex
            pthread_exit(0);
    }
}

int main(void) {
   pthread_t thread1, thread2;

   pthread_create( &thread1, NULL, oddCounter, (void *)1);
   pthread_create( &thread2, NULL, evenCounter, (void *)2);
   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL);

   exit(EXIT_SUCCESS);
}
