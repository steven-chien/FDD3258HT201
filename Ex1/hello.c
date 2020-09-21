#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{
#pragma omp parallel
    {
        printf("Hello World from Thread %d!\n", omp_get_thread_num());
    }
}
