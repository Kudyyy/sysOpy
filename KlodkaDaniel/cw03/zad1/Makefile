CFLAGS= -Wall -std=c99
CC=gcc

all: compile run

compile: 
	$(CC) $(CFLAGS) main.c -o main
	$(CC) $(CFLAGS) test.c -o test
run:
	./main script

clean:
	rm -f test main

