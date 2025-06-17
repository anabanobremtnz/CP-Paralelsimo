#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

// Recolección usando FlatTree: proceso root recibe datos de todos os demáis procesos e realiza a suma. Os resultados reducidos envíanse de volta a todos os procesos.
 int MPI_FlattreeColectiva(const void *sendbuff, void *recvbuff, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) {
	int rank, numprocs, d;
	double *recv;

	// Variables MPI
    MPI_Comm_size(comm, &numprocs);
    MPI_Comm_rank(comm, &rank);

	// Proceso raiz envia unha mensaxe por cada proceso k usando comm
    if (rank == root) {
        recv = malloc(sizeof(double) * count);
		memcpy(recvbuff, sendbuff, count*sizeof(double));
mpirun -np 4 ./main
		for(int i = 0; i < numprocs; i++) {
			if(i == root)
				continue;

			d = MPI_Recv(recv, count, datatype, i, 1, comm, MPI_STATUS_IGNORE);

            // Os recibidos súmanse aos recv previos
			for(int j = 0; j < count; j++) {
				((double *)recvbuff)[j] += recv[j];
			}
		}
		free(recv);
    } else {
		// Procesos non raiz envian os datos a ser reducidos
        d = MPI_Send(sendbuff, count, datatype, root, 1, comm);
    }
	return d;
}

// Difusión binomial. Cada proceso envía os seus datos ao proceso, e cada proceso recibe os datos dun proceso ata que todos teñen os datos.
int MPI_BinomialCast(void *buff, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    int numprocs, rank, d;

    MPI_Comm_size(comm, &numprocs);
    MPI_Comm_rank(comm, &rank);

    if(rank > 0)
        d = MPI_Recv(buff, count, datatype, MPI_ANY_SOURCE, 33, comm, MPI_STATUS_IGNORE);

    for(int i=1; ; i*= 2){
        if(rank < i){
            if(i+rank >= numprocs)
                break;
            d = MPI_Send(buff, count, datatype, i+rank, 33, comm);
        }
    }

    return d;
}

int main(int argc, char *argv[])
{
    int i, n;
    double PI25DT = 3.141592653589793238462643;
    double pi, h, sum, x, pirec;

    // Variables auxiliares
    int numprocs, rank;

	// Variables para MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    while (1)
    {
		// Dado o primeiro proceso, fai sends, print e scan
        if (rank == 0) {
            printf("Enter the number of intervals: (0 quits) \n");
            scanf("%d",&n);
        }

        // Enviar e recibir n
		// MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_BinomialCast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (n == 0) break;

        h   = 1.0 / (double) n;
        sum = 0.0;

		// Dado o bucle, modificado para ser relativo a numprocs
        for (i = rank + 1; i <= n; i += numprocs) {
            // get result
            x = h * ((double)i - 0.5);
            sum += 4.0 / (1.0 + x*x);
        }
        pi = h * sum;

		// Enviar y recibir pi
		// MPI_Reduce(&pi, &piBuf, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_FlattreeColectiva(&pi, &pirec, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0)
            printf("pi is approximately %.16f, Error is %.16f\n", pirec, fabs(pirec - PI25DT));
    }
	// Finalizar todos os procesos
    MPI_Finalize();
}