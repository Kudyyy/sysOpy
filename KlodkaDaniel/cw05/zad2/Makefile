CFLAGS= -Wall -std=c99
CC=gcc

all: compile run

compile: 
	$(CC) $(CFLAGS) master.c -o master
	$(CC) $(CFLAGS) slave.c -lm -o slave
run:
	./master potok 500

clean:
	rm -f master slave data
