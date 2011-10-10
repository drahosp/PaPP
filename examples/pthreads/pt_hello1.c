/*
  A polyglot version of the Hello World program using Pthreads.
  The threads are passed a single argument, a numerical id, which
  they use to print out one of 8 different salutations.
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define NUM_THREADS	10

/* Strings for the Hello World messages */
char *messages[NUM_THREADS];


void *PrintHello(void *threadid) {
   int *myarg;
   sleep(1);
   myarg = (int *) threadid;
   printf("Thread %d: %s\n", *myarg, messages[*myarg]);
   pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];    /* Thread identifiers */
  int *taskid[NUM_THREADS];          /* Id numbers for each thread */
  int rc, i;
  
  messages[0] = "English: Hello World!";
  messages[1] = "French: Bonjour, le monde!";
  messages[2] = "Spanish: Hola al mundo";
  messages[3] = "Klingon: Nuq neH!";
  messages[4] = "German: Guten Tag, Welt!"; 
  messages[5] = "Russian: Zdravstvytye, mir!";
  messages[6] = "Japan: Sekai e konnichiwa!";
  messages[7] = "Latin: Orbis, te saluto!";
  messages[8] = "Finnish: Terve, maailma!";
  messages[9] = "Swedish: Hallå, värld";
  
  
  for(i=0; i<NUM_THREADS; i++) {
    taskid[i] = (int *) malloc(sizeof(int));
    *taskid[i] = i;
    /* printf("Creating thread %d\n", i); */
    rc = pthread_create(&threads[i], NULL, PrintHello, (void *) taskid[i]);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }
  
  pthread_exit(NULL);
}
