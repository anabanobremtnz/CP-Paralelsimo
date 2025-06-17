# Práctica 3: Descomposición de dominio

## Explicación
O obxectivo das operacións computacionais son ter no vector result[N], en cada unha das súas posicións, a acumulación da multiplicación de cada elemento da matriz por cada elemento do vector máis a posición que corresponde. É dicir, para result[0] a operación é a suma comezando pr N = 0 de matriz[0][N] * vector[N] + 0, para a seguinte posición, result[1], sería matriz[1][N] * vector[N] + 1, ata N = 1024.
- **MPI_Comm_size** para obtener o numero de procesos, numproc.
- **MPI_Comm_rank** para obter o identificador de cada proceso, rank.
- Valores de timeval para as medicións de tempo.
- Asignar m filas da matriz a cada proceso. Con **ceil()** se temos un valor con decimais aproxima ao seguinte enteiro, é dicir, se temos 13.40 -> 14.00 e 13.00 -> 13.00 (cubrimos o caso no que o número de procesos non é multiplo das filas).
- Unha matriz **mat[m][N]** local para cada proceso, e un vector **res[m]** co resultado local de cada proceso.
Así teremos a matriz, o vector, e o resultado total orixinal de tamaño N e as matrices e os vectores locais que podemos repartir e reunir para cada proceso de tamaño m, o asignado a cada proceso.

1. O proceso 0 inicializa a matriz e o vector igual que antes.
2. **MPI_Bcast**: comunicación un a todos de N (1024) datos de tipo float dende o proceso 0 (root) ao resto de procesos de comm.
3. **MPI_Scatter**: distribución dos elementos da matriz inicializada (buffer) a cada proceso. Envíase m*N (filas x columnas) elementos tipo float. Crease unha matriz mat[m][N] para cada proceso onde reciben os elementos cos que traballa cada un (buffer recepcion), tamén de tipo float. Os datos son enviados polo proceso 0 a traves do comm.
4. Agora que cada proceso ten o vector e unha parte da matriz **faise a operacion**. Cada proceso ten un **res[m]** no que garda o resultado parcial localmente.
5. **MPI_Gather**: recoleción dos res[m] en **result[N]**. Recollemos do proceso 0 (root) cada res con m elementos tipo float concatenados a result[N].
6. Para os calculos dos tempos facemos como antes e medimos os microsegundos da computacion e da comunicación coa mesma fórmula e printeamos o de cada proceso. 
7. **MPI_Reduce**: función que permite calcular o máximo, a suma, etc. dos procesos dun comunicador. Neste caso con todos os microseconds facemos a suma e gardamos en **tcomm e tcomp**, para facer a media máis adiante.
8. Mostramos os resultados como facíamos anteriormente pero agora coa media dos tempos.