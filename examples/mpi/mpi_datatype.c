/*
A simple MPI example program that uses MPI_Type_struct to build a
derived datatype.

The program should be run with two processes. It first builds a
derived datatype consisting of a struct with three members:
two floating-point values and one integer. Process zero reads in
three input values (two floats and one int) from standard input,
places these in a struct and sends all three values in a message
to process one. This receives the message and sends it back to
process zero, which prints out the values.
*/

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
int main(int argc, char *argv[]) {
  const int tag = 42;	/* Message tag */
  int id, ntasks, err;
  MPI_Status status;

  MPI_Datatype my_type;   /* This is the new MPI datatype that we will build */

  typedef struct {     /* Data structure for input data */
    float a;
    float b;
    int n;
  } Indata_type;

  Indata_type indata, recdata;   /* Variables for input data */

  int lengtharray[3];           /* Array of lengths */
  MPI_Aint disparray[3];        /* Array of displacements */
  MPI_Datatype typearray[3];    /* Array of MPI datatypes */

  MPI_Aint startaddress, address;   /* Variables used to calculate displacements */

  err = MPI_Init(&argc, &argv);	/* Initialize MPI */
  if (err != MPI_SUCCESS) {
    printf("MPI initialization failed!\n");
    exit(1);
  }
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);	/* Get nr of tasks */
  MPI_Comm_rank(MPI_COMM_WORLD, &id);		/* Get id of this process */

  if (ntasks != 2) {
    if (id == 0) printf("You have to run this program with 2 processes\n");
    MPI_Finalize();
    exit(0);
  }

  if (id == 0) {			       	/* Process 0 does this */
    printf("Enter a, b and n: ");
    fflush(stdout);                 /* Force stdout to display */
    scanf("%f %f %d", &indata.a, &indata.b, &indata.n);  /* Read input */
  }

  lengtharray[0] = lengtharray[1] =lengtharray[2] = 1;  /* Set array of lengths */
  typearray[0] = typearray[1] = MPI_FLOAT;              /*   and data types */
  typearray[2] = MPI_INT;

  /* First element, a, is at displacement 0 */
  disparray[0] = 0;

  /* Calculate displacement of b */
  MPI_Address(&indata.a, &startaddress);
  MPI_Address(&indata.b, &address);
  disparray[1] = address-startaddress;     /* Displacement of second element, b */

  MPI_Address(&indata.n, &address);
  disparray[2] = address-startaddress;     /* Displacement of third element, n */

  /* Build the data structure my_type */
  MPI_Type_struct(3, lengtharray, disparray, typearray, &my_type);
  MPI_Type_commit(&my_type);


  if (id==0) {       /* Process 0 does this */
    printf("Sending input data to process 1\n");
    MPI_Send(&indata, 1, my_type, 1, tag, MPI_COMM_WORLD);
    MPI_Recv(&recdata, 1, my_type, 1, tag, MPI_COMM_WORLD, &status);
    printf("Received message with values %3.1f  %3.1f  and %d from process %d\n", \
	   recdata.a, recdata.b, recdata.n, status.MPI_SOURCE);
  }
  else {       /* Process 1 does this */
    MPI_Recv(&indata, 1, my_type, 0, tag, MPI_COMM_WORLD, &status);
    MPI_Send(&indata, 1, my_type, 0, tag, MPI_COMM_WORLD);
  }

  err = MPI_Finalize();	/* Terminate MPI */
  exit(0);
}
