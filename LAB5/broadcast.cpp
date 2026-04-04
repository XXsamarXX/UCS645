#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

#define N 10000000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<double> data(N);

    double start, end;

    // 🔹 Custom Broadcast
    if(rank == 0) {
        for(int i=0;i<N;i++) data[i]=i;
    }

    start = MPI_Wtime();

    if(rank == 0) {
        for(int i=1;i<size;i++)
            MPI_Send(data.data(), N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(data.data(), N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    end = MPI_Wtime();

    if(rank==0)
        cout<<"MyBcast Time: "<<end-start<<endl;

    // 🔹 MPI_Bcast
    start = MPI_Wtime();

    MPI_Bcast(data.data(), N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    end = MPI_Wtime();

    if(rank==0)
        cout<<"MPI_Bcast Time: "<<end-start<<endl;

    MPI_Finalize();
}