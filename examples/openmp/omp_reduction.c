/* 
   OpenMP example program which computes the dot product of two arrays a and b
   (that is sum(a[i]*b[i]) ) using a sum reduction.
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000

int main (int argc, char *argv[]) {
  
  double a[N], b[N];
  double sum = 0.0;
  int i;
  
  /* Initialization */
#pragma omp parallel for shared(a,b) private(i)
  for (i=0; i < N; i++)
    a[i] = b[i] = (double)i;

  /* Parallel for loop computing the sum of all products */
#pragma omp parallel for shared(a, b) private(i) reduction(+:sum)
  for (i=0; i < N; i++) {
    sum = sum + (a[i] * b[i]);
  }

  printf("   Sum = %2.1f\n",sum);
  exit(0);
}
