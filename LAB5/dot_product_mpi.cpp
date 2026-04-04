#include <mpi.h>
#include <iostream>

using namespace std;

#define N 50000000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = N / size;

    double multiplier = 2.0;

    MPI_Bcast(&multiplier, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double local_sum = 0;

    double start = MPI_Wtime();

    for(int i=0;i<local_n;i++) {
        double A = 1.0;
        double B = 2.0 * multiplier;
        local_sum += A * B;
    }

    double global_sum;

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if(rank==0) {
        cout<<"Dot Product: "<<global_sum<<endl;
        cout<<"Time: "<<end-start<<endl;
    }

    MPI_Finalize();
}