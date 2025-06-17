#include <stdio.h>
#include <sys/time.h>

#define DEBUG 1

#define N 1024

int main(int argc, char *argv[] ) {

    int i, j;
    float matrix[N][N];
    float vector[N];
    float result[N];
    struct timeval  tv1, tv2;

    // Initialize Matrix and Vector con valores ata 1024
    for(i=0;i<N;i++) {
        vector[i] = i;
        for(j=0;j<N;j++) {
        matrix[i][j] = i+j;
        }
    }

    gettimeofday(&tv1, NULL);

    // Facer acumulacion en cada pos da multiplicacion de cada elemento do vector e da matriz mais o num da posicion
    for(i=0;i<N;i++) {
        result[i]=0;
        for(j=0;j<N;j++) {
        result[i] += matrix[i][j]*vector[j];
        }
    }

    gettimeofday(&tv2, NULL);
        
    int microseconds = (tv2.tv_usec - tv1.tv_usec)+ 1000000 * (tv2.tv_sec - tv1.tv_sec); // calcular tempo

    /* Display result */
    if (DEBUG){
        for(i=0;i<N;i++) {
        printf(" %f \t ",result[i]);
        }
        printf("\n");
    } else {
        printf ("Time (seconds) = %lf\n", (double) microseconds/1E6); // tempo secuencial: 0.0061
    }    

    return 0;
}