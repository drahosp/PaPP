
// Dining philosophers - all philosophers take first the right and than the left chopstick, except leftie
// gcc -o leftie dphil_common.c dphil_leftie.c -lpthread

#include "dphil.h"

pthread_mutex_t chopstick_mutexes[PHIL_COUNT];

void Init(void) {
    int i;
    for (i=0;i<PHIL_COUNT;i++)
        pthread_mutex_init(&chopstick_mutexes[i], NULL);
}

void Close(void) {
    int i;
    for (i=0;i<PHIL_COUNT;i++)
        pthread_mutex_destroy(&chopstick_mutexes[i]);
}

void Philosopher(int phil) {
    Think(phil);

    int first = (phil != 0 ? phil : (phil+1)%PHIL_COUNT);
    int second = (phil != 0 ? (phil+1)%PHIL_COUNT : phil);

    pthread_mutex_lock(&chopstick_mutexes[first]);
    pthread_mutex_lock(&chopstick_mutexes[second]);

    Eat(phil);

    pthread_mutex_unlock(&chopstick_mutexes[first]);
    pthread_mutex_unlock(&chopstick_mutexes[second]);
}
