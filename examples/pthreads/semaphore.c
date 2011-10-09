#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// EXAMPLE: Semaphore
// Semaphore controlled count to 2*N
// 2 Threads increment a counter

#define N 1000
#define MAXCOUNT 2*N
#define SEMINITVALUE 1

int count = 0;
sem_t sem;                      // semaphore protecting 'count' variable being accessed concurrently

void *function()
{
    int i;
    for (i = 0; i < N; i++)     // 2 threads count to 2*N, each thread counts to N
    {
        sem_wait(&sem);         /* If semaphore value is 0, it suspends the calling thread until the semaphore value is incremented.
                                   When sem_wait() function is "finished" the semaphore value is decremented.
                                   It is done so immediately (if thread has not been suspended) or when the suspended thread is resumed. */
        ++count;                // critical section
        sem_post (&sem);        // increments the semaphore value
    }
    pthread_exit(0);
}

int main(void)
{
    pthread_t tid1, tid2;

    // All semaphore functions return 0 on success and -1 on error.

    if (sem_init(&sem, 0, SEMINITVALUE) == -1) // initializes semaphore object pointed to by sem
    {
        printf("\n ERROR creating semaphore");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&tid1, NULL, function, NULL))
    {
      printf("\n ERROR creating thread 1");
      exit(EXIT_FAILURE);
    }

    if (pthread_create(&tid2, NULL, function, NULL))
    {
      printf("\n ERROR creating thread 2");
      exit(EXIT_FAILURE);
    }

    if (pthread_join(tid1, NULL)) // wait for the thread 1 to finish
    {
      printf("\n ERROR joining thread");
      exit(EXIT_FAILURE);
    }

    if (pthread_join(tid2, NULL)) // wait for the thread 2 to finish
    {
      printf("\n ERROR joining thread");
      exit(EXIT_FAILURE);
    }

    if (count < MAXCOUNT)
        printf("\n BOOM! count is [%d], should be %d\n", count, MAXCOUNT);
    else
        printf("\n OK! count is [%d]\n", count);

    sem_destroy(&sem); // No threads should be waiting on the semaphore at the time sem_destroy is called.

    exit(EXIT_SUCCESS);
}
