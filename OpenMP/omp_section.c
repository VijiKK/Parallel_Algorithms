#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{
   omp_set_num_threads(8);
#pragma omp parallel
    {
        #pragma omp master // nowait
        {
            printf("%d section single\n", omp_get_thread_num());
        }

        #pragma omp sections
        {

#pragma omp section
            {
                printf("%d section 1\n", omp_get_thread_num());
            }

#pragma omp section
            {
                printf("%d section 2\n", omp_get_thread_num());
            }

#pragma omp section
            {
                printf("%d section 3\n", omp_get_thread_num());
            }

        }        
    }
    return 0;

}
