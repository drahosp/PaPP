/*
  MPI program where each process draws 1000000 random numbers between 0 and 1
  and adds the values together and computes the mean value. The program uses
  MPI_Reduce to sum the values from each process.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  double r, maxval, sum, mean;
  double sum_total, mean_total;
  int id, ntasks, i, n;

  /* All proceses do exactly the same computation */
  
  MPI_Init(&argc, &argv);                  /* Initialize MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);  /* Get nr of tasks */
  MPI_Comm_rank(MPI_COMM_WORLD, &id);      /* Get id of this process */

  n = 1000000;           /* Number of random values in each process */
  sum = 0.0;
  maxval = pow(2,31)-1;  /* 2 to the power of 31 minus 1 */
  srandomdev();          /* Seed the random number generator */

  /* Draw random numbers and compute their sum and mean value */
  for (i=0; i<n; i++) {
    r = (double)random()/maxval;   /* Random number between 0 and 1 */
    sum += r;                      /* Add the values together */
  }
  mean = sum/(double)n;       /* Compute the mean value */

  /* Sum the values in all processes with a reduction operation */
  MPI_Reduce(&sum, &sum_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce(&mean, &mean_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (id == 0) {
    printf("The total sum is %f and mean is %f \n", sum_total, mean_total/(double)ntasks);
    printf("Expected sum is %f and mean %f \n", n*ntasks*0.5, 0.5);
    printf("The difference is %f and %f \n", sum_total-(n*ntasks*0.5), 
	   mean_total/(double)ntasks-0.5);
  }

  MPI_Finalize();	         /* Terminate MPI */
  if (id==0) printf("Ready\n");
  exit(0);
}
