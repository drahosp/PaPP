
// Dining philosophers - simple solution with "table" locking
// gcc -o locktable dphil_common.c dphil_locktable.c -lpthread

#include "dphil.h"

pthread_mutex_t table_mutex;

void Init(void) {
    pthread_mutex_init(&table_mutex, NULL);
}

void Close(void) {
    pthread_mutex_destroy(&table_mutex);
}

void Philosopher(int phil) {

    Think(phil);
    pthread_mutex_lock(&table_mutex);
    Eat(phil);
    pthread_mutex_unlock(&table_mutex);
}
