#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NITER 1000

// EXAMPLE: Semaphore
// Semaphore controlled count to 2*NITER
// 2 Threads increment a counter
int count = 0;
sem_t sem;

void *function(void *null) {
    int i, tmp;
    for(i = 0; i < NITER; i++) {
        sem_wait(&sem);
        tmp = count;
        tmp = tmp+1;
        count = tmp;
        sem_post (&sem);
    }
    pthread_exit(0);
}

int main(void) {
    pthread_t tid1, tid2;

    if (sem_init(&sem, 0, 1) == -1) {
        printf("\n ERROR creating semaphore");
        exit(1);
    }

    if(pthread_create(&tid1, NULL, function, NULL)) {
      printf("\n ERROR creating thread 1");
      exit(1);
    }

    if(pthread_create(&tid2, NULL, function, NULL)) {
      printf("\n ERROR creating thread 2");
      exit(1);
    }

    if(pthread_join(tid1, NULL)) {	/* wait for the thread 1 to finish */
      printf("\n ERROR joining thread");
      exit(1);
    }

    if(pthread_join(tid2, NULL)) {  /* wait for the thread 2 to finish */
      printf("\n ERROR joining thread");
      exit(1);
    }

    if (count < 2 * NITER)
        printf("\n BOOM! count is [%d], should be %d\n", count, 2*NITER);
    else
        printf("\n OK! count is [%d]\n", count);

    sem_destroy(&sem);
    exit(EXIT_SUCCESS);
}
