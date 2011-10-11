/*
A simple MPI example program using broadcast and non-blocking receive
with MPI_Waitany.

The program consists of one processes (process 0) which broadcasts a
message containing its identifier to N-1 other processes. These receive
the message and and replies with a message containing the hostname of the
machine the process runs on. The sender process receives thes messages
using non-blocking receive. It accepts messages in the order they arrive
(using MPI_Waitany), and prints out the received identifier and hostname.
You can think of this program as a test of which of the processes are the
fastest to reply.

Compile the program with 'mpicc send-nonblocking-waitany.c -o send-nonblocking-waitany'
Run the program with 'mpirun -np 6 ./send-nonblocking-waitany'
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#define MAXPROC 16    /* Max number of procsses */

int main(int argc, char* argv[]) {
  int i, np, me, index;
  const int tag  = 42;    /* Tag value for communication */
  const int root = 0;     /* Root process in broadcast */

  MPI_Status status;              /* Status object for non-blocing receive */
  MPI_Request recv_req[MAXPROC];  /* Request objects for non-blocking receive */
  
  char myname[MPI_MAX_PROCESSOR_NAME];             /* Local host name string */
  char hostname[MAXPROC][MPI_MAX_PROCESSOR_NAME];  /* Received host names */
  int namelen;

  MPI_Init(&argc, &argv);                /* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &np);    /* Get nr of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);    /* Get own identifier */
  
  MPI_Get_processor_name(myname, &namelen);  /* Get host name */
  myname[namelen++] = (char)0;               /* Terminating null byte */

  /* First check that we have at least 2 and at most MAXPROC processes */
  if (np<2 || np>MAXPROC) {
    if (me == 0) {
      printf("You have to use at lest 2 and at most %d processes\n", MAXPROC);
    }
    MPI_Finalize();
    exit(0);
  }

  if (me == 0) {    /* Process 0 does this */

    printf("Process %d on host %s broadcasting to all processes\n",me, myname);
    /* Broadcast a message containing the process id */
    MPI_Bcast(&me, 1, MPI_INT, root, MPI_COMM_WORLD);

    printf("Process %d receiving from all other processes\n", me);
    /* Receive a message from all other processes */
    for (i=1; i<np; i++) {
      MPI_Irecv (hostname[i], namelen, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, \
		 &recv_req[i]);
    }
    /* While the messages are delivered, we could do computations here */

    for (i=1; i<np; i++) {
      /* Wait until at least one message has been received */
      /* Request array start from element 1, because we don't receve */
      /* any message from process 0 (this process) */ 
      MPI_Waitany(np-1, &recv_req[1], &index, &status);

      /* Print out which process we received a message from */
      printf("Received a message from process %d on %s\n", status.MPI_SOURCE, \
	hostname[index+1]);
    }
    printf("Ready\n");

  } else { /* all other processes do this */

    int y;  /* Local variable for broadcast */

    /* Receive the broadcasted message from process 0 */
    MPI_Bcast(&y, 1, MPI_INT, root, MPI_COMM_WORLD);
    /* Send own identifier back to process 0 */
    MPI_Send (myname, namelen, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

  }

  MPI_Finalize();
  exit(0);
}
