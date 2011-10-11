/*
A simple MPI example program using synchronous send. The program does
exactly the same as send-recv1, but the communication is synchronous.

The sender process sends a message containing its identifier
to the receiver. This receives the message and sends it back.
Both processes use synchronous send operations (MPI_Ssend)

Compile the program with 'mpicc send-synchronous.c -o send-synchronous'
Run the program with 'mpiexec -n 2 ./send-synchronous'
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
  int x, y, np, me;
  int tag = 42;
  MPI_Status  status;

  MPI_Init(&argc, &argv);               /* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &np);   /* Get number of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);   /* Get own identifier */

  /* Check that we run on exactly two processors */
  if (np != 2) {
    if (me == 0) {
      printf("You have to use exactly 2 processors to run this program\n");
    }
    MPI_Finalize();	       /* Quit if there is only one processor */
    exit(0);
  }
  
  x = me;
  if (me == 0) {    /* Process 0 does this */
    
    printf("Process %d sending to process 1\n", me);
    MPI_Ssend(&x, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);  /* Synchronous send */
    printf("Process %d receiving from process 1\n", me);
    MPI_Recv (&y, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
    printf ("Process %d received value %d\n", me, y);

    
  } else {         /* Process 1 does this */
    y = me;
    /* Since we use synchronous send, we have to do the receive-operation */
    /* first, otherwise we will get a deadlock */
    MPI_Ssend (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);  /* Synchronous send */
    MPI_Recv (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

  }

  MPI_Finalize();
  exit(0);
}
