#include <stdio.h>
#include <omp.h>


int main(int argc, char *argv[])
{
   omp_set_num_threads(3);
  #pragma omp parallel
    {

// #pragma omp for nowait -- remove the implicit barrier
#pragma omp for schedule(dynamic) 
        for (int i=0; i<20; i++)
        {
            printf("thread %d -- iteration %d\n", omp_get_thread_num(), i);
        }

        //printf("the end\n");
    }
    
    return 0;
}

