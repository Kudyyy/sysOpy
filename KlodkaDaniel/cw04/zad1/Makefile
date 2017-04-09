CFLAGS= -Wall -std=c99
CC=gcc

all: compile run

compile: 
	$(CC) $(CFLAGS) main.c -o main1
	chmod a+x sigtstp.sh
run:
	./main1

clean:
	rm -f main1
