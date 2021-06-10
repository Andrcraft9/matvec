#include <mpi.h>
#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>

#define _N_ 9216             // matrix dimension and vector size
#define _M_ 1000             // just a number of repetitions
#define _ETALON_NORM_ 20580.1167

int main (int argc, char **argv)
{
    int id, np;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &np);
    const int th = omp_get_max_threads();

    const int n = _N_ / np; // local number of vector components and matrix rows
    double *a      = malloc(n * _N_ * sizeof(double)); // local rows of matrix A
    double *x      = malloc(n       * sizeof(double)); // local components of vector x
    double *y      = malloc(n       * sizeof(double)); // local components of vector y
    double *x_glob = malloc(    _N_ * sizeof(double)); // working vector with global X

    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < _N_; j++)
            {
                a[i*_N_ + j] = 1. / (1. + i+id*n + j); // global i and j
            }
        }

        #pragma omp for nowait
        for (int i = 0; i < n; i++)
        {
            x[i] = i+id*n; // local components of vector x
            y[i] = 0.;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double T = MPI_Wtime();

    MPI_Allgather(x, n, MPI_DOUBLE, x_glob, n, MPI_DOUBLE, MPI_COMM_WORLD); 

    #pragma omp parallel
    {
        for (int k = 0; k < _M_; k++) 
        {
            #pragma omp for nowait
            for (int i = 0; i < n; i++) 
            {
                double *ai = a + i * _N_; // address of i-th matrix row
                for (int j = 0; j < _N_; j++)
                {
                    y[i] += ai[j] * x_glob[j];
                }
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    T = MPI_Wtime() - T;
    double perf = 1e-6 * _N_ * _N_ * _M_ / T;

    // Compute norm of vector Y here: sum=||Y||
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++)
    {
        sum += y[i] * y[i];
    }

    double sum_glob = 0.0;
    MPI_Allreduce(&sum, &sum_glob, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    sum_glob = sqrt(sum_glob) / _M_;

    if (id == 0)
    {
       printf("MPI-1p mvm: N=%d np=%d th=%d norm=%lf (etalon=%lf, diff=%lf) time=%lf perf=%.2lf MFLOPS\n", 
              _N_, np, th, sum_glob, _ETALON_NORM_, fabs(sum_glob - _ETALON_NORM_), T, perf);
    }

    MPI_Finalize();
    return 0;
}
