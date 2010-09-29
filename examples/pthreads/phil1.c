// Dining philosophers - simple solution with mutex
// gcc -o phil1 phil1.c -lpthread

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <windows.h>

#define PHIL_COUNT 5

int chopsticks[PHIL_COUNT];
int eat_times[PHIL_COUNT];
int think_times[PHIL_COUNT];
int block_times[PHIL_COUNT];
int block_starts[PHIL_COUNT];
int thread_starts[PHIL_COUNT];
int thread_stops[PHIL_COUNT];
int stop = 0;

pthread_mutex_t aux_mutex = PTHREAD_MUTEX_INITIALIZER;



void sleep_ms(int time_in_ms)
{
    Sleep(time_in_ms);
}

int time_ms(void)
{
   return GetTickCount();
}


unsigned int rnd_x = 0;

void srandom(int seed_x) {
    rnd_x = seed_x;
}

unsigned int random(void) {
    rnd_x = 16807 * ( rnd_x % (2147483647 / 16807) ) - (2147483647 % 16807) * ( rnd_x / (2147483647 / 16807) );
    return rnd_x <= 0 ? rnd_x += 2147483647 : rnd_x;
}

unsigned int random_time_ms(void) {
    return 100 + (random() % 10)*100;
}

void pickup(int phil) {
    if (chopsticks[phil] == phil) { printf("Left chopstick already picked up by this philosopher\n"); exit(1); }
    if (chopsticks[phil] != -1) { printf("Left chopstick already picked up by some philosopher\n"); exit(1); }
    chopsticks[phil] = phil;

    if (chopsticks[(phil+1)%PHIL_COUNT] == phil) { printf("Right chopstick already picked up by this philosopher\n"); exit(1); }
    if (chopsticks[(phil+1)%PHIL_COUNT] != -1) { printf("Right chopstick already picked up by some philosopher\n"); exit(1); }
    chopsticks[(phil+1)%PHIL_COUNT] = phil;
}

void pickdown(int phil) {
    if (chopsticks[phil] != phil) { printf("Left chopstick hasn't been picked up by this philosopher\n"); exit(1); }
    chopsticks[phil] = -1;

    if (chopsticks[(phil+1)%PHIL_COUNT] != phil) { printf("Right chopstick hasn't been picked up by this philosopher\n"); exit(1); }
    chopsticks[(phil+1)%PHIL_COUNT] = -1;
}

void eat(int phil) {
    pthread_mutex_lock(&aux_mutex);
    block_times[phil] += time_ms() - block_starts[phil];
    int eat_time = random_time_ms();
    eat_times[phil] += eat_time;
    pickup(phil);
    printf("Philosopher %d is eating for %d s\n", phil, eat_time);
    pthread_mutex_unlock(&aux_mutex);
    sleep_ms(eat_time);
    pthread_mutex_lock(&aux_mutex);
    printf("Philosopher %d stopped eating\n", phil);
    pickdown(phil);
    pthread_mutex_unlock(&aux_mutex);
}

void think(int phil) {
    pthread_mutex_lock(&aux_mutex);
    int think_time = random_time_ms();
    think_times[phil] += think_time;
    printf("Philosopher %d is thinking for %d s\n", phil, think_time);
    pthread_mutex_unlock(&aux_mutex);
    sleep_ms(think_time);
    pthread_mutex_lock(&aux_mutex);
    printf("Philosopher %d stopped thinking\n", phil);
    block_starts[phil] = time_ms();
    pthread_mutex_unlock(&aux_mutex);
}



void* philosopher1(void* param);
void* philosopher2(void* param);
void init1(void);
void init2(void);

int main(void) {

    int i;

    srandom(time(0));

    pthread_t threads[PHIL_COUNT];
    int start_time;
    int total_time;

    for (i=0; i<PHIL_COUNT; i++) {
        chopsticks[i] = -1;
        eat_times[i] = 0;
        think_times[i] = 0;
        block_times[i] = 0;
    }

    start_time = time_ms();

    init2();

    for (i=0; i<PHIL_COUNT; i++)
        pthread_create( &threads[i], NULL, &philosopher2, (void*) i);

    sleep_ms(100000);
    stop = 1;

    for (i=0; i<PHIL_COUNT; i++)
        pthread_join( threads[i], NULL);

    total_time = time_ms() - start_time;

    for (i=0; i<PHIL_COUNT; i++) {
        //printf("Philosopher %d: eat_time=%d ms, think_time=%d ms and block_time=%d ms (%d ms)\n", i, eat_times[i], think_times[i], (thread_stops[i]-thread_starts[i])-(eat_times[i]+think_times[i]), block_times[i]);
        printf("Philosopher %d: eat_time=%d%%, think_time=%d%%, block_time=%d%%\n", i, eat_times[i]*100 / (thread_stops[i]-thread_starts[i]), think_times[i]*100 / (thread_stops[i]-thread_starts[i]), block_times[i]*100 / (thread_stops[i]-thread_starts[i]));
    }

    exit(EXIT_SUCCESS);
}


pthread_mutex_t phil_mutex;

void init1(void)
{
    phil_mutex = PTHREAD_MUTEX_INITIALIZER;
}

void* philosopher1(void* param) {
    int phil;
    phil = (int)(param);

    thread_starts[phil] = time_ms();

    while(!stop) {
        think(phil);
        pthread_mutex_lock(&phil_mutex);
        eat(phil);
        pthread_mutex_unlock(&phil_mutex);
    }

    thread_stops[phil] = time_ms();

    pthread_exit(0);
    return NULL;
}



pthread_mutex_t chopstick_mutexes[PHIL_COUNT];
int

void init2(void)
{
    int i;
    for (i=0;i<PHIL_COUNT;i++)
        chopstick_mutexes[i] = PTHREAD_MUTEX_INITIALIZER;
}

void* philosopher2(void* param) {
    int phil;
    phil = (int)(param);

    thread_starts[phil] = time_ms();

    while(!stop) {
        think(phil);


        pthread_mutex_lock(&chopstick_mutexes[phil]);
        pthread_mutex_lock(&chopstick_mutexes[(phil+1)%PHIL_COUNT]);
        eat(phil);
        pthread_mutex_unlock(&chopstick_mutexes[(phil+1)%PHIL_COUNT]);
        pthread_mutex_unlock(&chopstick_mutexes[phil]);
    }

    thread_stops[phil] = time_ms();

    pthread_exit(0);
    return NULL;
}

