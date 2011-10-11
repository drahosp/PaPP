/*
A simple MPI example program using standard mode send and receive

The program consists of two processes. Process 0 sends a message
containing its identifier to the receiver. This receives the message
and sends it back.

Compile the program with 'mpicc send-standard.c -o send-standard'
Run the program with 'mpiexec -n 2 ./send-standard'
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
  int x, y, np, me;
  int tag = 42;
  MPI_Status status;

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
  
  x = me;
  if (me == 0) {

    printf("Process %d sending to process 1\n", me);    
    MPI_Send(&x, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
    printf("Process %d receiving from process 1\n", me);
    MPI_Recv (&y, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
    printf ("Process %d received value %d\n", me, y);
    
  } else { /* me == 1 */
    
    y = me;
    MPI_Recv (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    MPI_Send (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
  }
  
  MPI_Finalize();
  exit(0);
}
