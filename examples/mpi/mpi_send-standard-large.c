/*
A simple MPI example program using standard mode send and receive

The program consists of two processes. Process 0 sends a large message
to the receiver. This receives the message and sends it back.
This program deadlocks if the the send and receive calls are in
the wrong order, i.e. if both proceesses first try to send, because
the message is large enough so that standard mode send does not use
buffered communication.

Compile the program with 'mpicc send-standard-large.c -o send-standard-large'
Run the program with 'mpiexec -n 2 ./send-standard-large'
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
  const int K = 1024;
  const int msgsize = 64*K;  /* -5 is still buffered */
  int np, me, i;
  int *X, *Y;
  int tag = 42;
  MPI_Status  status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);

  /* Check that we run on exactly two processors */
  if (np != 2) {
    if (me == 0) {
      printf("You have to use exactly 2 processors to run this program\n");
    }
    MPI_Finalize();	       /* Quit if there is only one processor */
    exit(0);
  }
  
  /* Allocate memory for large message buffers */
  X = malloc(msgsize*sizeof(int));
  Y = malloc(msgsize*sizeof(int));


  /* Initialize X and Y */
  for (i=0; i<msgsize; i++) {
    X[i] = 1234;
    Y[i] = 2;
  }

  if (me == 0) {

    printf("Message size is %d bytes\n", msgsize*sizeof(int));

    printf("Process %d sending to process 1\n", me);
    MPI_Send(X, msgsize, MPI_INT, 1, tag, MPI_COMM_WORLD);
    printf("Process %d receiving from process 1\n", me);
    MPI_Recv (Y, msgsize, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
    printf ("Y now has the value %d\n", Y[0]);

  } else { /* me == 1 */

    MPI_Recv (Y, msgsize, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    MPI_Send (Y, msgsize, MPI_INT, 0, tag, MPI_COMM_WORLD);

  }

  MPI_Finalize();
  exit(0);
}
