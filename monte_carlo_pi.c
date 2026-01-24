#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank, size; MPI_Comm_rank(MPI_COMM_WORLD,&rank); MPI_Comm_size(MPI_COMM_WORLD,&size);
    long long N = (argc>1) ? atoll(argv[1]) : 100000000; // Total number of samples
    long long localN = N/size + (rank < N%size ? 1 : 0);

    // Initialize random seed for each process
    srand(1234u + rank * 777);

    long long hit = 0;
    for (long long i=0;i<localN;i++){
        double x = rand()/(double)RAND_MAX;
        double y = rand()/(double)RAND_MAX;
        if (x*x + y*y <= 1.0) hit++;
    }

    long long total_hit;
    MPI_Reduce(&hit, &total_hit, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank==0){
        double pi = 4.0 * (double)total_hit / (double)N;
        printf("pi = %.8f\n", pi);
    }
    MPI_Finalize();
    return 0;
}
