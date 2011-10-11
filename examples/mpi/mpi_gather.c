/*
A simple MPI example program that uses MPI_Gather.

The program should be run with an even number of processes.
Each process initializes an array x of integers. These arrays are
collected to process zero using MPI_Gather and placed in an array y.
All processes, except process zero, then send a message with the
hostname of the machine the process runs on and its own part of
the distributed array. Process 0 receives these messages and prints
out the hostnames and the original arrays in each process.
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "mpi.h"
#define MAXPROC 8    /* Max number of procsses */
#define NAMELEN 80   /* Max length of machine name */
#define LENGTH 24    /* Lengt of send buffer is divisible by 2, 4, 6 and 8 */

int main(int argc, char* argv[]) {
  int i, j, np, me;
  const int nametag  = 42;    /* Tag value for sending name */
  const int datatag  = 43;    /* Tag value for sending data */
  const int root = 0;         /* Root process in scatter */
  MPI_Status status;          /* Status object for receive */

  char myname[NAMELEN];             /* Local host name string */
  char hostname[MAXPROC][NAMELEN];  /* Received host names */

  int x[LENGTH];        /* Send buffer */
  int y[LENGTH];        /* Receive buffer */

  MPI_Init(&argc, &argv);                /* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &np);    /* Get nr of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);    /* Get own identifier */
  
  gethostname(myname, NAMELEN);    /* Get host name */

  /* Check that we have an even number of processes and at most MAXPROC */
  if (np>MAXPROC || np%2 != 0) {
    if (me == 0) {
      printf("You have to use an even number of processes (at most %d)\n", MAXPROC);
    }
    MPI_Finalize();
    exit(0);
  }

  /* Each process initializes its local array */
  for (i=0; i<LENGTH/np; i++) {
    x[i] = (LENGTH/np)*me+i;
  }

  if (me == 0) {    /* Process 0 does this */
    
    printf("Process %d on host %s is gathering array x from all %d processes\n\n", \
	   me, myname, np);

    /* Gather the array x from all proceses, place it in y */
    MPI_Gather(x, LENGTH/np, MPI_INT, y, LENGTH/np, MPI_INT, root, MPI_COMM_WORLD);

    /* Print out the gathered array */
    printf("Process %d on host %s got elements\n", me, myname);
    for (i=0; i<LENGTH; i++) {
      printf(" %d", y[i]);
    }
    printf("\n\n");

    /* Print out the local array x on process 0 */
    printf("Process %d on host %s had elements", me, myname);
    for (i=0; i<LENGTH/np; i++) {
      printf (" %d", x[i]);
    }
    printf ("\n");

    /* Receive messages with hostname and the original data */
    /* from all other processes */
    for (i=1; i<np; i++) {
      MPI_Recv (&hostname[i], NAMELEN, MPI_CHAR, i, nametag, MPI_COMM_WORLD, \
		&status);
      MPI_Recv (&y, LENGTH/np, MPI_INT, i, datatag, MPI_COMM_WORLD, &status);
      printf("Process %d on host %s had elements", i, hostname[i]);
      for (j=0; j<LENGTH/np; j++) {
	printf(" %d", y[j]);
      }
      printf("\n");
    }
    
    printf("Ready\n");


  } else { /* all other processes do this */

    /* Receive the scattered array from process 0, place it in array y */
    MPI_Gather(&x, LENGTH/np, MPI_INT, &y, LENGTH/np, MPI_INT, root, \
		MPI_COMM_WORLD);
    /* Send own name back to process 0 */
    MPI_Send (&myname, NAMELEN, MPI_CHAR, 0, nametag, MPI_COMM_WORLD);
    /* Send the received array back to process 0 */
    MPI_Send (&x, LENGTH/np, MPI_INT, 0, datatag, MPI_COMM_WORLD);

  }

  MPI_Finalize();
  exit(0);
}
