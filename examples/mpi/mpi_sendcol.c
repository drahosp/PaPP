/************************************************************************

A simple MPI example program that uses MPI_Type_vector to build a
derived datatype that describes a column of a matrix.

The program should be run with two processes.
The program first builds a derived datatype consisting of a column
of elements from a square matrix. Process zero then initializes a
matrix with integers 0 .. LENGTH*LENGTH-1 and sends the first and
last columns of the matrix x to process 1. This recieves the columns
and places them into a local matrix y, which is initialized with all
zeros. Process one then sends back a message with its own hostname
and the modified matrix y. Process zero receives this and prints out
the modified matrix, where all but the first and last columns are
zeros.

Compile the program with 'mpicc sendcol.c -o sendcol'
Run the program with 'mpiexec -n 2 sendcol'

************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#define LENGTH 8    /*  Size of matrix in LENGTH*LENGTH */

int main(int argc, char* argv[]) {
  int i, j, np, me;
  const int nametag  = 42;    /* Tag value for sending name */
  const int datatag  = 43;    /* Tag value for sending data */
  MPI_Status status;          /* Status object for receive */
  MPI_Datatype Column_type;   /* Data type for a column of the matrix */

  char myname[MPI_MAX_PROCESSOR_NAME];       /* Local host name string */
  char hostname[MPI_MAX_PROCESSOR_NAME];     /* Received host names */

  int x[LENGTH][LENGTH];      /* Send buffer */
  int y[LENGTH][LENGTH];      /* Receive buffer */
  int namelen;

  MPI_Init(&argc, &argv);                /* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &np);    /* Get nr of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);    /* Get own identifier */
  
  MPI_Get_processor_name(myname, &namelen);  /* Get host name */
  myname[namelen++] = (char)0;               /* Terminating null byte */

  /* Define the column data type */
  MPI_Type_vector(LENGTH, 1, LENGTH, MPI_INT, &Column_type);
  MPI_Type_commit(&Column_type);

  if (np != 2) {
    if (me == 0) printf("You have to run this program with 2 processes\n");
    MPI_Finalize();
    exit(0);
  }

  if (me == 0) {    /* Process 0 does this */
    int k = 0;
    /* Initialize the matrix x with values 0 .. LENGTH*LENGTH-1 */
    for (i=0; i<LENGTH; i++) {
      for (j=0; j<LENGTH; j++) {
	x[i][j] = k++;
      }
    }

    /* Print out the matrix before we send columns to process 1 */
    printf("The original matrix is\n");
    for (i=0; i<LENGTH; i++) {
      for (j=0; j<LENGTH; j++) {
	printf("%4d ", x[i][j]);
      }
      printf("\n");
    }

    /* Send first column of array x */
    MPI_Send(&x[0][0], 1, Column_type, 1, datatag, MPI_COMM_WORLD);

    /* Send last column of array x */
    MPI_Send(&x[0][LENGTH-1], 1, Column_type, 1, datatag, MPI_COMM_WORLD);

    /* Receive messages with hostname and the matrix */
    MPI_Recv (hostname, namelen, MPI_CHAR, 1, nametag, MPI_COMM_WORLD, \
	      &status);
    MPI_Recv (y, LENGTH*LENGTH, MPI_INT, 1, datatag, MPI_COMM_WORLD, &status);

    /* Print out the matrixes that we received from process 1 */
    printf("\nThe recived matrix from process 1 on host %s is\n", hostname);
    for (i=0; i<LENGTH; i++) {
      for (j=0; j<LENGTH; j++) {
	printf("%4d ", y[i][j]);
      }
      printf("\n");
    }

    printf("Ready\n");


  } else { /* Processes 1 does this */

    /* Initialize the matrix y with zeros */
    for (i=0; i<LENGTH; i++) {
      for (j=0; j<LENGTH; j++) {
	y[i][j] = 0;
      }
    }

    /* Receive the first and last columns from process 0, place them in array y */
    MPI_Recv(&y[0][0], 1, Column_type, 0, datatag, MPI_COMM_WORLD, &status);
    MPI_Recv(&y[0][LENGTH-1], 1, Column_type, 0, datatag, MPI_COMM_WORLD, &status);

        /* Send own name back to process 0 */
    MPI_Send (myname, namelen, MPI_CHAR, 0, nametag, MPI_COMM_WORLD);
    /* Send the modified matrix back to process 0 */
    MPI_Send (y, LENGTH*LENGTH, MPI_INT, 0, datatag, MPI_COMM_WORLD);

  }

  MPI_Finalize();
  exit(0);
}
