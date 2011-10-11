/* 
   This program demonstrates how to build communicators using
   MPI_Comm_split. For each process it builds a new communicator
   for the processes in the same row and column in a two-dimensional
   process structure.
   You have to use a square number of processes to execute the
   program, for instance 4 or 9 processes.

   Compile the program with  'mpicc rowcol.c -o rowcol'
   To run the program, on four processes do: 'mpiexec -n 4 rowcol
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int id, ntasks, q, i;
  int row, col;
  int ntasks_row, id_row;       /* Nr of processes and rank in the row */
  int id_col;                   /* Rank in the column communicator */
  MPI_Comm row_comm, col_comm;  /* Communicators for row and column */
  int X[10];                    /* Send buffer */
  
  MPI_Init(&argc, &argv);	/* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);	/* Get nr of tasks */
  MPI_Comm_rank(MPI_COMM_WORLD, &id);	/* Get id of this process */

  /* The process grid will be of size q*q */
  q = (int) sqrt((double) ntasks);

  /* Check that we have a square number of processes */
  if (q*q != ntasks) {
    if (id == 0) {
      printf("You have to use a square number of processes\n");
      printf("Quitting\n");
    }
    MPI_Finalize();
    exit(1);
  }

  /* Calculate on which row and column this process is */
  row = id/q;
  col = id%q;

  /* Build communicator for processes in the same row */
  MPI_Comm_split(MPI_COMM_WORLD, row, id, &row_comm);

  /* Build communicator for processes in the same column */
  MPI_Comm_split(MPI_COMM_WORLD, col, id, &col_comm);

  /* Get nr of tasks in the row communicator */
  MPI_Comm_size(row_comm, &ntasks_row);

  /* Get own rank in row communicator */
  MPI_Comm_rank(row_comm, &id_row);
  
  /* Get own rank in row communicator */
  MPI_Comm_rank(col_comm, &id_col);

  printf("Process %d has rank %d in its row communicator ", id, id_row);
  printf("and %d in the column communicator\n", id_col);

  /* The first process in each row assigns values to X */
  if (id_row==0) {
    for (i=0; i<10; i++) X[i]=i;  
  }
  MPI_Bcast(&X, 10, MPI_INT, 0, row_comm);

  MPI_Finalize();	         /* Terminate MPI */
  if (id==0) printf("Ready\n");
  exit(0);
}
