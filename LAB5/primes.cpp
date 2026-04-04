#include <mpi.h>
#include <iostream>
#include <vector>
using namespace std;

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int max_val = 1000;
    int chunk = max_val / size;
    int start = rank * chunk + 1;
    int end = (rank == size - 1) ? max_val : start + chunk - 1;

    double t_start = MPI_Wtime();

    vector<int> local_primes;
    for (int i = start; i <= end; i++)
        if (isPrime(i)) local_primes.push_back(i);

    int local_count = local_primes.size();
    int global_count = 0;
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double t_end = MPI_Wtime();

    if (rank == 0) {
        printf("Total primes found: %d\n", global_count);
        printf("Execution Time with %d process(es): %f seconds\n", size, t_end - t_start);
    }

    MPI_Finalize();
    return 0;
}
