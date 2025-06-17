# paralelismo-p1

## Librería
```bash
sudo apt update
sudo apt install libopenmpi-dev openmpi-bin
```

## Compilar
```bash
mpicc main.c -o main
```

## Executar
```bash
mpirun -np 4 ./main # Para executar 4 procesos
```
