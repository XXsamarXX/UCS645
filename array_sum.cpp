#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 100;
    int chunk = n / size;
    int *array = NULL;

    if (rank == 0) {
        array = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) array[i] = i + 1;
    }

    double start = MPI_Wtime();

    int *local = (int*)malloc(chunk * sizeof(int));
    MPI_Scatter(array, chunk, MPI_INT, local, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    long long local_sum = 0;
    for (int i = 0; i < chunk; i++) local_sum += local[i];
    printf("Process %d local sum: %lld\n", rank, local_sum);

    long long global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Global Sum = %lld\n", global_sum);
        printf("Average    = %.2f\n", (double)global_sum / n);
        printf("Execution Time with %d process(es): %f seconds\n", size, end - start);
    }

    free(local);
    if (rank == 0) free(array);
    MPI_Finalize();
    return 0;
}
