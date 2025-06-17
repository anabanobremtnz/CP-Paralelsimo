all:
	mpicc matvec_oc.c -Wall -g -I/usr/lib/x86_64-linux-gnu/openmpi/include/ -o oc -lm
	mpirun -np 3 ./oc

clean:
	rm -vf *.o oc
