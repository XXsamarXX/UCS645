#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 8;
    int chunk = n / size;
    int A_full[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int B_full[] = {8, 7, 6, 5, 4, 3, 2, 1};

    int *localA = (int*)malloc(chunk * sizeof(int));
    int *localB = (int*)malloc(chunk * sizeof(int));

    double start = MPI_Wtime();

    MPI_Scatter(A_full, chunk, MPI_INT, localA, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B_full, chunk, MPI_INT, localB, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    long long local_dot = 0;
    for (int i = 0; i < chunk; i++)
        local_dot += (long long)localA[i] * localB[i];

    long long global_dot = 0;
    MPI_Reduce(&local_dot, &global_dot, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);
    printf("Process %d local dot: %lld\n", rank, local_dot);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Global Dot Product = %lld (Expected: 120)\n", global_dot);
        printf("Execution Time with %d process(es): %f seconds\n", size, end - start);
    }

    free(localA);
    free(localB);
    MPI_Finalize();
    return 0;
}
