#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000000

int main(int argc, char *argv[])
{
    printf("Threads: %d\n", omp_get_max_threads());
    double x[N];

    //srand(time(0));
    srand(0);
    for (int i =0; i < N; i++) {
        x[i] = ((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*1000;
    }

    double maxval = 0.0; int maxloc = 0;

    double start_time = omp_get_wtime();
#pragma omp parallel for
    for (int i=0; i < N; i++){
        if (x[i] > maxval) {
             maxval = x[i]; maxloc = i;
        }
    }
    double end_time = omp_get_wtime();

    printf("maxloc: %d ; maxval: %f\n", maxloc, maxval);
    printf("Elapsed: %f\n", end_time - start_time);

    return 0;
}
