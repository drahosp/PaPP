/*
A simle example program using MPI - Hello World

The program consists of one receiver process and N-1 sender
processes. The sender processes send a message consisting
of their process identifier (id) and their hostname to the receiver.
The receiver process prints out the values it receives in the
messages.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  const int tag = 42;	        /* Message tag */
  int   id, ntasks, source_id, err, i;
  MPI_Status status;
  char msg[80];	      	       /* Message array */
  
  err = MPI_Init(&argc, &argv); /* Initialize MPI */
  if (err != MPI_SUCCESS) {
    printf("MPI_init failed!\n");
    exit(1);
  }

  err = MPI_Comm_size(MPI_COMM_WORLD, &ntasks);	/* Get nr of tasks */
  if (err != MPI_SUCCESS) {
    printf("MPI_Comm_size failed!\n");
    exit(1);
  }

  err = MPI_Comm_rank(MPI_COMM_WORLD, &id);    /* Get id of this process */
  if (err != MPI_SUCCESS) {
    printf("MPI_Comm_rank failed!\n");
    exit(1);
  }

  /* Check that we run on at least two processors */
  if (ntasks < 2) {
    printf("You have to use at least 2 processors to run this program\n");
    MPI_Finalize();	       /* Quit if there is only one processor */
    exit(0);
  }

  /* Process 0 (the receiver) does this */
  if (id == 0) {
    int length;
    /* First print own greeting message */
    MPI_Get_processor_name(msg, &length);  /* Get name of this processor */
    printf("Hello World from process %d running on %s\n", id, msg);
    /* Then receive and print greetings from all other processes */
    for (i=1; i<ntasks; i++) {
      err = MPI_Recv(msg, 80, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, \
		     &status);          /* Receive a message from any sender */
      if (err != MPI_SUCCESS) {
	printf("Error in MPI_Recv!\n");
	exit(1);
      }
      source_id = status.MPI_SOURCE;	         /* Get id of sender */
      printf("Hello World from process %d running on %s\n", source_id, msg);
    }
  }

  /* Processes 1 to N-1 (the senders) do this */
  else {
    int length;
    MPI_Get_processor_name(msg, &length);  /* Get name of this processor */
    err = MPI_Send(msg, length, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    if (err != MPI_SUCCESS) {
      printf("Process %i: Error in MPI_Send!\n", id);
      exit(1);
    }
  }
  
  err = MPI_Finalize();	         /* Terminate MPI */
  if (err != MPI_SUCCESS) {
    printf("Error in MPI_Finalize!\n");
    exit(1);
  }
  if (id==0) printf("Ready\n");
  exit(0);
}
