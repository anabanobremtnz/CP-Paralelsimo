#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>

#define DEBUG 1

#define N 1024

int main(int argc, char *argv[] ) {

	int i, j, numprocs, rank, tcomm, tcomp;
	float matrix[N][N];	
    float vector[N];
    float result[N];
	struct timeval  tv1, tv2, tv3, tv4;

	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int m = ceil((float)N / numprocs);	// Calcular un numero enteiro de filas para cada proc
	float mat[m][N]; 	// Matriz local dividida
    float res[m];		// Resultado local de cada proceso antes da recolecion
	double avgcomm, avgcomp;

    // O proceso 0 inicializa o vector e a matriz
	if (rank == 0){
		for(i=0;i<N;i++) {
        	vector[i] = i;
			for(j=0;j<N;j++) {
			matrix[i][j] = i+j;
			}
		}
	}

    gettimeofday(&tv1, NULL); // Comezo comunicacion

    MPI_Bcast(vector, N, MPI_FLOAT, 0, MPI_COMM_WORLD); // Difusion do vector a todos os procesos
	MPI_Scatter(matrix, m*N, MPI_FLOAT, mat, m*N, MPI_FLOAT, 0, MPI_COMM_WORLD); // Distribucion dos m*N elementos da matriz a cada proceso

    gettimeofday(&tv2, NULL); // Fin comunicacion / comezo computacion
    
	// Gardar resultado da operacion localmente
    for(i=0;i<m;i++) {
        res[i]=0;
        for(j=0;j<N;j++) {
        res[i] += mat[i][j]*vector[j];
        }
    }

    gettimeofday(&tv3, NULL); // Comezo comunicacion / fin computacion

    MPI_Gather(res, m, MPI_FLOAT, result, m, MPI_FLOAT, 0, MPI_COMM_WORLD); // Recolecion dos resultados no vector result

    gettimeofday(&tv4, NULL); // Fin comunicacion

    int microsecondscomm = (tv2.tv_usec - tv1.tv_usec) + 1000000 * (tv2.tv_sec - tv1.tv_sec) + (tv4.tv_usec - tv3.tv_usec)+ 1000000 * (tv4.tv_sec - tv3.tv_sec); // Tempo total de comunicacion
    int microsecondscomp = (tv3.tv_usec - tv2.tv_usec) + 1000000 * (tv3.tv_sec - tv2.tv_sec); // Tempo de computacion

    printf ("\n %d -> Computing time (seconds) = %lf and communication time (seconds) = %lf\n", rank, (double) microsecondscomp/1E6, (double) microsecondscomm/1E6);

	// Calcular suma dos tempos para a media
    MPI_Reduce(&microsecondscomm,&tcomm, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&microsecondscomp,&tcomp, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
            
    /* Display result */
    if (rank == 0) {  
		if (DEBUG) {
			printf("\n");
        	for(i=0;i<N;i++) {
        	printf(" %f \t ",result[i]);
     	   }
			printf("\n");
		} else {
			// Calcular a media dos tempos de todos os procesos
			printf("\n");
			avgcomp = (double) tcomp / numprocs;
			avgcomm = (double) tcomm / numprocs;
			printf("Avg. computing time (seconds) = %lf\n", avgcomp/1E6);
			printf("Avg. communication time (seconds) = %lf\n", avgcomm/1E6);
		}
    }    

    MPI_Finalize();
    return 0;
}

