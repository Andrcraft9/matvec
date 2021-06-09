CC = mpicc -g
CFLAGS = -O3 -fopenmp

all:
	$(CC) $(CFLAGS) -o mvm mvm.c -lm

