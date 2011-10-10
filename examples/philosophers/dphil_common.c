#include "dphil.h"

#define _T(X) testError(X, __LINE__, __FILE__)

unsigned int random_time(void) {
    // 1-3 seconds, reentrant random
    return 1 + (rand_r(&seed)/(RAND_MAX/3));
}

void testError(int rc, int line, char* file) {
    if (rc == 0) return;
    printf("Error %s (0x%X) on line %d in file\n%s\n", strerror(rc), rc, line, file);
    exit(EXIT_FAILURE);
}

void Pickup(int phil) {
    if (chopsticks[phil] == phil) { printf("Left chopstick already picked up by this philosopher\n"); exit(1); }
    if (chopsticks[phil] != -1) { printf("Left chopstick already picked up by some philosopher\n"); exit(1); }
    chopsticks[phil] = phil;

    if (chopsticks[(phil+1)%PHIL_COUNT] == phil) { printf("Right chopstick already picked up by this philosopher\n"); exit(1); }
    if (chopsticks[(phil+1)%PHIL_COUNT] != -1) { printf("Right chopstick already picked up by some philosopher\n"); exit(1); }
    chopsticks[(phil+1)%PHIL_COUNT] = phil;
}

void Pickdown(int phil) {
    if (chopsticks[phil] != phil) { printf("Left chopstick hasn't been picked up by this philosopher\n"); exit(1); }
    chopsticks[phil] = -1;

    if (chopsticks[(phil+1)%PHIL_COUNT] != phil) { printf("Right chopstick hasn't been picked up by this philosopher\n"); exit(1); }
    chopsticks[(phil+1)%PHIL_COUNT] = -1;
}

void Eat(int phil) {
    _T(pthread_mutex_lock(&aux_mutex));
    block_time[phil] += time(0) - block_start[phil];
    int eat_len = random_time();
    eat_time[phil] += eat_len;
    Pickup(phil);
    printf("Philosopher %d is eating for %d s\n", phil, eat_len);
    _T(pthread_mutex_unlock(&aux_mutex));
    sleep(eat_len);
     _T(pthread_mutex_lock(&aux_mutex));
    printf("Philosopher %d stopped eating\n", phil);
    Pickdown(phil);
     _T(pthread_mutex_unlock(&aux_mutex));
}

void Think(int phil) {
    _T(pthread_mutex_lock(&aux_mutex));
    int think_len = random_time();
    think_time[phil] += think_len;
    printf("Philosopher %d is thinking for %d s\n", phil, think_len);
    _T(pthread_mutex_unlock(&aux_mutex));
    sleep(think_len);
    _T(pthread_mutex_lock(&aux_mutex));
    printf("Philosopher %d stopped thinking\n", phil);
    block_start[phil] = time(0);
    _T(pthread_mutex_unlock(&aux_mutex));
}

void* philosopher_wrapper(void* param) {
    int phil;
    phil = (int)(param);

    while(!stop) {
        Philosopher(phil);
    }

    pthread_exit(0);
    return NULL;
}

int main(void) {
    int i;
    stop = 0;
    seed = 0;

    srand(time(0));

    pthread_t threads[PHIL_COUNT];
    int start_time;
    int total_time;

    for (i=0; i<PHIL_COUNT; i++) {
        chopsticks[i] = -1;
        eat_time[i] = 0;
        think_time[i] = 0;
        block_time[i] = 0;
    }

    _T(pthread_mutex_init(&aux_mutex, NULL));
    Init();

    start_time = time(0);

    for (i=0; i<PHIL_COUNT; i++)
        _T(pthread_create( &threads[i], NULL, &philosopher_wrapper, (void*) i));

    sleep(SIM_LEN);
    stop = 1;

    for (i=0; i<PHIL_COUNT; i++)
        _T(pthread_join( threads[i], NULL));

    total_time = time(0) - start_time;

    Close();

    for (i=0; i<PHIL_COUNT; i++) {
        printf("Philosopher %d: eat_time=%d%%, think_time=%d%%, block_time=%d%%\n", i, eat_time[i]*100/total_time, think_time[i]*100/total_time, block_time[i]*100/total_time);
    }

    pthread_mutex_destroy(&aux_mutex);
    exit(EXIT_SUCCESS);
}
