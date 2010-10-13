
// Dining philosophers - all philosophers take first the right and than the left chopstick, except leftie
// gcc -o footman dphil_common.c dphil_footman.c -lpthread

#include "dphil.h"

pthread_mutex_t chopstick_mutexes[PHIL_COUNT];
pthread_mutex_t footman_mutex = PTHREAD_MUTEX_INITIALIZER;;
pthread_cond_t footman_cond = PTHREAD_COND_INITIALIZER;

int footman_count = 0;

void Init(void) {
    int i;
    for (i=0;i<PHIL_COUNT;i++)
        pthread_mutex_init(&chopstick_mutexes[i], NULL);
    pthread_mutex_init(&footman_mutex, NULL);
    pthread_cond_init(&footman_cond, NULL);
}

void Close(void) {
    int i;
    for (i=0;i<PHIL_COUNT;i++)
        pthread_mutex_destroy(&chopstick_mutexes[i]);
    pthread_mutex_destroy(&footman_mutex);
    pthread_cond_destroy(&footman_cond);
}

void Philosopher(int phil) {
    Think(phil);

    int first = phil;
    int second = (phil+1)%PHIL_COUNT;


    pthread_mutex_lock(&footman_mutex);
    while (footman_count == (PHIL_COUNT-1)) pthread_cond_wait(&footman_cond, &footman_mutex);
    footman_count++;
    pthread_mutex_unlock(&footman_mutex);

    pthread_mutex_lock(&chopstick_mutexes[first]);
    pthread_mutex_lock(&chopstick_mutexes[second]);

    Eat(phil);

    pthread_mutex_unlock(&chopstick_mutexes[first]);
    pthread_mutex_unlock(&chopstick_mutexes[second]);

    pthread_mutex_lock(&footman_mutex);
    footman_count--;
    pthread_cond_signal(&footman_cond);
    pthread_mutex_unlock(&footman_mutex);
}
