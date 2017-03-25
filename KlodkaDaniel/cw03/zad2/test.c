#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void intHandler(int noob) {
    printf("lol nope\n");
}

int main(int argc, char* argv[]){
	 
	signal(SIGXCPU, intHandler);
	while(1); // HE HE HE 
}