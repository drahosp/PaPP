/***********************************************************************

A simple MPI example program that uses MPI_Scatter.

The program should be run with an even number of processes.
Process zero initializes an array of integers x and distributes the
array evenly among all processes using MPI_Scatter. The processes
receive thir part of the array into a local array y. Each process
replies with a message containing the hostname of the machine
the process runs on and its own part of the distributed array.
Process 0 receives messages containing hostnames and the distributed
array, and  prints out the received messages

Compile the program with 'mpicc scatter.c -o scatter'
Run the program with 'mpiexec -n 4 scatter'

*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#define MAXPROC 8    /* Max number of procsses */
#define LENGTH 24    /* Lengt of send buffer is divisible by 2, 4, 6 and 8 */

int main(int argc, char* argv[]) {
  int i, j, np, me;
  const int nametag  = 42;    /* Tag value for sending name */
  const int datatag  = 43;    /* Tag value for sending data */
  const int root = 0;         /* Root process in scatter */
  MPI_Status status;          /* Status object for receive */

  char myname[MPI_MAX_PROCESSOR_NAME];       /* Local host name string */
  char hostname[MAXPROC][MPI_MAX_PROCESSOR_NAME];  /* Received host names */
  int namelen;

  int x[LENGTH];        /* Send buffer */
  int y[LENGTH];        /* Receive buffer */

  MPI_Init(&argc, &argv);                /* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &np);    /* Get nr of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);    /* Get own identifier */
  
  MPI_Get_processor_name(myname, &namelen);  /* Get host name */
  myname[namelen++] = (char)0;               /* Terminating null byte */

  /* Check that we have an even number of processes and at most MAXPROC */
  if (np>MAXPROC || np%2 != 0) {
    if (me == 0) {
      printf("You have to use an even number of processes (at most %d)\n", MAXPROC);
    }
    MPI_Finalize();
    exit(0);
  }

  if (me == 0) {    /* Process 0 does this */
    
    /* Initialize the array x with values 0 .. LENGTH-1 */
    for (i=0; i<LENGTH; i++) {
      x[i] = i;
    }

    printf("Process %d on host %s is distributing array x to all %d processes\n\n",
	   me, myname, np);

    /* Scatter the array x to all proceses, place it in y */
    MPI_Scatter(x, LENGTH/np, MPI_INT, y, LENGTH/np, MPI_INT, root,
		MPI_COMM_WORLD);

    /* Print out own portion of the scattered array */
    printf("Process %d on host %s has elements", me, myname);
    for (i=0; i<LENGTH/np; i++) {
      printf(" %d", y[i]);
    }
    printf("\n");

    /* Receive messages with hostname and the scattered data */
    /* from all other processes */
    for (i=1; i<np; i++) {
      MPI_Recv (hostname[i], namelen, MPI_CHAR, i, nametag, MPI_COMM_WORLD,
		&status);
      MPI_Recv (y, LENGTH/np, MPI_INT, i, datatag, MPI_COMM_WORLD, &status);
      printf("Process %d on host %s has elements", i, hostname[i]);
      for (j=0; j<LENGTH/np; j++) {
	printf(" %d", y[j]);
      }
      printf("\n");
    }
    
    printf("Ready\n");


  } else { /* all other processes do this */

    /* Receive the scattered array from process 0, place it in array y */
    MPI_Scatter(x, LENGTH/np, MPI_INT, y, LENGTH/np, MPI_INT, root, \
		MPI_COMM_WORLD);
    /* Send own name back to process 0 */
    MPI_Send (myname, namelen, MPI_CHAR, 0, nametag, MPI_COMM_WORLD);
    /* Send the received array back to process 0 */
    MPI_Send (y, LENGTH/np, MPI_INT, 0, datatag, MPI_COMM_WORLD);

  }

  MPI_Finalize();
  exit(0);
}
