/*
  MPI example program that computes the value of Pi using a Monte Carlo
  simulation. The program samples points inside the rectangle delimited
  by (0,0) and (1,1) and counts how many of these are within a circle
  with a radius of 1. The ratio between the number of points inside the
  circle and the total number of samples is Pi/4. All processes use
  different random number sequences.
*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main(int argc,char *argv[])
{
  const double PI24 = 3.141592653589793238462643;

  int n,  myid, numprocs, i;
  double pi;
  double sum_n, in_circle, sum_c;
  double starttime, endtime;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);

  srand(myid);          /* Seed the random number generator */

  /* Read the number of random samples in each process */
  if (myid == 0) {
    printf("Give number of samples in each process: ");
    fflush(stdout);
    scanf("%d", &n);
  }
  /* Send n to all proceses */
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (myid == 0)
    printf("Number of samples is %d\n", n);
  fflush(stdout);

  /* Start measuring time */
  if (myid == 0) starttime = MPI_Wtime();

  in_circle = 0.0;

  /* Draw n random points and count how many are inside the circle */
  for (i=0; i<n; i++) {
    double x, y, dist;
    x = (double)rand()/RAND_MAX; /* Draw a random point */
    y = (double)rand()/RAND_MAX;
    dist = sqrt(x*x+y*y);        /* Compute distance of the point (x,y) to origo */
    if (dist<=1.0) in_circle++;  /* Count how many points are inside the circle */
  }

  /* Sum all the samples inside the circle */
  MPI_Reduce(&in_circle, &sum_c, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (myid == 0) {
    sum_n = (double) (n*numprocs);  /* Total nr of random points */
    pi = (sum_c/sum_n)*4.0;
    endtime = MPI_Wtime();
    printf("The computed value of Pi is %2.24f\n", pi);
    printf("The  \"exact\" value of Pi is %2.24f\n", PI24);
    printf("The difference is %e\n", fabs(PI24-pi));
    printf("Wall clock time = %f s\n", endtime-starttime);
    fflush(stdout);
  }

  MPI_Finalize();
  exit(0);
}
