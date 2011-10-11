/*
 Basic example of using OpenMP. This example executes the main function body
 multiple times depending on the number of processors available
*/

#include <stdio.h>
#include <omp.h>

int main()
{
    int id;
    #pragma omp parallel private(id)
    {
        id = omp_get_thread_num();
        printf("%d: Hello World!\n", id);
    }
    return 0;
}
