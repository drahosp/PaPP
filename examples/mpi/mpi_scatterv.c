/************************************************************************

A simple MPI example program that uses MPI_Scatterv.

The program should be run with exactly 8 processes.
Process zero initializes a matrix of 8 by 8 integers and distributes
the lower triangular part of the matrix to the processes using
MPI_Scatterv. The processes receive different number of elements:
process 0 gets one element, process 1 gets 2 elements and process i
gets i+1 elements. The processes (except process 0) then send a
message back containing the hostname of the machine the process runs
on and its own part of the distributed array. Process 0 receives
these messages and prints out the received values, formatted as a
lower triangular matrix.

Compile the program with 'mpicc scatterv.c -o scatterv'
Run the program with 'mpiexec -n 8 scatterv'

************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#define MAXPROC 8    /* Max number of procsses */
#define LENGTH 8    /*  Size of matrix is LENGTH * LENGTH */

int main(int argc, char* argv[]) {
  int i, j, np, me, count;
  const int nametag  = 42;    /* Tag value for sending name */
  const int datatag  = 43;    /* Tag value for sending data */
  const int root = 0;         /* Root process in scatter */
  MPI_Status status;          /* Status object for receive */

  char myname[MPI_MAX_PROCESSOR_NAME];             /* Local host name string */
  char hostname[MAXPROC][MPI_MAX_PROCESSOR_NAME];  /* Received host names */
  int namelen;

  int x[LENGTH][LENGTH];        /* Send buffer */
  int y[LENGTH];                /* Receive buffer */

  int *sendcount, *displs;      /* Arrays for sendcounts and displacements */

  MPI_Init(&argc, &argv);                /* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &np);    /* Get nr of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);    /* Get own identifier */
  
  MPI_Get_processor_name(myname, &namelen);  /* Get host name */
  myname[namelen++] = (char)0;               /* Terminating null byte */

  /* Check that we have one process for each row in the matrix */
  if (np != LENGTH) {
    if (me == 0) {
      printf("You have to use %d processes\n", LENGTH);
    }
    MPI_Finalize();
    exit(0);
  }

  /* Allocate memory for the sendcount and displacements arrays */
  sendcount = (int *)malloc(LENGTH*sizeof(int));
  displs = (int *)malloc(LENGTH*sizeof(int));

  /* Initialize sendcount and displacements arrays */
  for (i=0; i<LENGTH; i++) {
    sendcount[i] = i+1;
    displs[i] = i*LENGTH;
  }

  if (me == 0) {    /* Process 0 does this */
    
    /* Initialize the matrix x with values 0 .. LENGTH*LENGTH-1 */
    for (i=0; i<LENGTH; i++) {
      for (j=0; j<LENGTH; j++) {
	x[i][j] = i*LENGTH+j;
      }
    }

    /* Print out the matrix before it is distributed */
    printf("The matrix is\n");
    for (i=0; i<LENGTH; i++) {
      for (j=0; j<LENGTH; j++) {
	printf("%4d ", x[i][j]);
      }
      printf("\n");
    }

    printf("\nProcess %d on host %s is scattering the lower triangular part \nof the matrix x to all %d processes\n\n",  me, myname, np);

    /* Scatter the lower triangular part of array x to all proceses, place it in y */
    MPI_Scatterv(&x, sendcount, displs, MPI_INT, &y, LENGTH, MPI_INT, root, \
		 MPI_COMM_WORLD);

    /* Process zero prints out own portion of the scattered array */
    for (i=0; i<sendcount[me]; i++) {
      printf("%4d", y[i]);
    }
    printf("                            ");
    printf("  from process %d on host %s\n", me, myname);

    /* Receive messages with hostname and the scattered data */
    /* from all other processes */
    for (i=1; i<np; i++) {
      MPI_Recv (&hostname[i], namelen, MPI_CHAR, i, nametag, MPI_COMM_WORLD, \
		&status);
      MPI_Recv (&y, LENGTH, MPI_INT, i, datatag, MPI_COMM_WORLD, &status);
      MPI_Get_count(&status, MPI_INT, &count);  /* Get nr of elements in message */

      /* Print out the elements we received from this process */
      for (j=0; j<count; j++) {
	printf("%4d", y[j]);
      }
      for (j=0; j<LENGTH-count; j++) {     /* Format output to look like a matrix */
	printf("    ");
      }
      printf("  from process %d on host %s\n", i, hostname[i]);
    }
    
    printf("Ready\n");


  } else { /* all other processes do this */

    /* Receive the scattered matrix from process 0, place it in array y */
    MPI_Scatterv(&x, sendcount, displs, MPI_INT, &y, LENGTH, MPI_INT, root, \
		MPI_COMM_WORLD);

    /* Send own name back to process 0 */
    MPI_Send (&myname, namelen, MPI_CHAR, 0, nametag, MPI_COMM_WORLD);
    /* Send the received array back to process 0 */
    MPI_Send (&y, sendcount[me], MPI_INT, 0, datatag, MPI_COMM_WORLD);

  }

  MPI_Finalize();
  exit(0);
}
