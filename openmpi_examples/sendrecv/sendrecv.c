/*
	from lecture, simple send and receive once -- jd
	adapted from
	http://mpi.deino.net/mpi_functions/MPI_Send.html
*/


#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, i;
    int buffer[10];
    MPI_Status status;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        for (i=0; i<10; i++)
            buffer[i] = i;
        MPI_Send(buffer, 10, MPI_INT, 1, 123, MPI_COMM_WORLD);
    }
    if (rank == 1)
    {
        for (i=0; i<10; i++)
            buffer[i] = -1;
        MPI_Recv(buffer, 10, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
        for (i=0; i<10; i++)
        {
            if (buffer[i] != i)
                printf("Error: buffer[%d] = %d but is expected to be %d\n", i, buffer[i], i);
        }
        fflush(stdout);
    }
    printf("Rank %d of %d processes.\n", rank, size);
    MPI_Finalize();
    return 0;
}

