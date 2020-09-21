#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000000
#define MAX_THREADS 32

typedef struct { double val; int loc; char pad[128]; } tvals;

int main(int argc, char *argv[])
{
    double x[N];

    //srand(time(0));
    srand(0);
    for (int i =0; i < N; i++) {
        x[i] = ((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*1000;
    }

    tvals maxinfo[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) { maxinfo[i].val = -1.0e30; maxinfo[i].loc = -1; }
    double mval = -1.0e30;
    int mloc = -1;

    double start_time = omp_get_wtime();

#pragma omp parallel shared(maxinfo)
    {
        int id = omp_get_thread_num();
        maxinfo[id].val = -1.0e30;
#pragma omp for
        for (int i = 0; i < N; i++) {
            if (x[i] > maxinfo[id].val) {
                 maxinfo[id].val = x[i]; maxinfo[id].loc = i;
            }

        }
    }

    for (int i = 0; i < MAX_THREADS; i++) {
            if (maxinfo[i].val > mval) {
                 mval = maxinfo[i].val; mloc = maxinfo[i].loc;
            }
    }
    double end_time = omp_get_wtime();

    printf("maxloc: %d ; maxval: %f\n", mloc, mval);
    printf("Elapsed: %f\n", end_time - start_time);

    return 0;
}
