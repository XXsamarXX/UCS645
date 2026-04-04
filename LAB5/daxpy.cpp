#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

#define N (1<<16)

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 2.5;
    int local_n = N / size;

    vector<double> X(local_n, 1.0);
    vector<double> Y(local_n, 2.0);

    double start = MPI_Wtime();

    for(int i = 0; i < local_n; i++) {
        X[i] = a * X[i] + Y[i];
    }

    double end = MPI_Wtime();

    if(rank == 0) {
        cout << "Execution Time: " << end - start << endl;
    }

    MPI_Finalize();
    return 0;
}