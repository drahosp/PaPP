/*
A simple MPI example program using non-blocking send and receive
and MPI_Waitall to wait for completion of all message transfers.

The program consists of one processes (process 0) which sends a message
containing its identifier to N-1 other processes. These receive the
message and send it back.
Both processes use non-blocking send and receive operations
(MPI_Isend and MPI_Irecv, and MPI_Waitall to wait until the messages
have arrived).

Compile the program with 'mpicc send-nonblocking-waitall.c -o send-nonblocking-waitall'
Run the program with 'mpiexec -n 6 ./send-nonblocking-waitall'
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#define MAXPROC 8    /* Max number of procsses */

int main(int argc, char* argv[]) {
  int i, x, np, me;
  int tag = 42;

  MPI_Status status[MAXPROC];
  /* Request objects for non-blocking send and receive */
  MPI_Request send_req[MAXPROC], recv_req[MAXPROC];
  int y[MAXPROC];  /* Array to receive values in */

  MPI_Init(&argc, &argv);                /* Initialize */
  MPI_Comm_size(MPI_COMM_WORLD, &np);    /* Get nr of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);    /* Get own identifier */

  /* First check that we have at least 2 and at most MAXPROC processes */
  if (np<2 || np>MAXPROC) {
    if (me == 0) {
      printf("You have to use at lest 2 and at most %d processes\n", MAXPROC);
    }
    MPI_Finalize();
    exit(0);
  }
  
  x = me;   /* This is the value we send, the process id */

  if (me == 0) {    /* Process 0 does this */
    
    printf("Process %d sending to all other processes\n",me);
    /* Send a message containing the process id to all other processes */
    for (i=1; i<np; i++) {
      printf("Pocess %d sending to %d\n", me, i);
      MPI_Isend(&x, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &send_req[i]);
    }
    /* While the messages are delivered, we could do computations here */

    /* Wait until all messages have been sent */
    /* Note that we use requests and statuses starting from index 1 
       since process zero does not send to it self */ 
    MPI_Waitall(np-1, &send_req[1], &status[1]);

    printf("Process %d receiving from all other processes\n", me);
    /* Receive a message from all other processes */
    for (i=1; i<np; i++) {
      MPI_Irecv (&y[i], 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &recv_req[i]);
    }
    /* While the messages are delivered, we could do computations here */

    /* Wait until all messages have been received */
    /* Requests and statuses start from index 1 */ 
    MPI_Waitall(np-1, &recv_req[1], &status[1]);

    /* Print out one line for each message we received */
    for (i=1; i<np; i++) {
      printf("Process %d received a message from process %d\n", me, y[i]);
    }
    printf("Process %d ready\n", me);
    
  } else { /* all other processes do this */

    MPI_Irecv (&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &recv_req[0]);
    MPI_Wait(&recv_req[0], &status[0]);

    MPI_Isend (&x, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &send_req[0]);
    /* Lots of computations here */
    MPI_Wait(&send_req[0], &status[0]);

  }

  MPI_Finalize();
  exit(0);
}
