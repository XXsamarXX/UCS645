#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(rank * 1000 + time(NULL));
    int nums[10];
    int local_max = 0, local_min = 1000;
    for (int i = 0; i < 10; i++) {
        nums[i] = rand() % 1001;
        if (nums[i] > local_max) local_max = nums[i];
        if (nums[i] < local_min) local_min = nums[i];
    }

    double start = MPI_Wtime();

    struct { int val; int rank; } send_max, recv_max;
    struct { int val; int rank; } send_min, recv_min;
    send_max.val = local_max; send_max.rank = rank;
    send_min.val = local_min; send_min.rank = rank;

    MPI_Reduce(&send_max, &recv_max, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    MPI_Reduce(&send_min, &recv_min, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);
    printf("Process %d: local max = %d, local min = %d\n", rank, local_max, local_min);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Global Maximum = %d (found in Process %d)\n", recv_max.val, recv_max.rank);
        printf("Global Minimum = %d (found in Process %d)\n", recv_min.val, recv_min.rank);
        printf("Execution Time with %d process(es): %f seconds\n", size, end - start);
    }

    MPI_Finalize();
    return 0;
}
