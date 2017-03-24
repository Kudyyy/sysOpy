CC=gcc
CFLAGS=-Wall -std=c99

all: stat nftw

stat: stat.c
	$(CC) $(CFLAGS) $^ -o $@.out

nftw: nftw.c
	$(CC) $(CFLAGS) $^ -o $@.out
	
clear:
	rm -f *.out
