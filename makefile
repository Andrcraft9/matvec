CC = mpiicc
CFLAGS = -O3 -qopenmp

all:
	$(CC) $(CFLAGS) -o mvm mvm.c -lm

