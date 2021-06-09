CC = gcc
CFLAGS = -O3

all:
	$(CC) $(CFLAGS) -o mvm mvm.c -lm

