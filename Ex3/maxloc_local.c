#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000000
#define MAX_THREADS 32

int main(int argc, char *argv[])
{
    double x[N];

    srand(0);
    for (int i =0; i < N; i++) {
        x[i] = ((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*1000;
    }

    int maxloc[MAX_THREADS] = { -1 }, mloc = -1;
    double maxval[MAX_THREADS] = { -1.0e30 }, mval = -10e30;

    double start_time = omp_get_wtime();

#pragma omp parallel shared(maxval, maxloc)
    {
        int id = omp_get_thread_num();
        maxval[id] = -1.0e30;
#pragma omp for
        for (int i = 0; i < N; i++) {
            if (x[i] > maxval[id]) {
                 maxval[id] = x[i]; maxloc[id] = i;
            }

        }
    }

    for (int i = 0; i < MAX_THREADS; i++) {
            if (maxval[i] > mval) {
                 mval = maxval[i]; mloc = maxloc[i];
            }
    }
    double end_time = omp_get_wtime();

    printf("maxloc: %d ; maxval: %f\n", mloc, mval);
    printf("Elapsed: %f\n", end_time - start_time);

    return 0;
}
