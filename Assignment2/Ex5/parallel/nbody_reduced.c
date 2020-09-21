#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#define TOTAL_STEPS 100
#define NUM_PARTICLES 2000
#define DIM 3
#define X 0
#define Y 1
#define Z 2

double delta_t = 0.05;
//double delta_t = 10.5;
double G = 6.673 * 10e-11;

int main(int argc, char *argv[])
{
printf("threads: %d\n", omp_get_max_threads());
    double       pos[NUM_PARTICLES][DIM];
    double   old_pos[NUM_PARTICLES][DIM];
    double       vel[NUM_PARTICLES][DIM];
    double    forces[NUM_PARTICLES][DIM];
    double    masses[NUM_PARTICLES];

    for (int q = 0; q < NUM_PARTICLES; q++) {
        pos[q][X] = (rand() / (double)(RAND_MAX)) * 2 - 1;
        pos[q][Y] = (rand() / (double)(RAND_MAX)) * 2 - 1;
        pos[q][Z] = (rand() / (double)(RAND_MAX)) * 2 - 1;
        
        old_pos[q][X] = pos[q][X];
        old_pos[q][Y] = pos[q][Y];
        old_pos[q][Z] = pos[q][Z];
        
        //vel[q][X] = (rand() / (double)(RAND_MAX)) * 2 - 1;
        //vel[q][Y] = (rand() / (double)(RAND_MAX)) * 2 - 1;
        //vel[q][Z] = (rand() / (double)(RAND_MAX)) * 2 - 1;
        vel[q][X] = 0.0;
        vel[q][Y] = 0.0;
        vel[q][Z] = 0.0;
        
        masses[q] = fabs((rand() / (double)(RAND_MAX)) * 2 - 1);
    }

    double start_time = omp_get_wtime();
    for (int step = 1; step <= TOTAL_STEPS; step++) {
        /* compute vel */
        memset(forces, 0, NUM_PARTICLES * DIM * sizeof(double));
#pragma omp parallel for schedule(static, 15)
        for (int q = 0; q < NUM_PARTICLES; q++) {
            for (int k = q + 1; k < NUM_PARTICLES; k++) {
                    double force_qk[DIM] = { 0.0 };

                    double x_diff = pos[q][X] - pos[k][X]; 
                    double y_diff = pos[q][Y] - pos[k][Y]; 
                    double z_diff = pos[q][Z] - pos[k][Z]; 

                    double dist = sqrt(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff); 
                    double dist_cubed = dist * dist * dist; 

                    force_qk[X] = G * masses[q] * masses[k] / dist_cubed * x_diff;
                    force_qk[Y] = G * masses[q] * masses[k] / dist_cubed * y_diff;
                    force_qk[Z] = G * masses[q] * masses[k] / dist_cubed * z_diff;

                    forces[q][X] += force_qk[X];
                    forces[q][Y] += force_qk[Y];
                    forces[q][Z] += force_qk[Z];

                    forces[k][X] -= force_qk[X];
                    forces[k][Y] -= force_qk[Y];
                    forces[k][Z] -= force_qk[Z];
            }
        }
    
        /* particle mover */
#pragma omp parallel for schedule(static, 15)
        for (int q = 0; q < NUM_PARTICLES; q++) {
            pos[q][X] += delta_t * vel[q][X];
            pos[q][Y] += delta_t * vel[q][Y];
            pos[q][Z] += delta_t * vel[q][Z];
            vel[q][X] += delta_t / masses[q] * forces[q][X];
            vel[q][Y] += delta_t / masses[q] * forces[q][Y];
            vel[q][Z] += delta_t / masses[q] * forces[q][Z];
        }
    }
    printf("%d\t%f\t%f\t%f\n", 1, pos[0][0], pos[1][0], pos[2][0]);
    double end_time  = omp_get_wtime(); 
    printf("time: %f\n", end_time - start_time);

    return 0;
}
