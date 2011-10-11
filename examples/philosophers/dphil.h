#ifndef DPHIL_H
#define DPHIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define PHIL_COUNT 5
#define SIM_LEN 100

// Data declarations
int chopsticks[PHIL_COUNT];
int eat_time[PHIL_COUNT];
int think_time[PHIL_COUNT];
int block_time[PHIL_COUNT];
int block_start[PHIL_COUNT];

int stop;
unsigned int seed;

pthread_mutex_t aux_mutex;

// Function declatations
void Init();
void Close();
void Philosopher(int);
void Think(int);
void Eat(int);

#endif
