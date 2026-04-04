#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank, size, value;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    double start = MPI_Wtime();

    if (rank == 0) {
        value = 100;
        printf("Process 0 starts with value: %d\n", value);
        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 received final value back: %d\n", value);
    } else {
        MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value += rank;
        printf("Process %d sending value: %d to Process %d\n", rank, value, next);
        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    double end = MPI_Wtime();

    if (rank == 0)
        printf("Execution Time with %d process(es): %f seconds\n", size, end - start);

    MPI_Finalize();
    return 0;
}
