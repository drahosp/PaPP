/*
A simple MPI example program using non-blocking send and receive.

The program consists of two processes. Process 0 sends a message
containing its identifier to the receiver. This receives the message
and sends it back.
Both processes use non-blocking send and receive operations
(MPI_Isend and MPI_Irecv, and MPI_Wait to wait until the message
has arrived).

Compile the program with 'mpicc send-nonblocking-wait.c -o send-nonblocking-wait'
Run the program with 'mpiexec -n 2 ./send-nonblocking-wait'
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
  int x, y, np, me;
  int tag = 42;
  MPI_Status status;
  MPI_Request send_req, recv_req;    /* Request object for send and receive */

  MPI_Init(&argc, &argv);              /* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &np);  /* Get number of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);  /* Get own identifier */

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
    MPI_Isend(&x, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &send_req);
    printf("Process %d receiving from process 1\n", me);
    MPI_Irecv (&y, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &recv_req);

    /* We could do computations here while we are waiting for communication */

    MPI_Wait(&send_req, &status);
    MPI_Wait(&recv_req, &status);
    printf("Process %d ready\n", me);
    
  } else {         

    MPI_Irecv (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &recv_req);
    MPI_Isend (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &send_req);

    /* We could do computations here while we are waiting for communication */

    MPI_Wait(&recv_req, &status);
    MPI_Wait(&send_req, &status);

  }

  MPI_Finalize();
  exit(0);
}
