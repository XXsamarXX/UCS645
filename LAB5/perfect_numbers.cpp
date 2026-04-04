#include <mpi.h>
#include <iostream>
using namespace std;

bool isPerfect(int n){
    int sum=1;
    for(int i=2;i<=n/2;i++)
        if(n%i==0) sum+=i;
    return sum==n;
}

int main(int argc,char** argv){
    MPI_Init(&argc,&argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int num = rank + 2;

    if(isPerfect(num))
        cout<<"Perfect: "<<num<<endl;

    MPI_Finalize();
}