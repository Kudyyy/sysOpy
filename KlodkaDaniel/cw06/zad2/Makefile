CFLAGS= -Wall -std=c99 -lrt
CC=gcc

all: compile

compile: 
	$(CC) $(CFLAGS) server.c -o server
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f server client
