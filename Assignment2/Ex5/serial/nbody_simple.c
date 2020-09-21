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
        for (int q = 0; q < NUM_PARTICLES; q++) {
            for (int k = 0; k < NUM_PARTICLES; k++) {
                if (k != q) {
                    double x_diff = pos[q][X] - pos[k][X]; 
                    double y_diff = pos[q][Y] - pos[k][Y]; 
                    double z_diff = pos[q][Z] - pos[k][Z]; 
                    double dist = sqrt(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff); 
                    double dist_cubed = dist * dist * dist; 
                    forces[q][X] -= G * masses[q] * masses[k] / dist_cubed * x_diff; 
                    forces[q][Y] -= G * masses[q] * masses[k] / dist_cubed * y_diff; 
                    forces[q][Z] -= G * masses[q] * masses[k] / dist_cubed * z_diff; 
                }
            }
        }
    
        /* particle mover */
        for (int q = 0; q < NUM_PARTICLES; q++) {
            pos[q][X] += delta_t * vel[q][X];
            pos[q][Y] += delta_t * vel[q][Y];
            pos[q][Z] += delta_t * vel[q][Z];
            vel[q][X] += delta_t / masses[q] * forces[q][X];
            vel[q][Y] += delta_t / masses[q] * forces[q][Y];
            vel[q][Z] += delta_t / masses[q] * forces[q][Z];
            //if (step==1 && q==6 /*isnan(pos[q][Y])*/) {
            //printf("%d\t%f\t%f\t%f\n", q, pos[q][X], pos[q][Y], pos[q][Z]);
            //printf("step=%d\tpos(%f,%f)\tvel(%f,%f)\tforces(%f,%f)\tmasses(%f)\n", q, step, pos[q][X], pos[q][Y], vel[q][X],vel[q][Y], forces[q][X], forces[q][Y], masses[q]);
            //    //exit(1);
            //}
        }
    }
    printf("%d\t%f\t%f\t%f\n", 1, pos[0][0], pos[1][0], pos[2][0]);
    double end_time  = omp_get_wtime(); 
    printf("time: %f\n", end_time - start_time);

    return 0;
}
