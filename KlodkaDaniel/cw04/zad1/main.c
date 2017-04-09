#define _XOPEN_SOURCE 500
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int flag = 1;

void sigtstp_handler(int sig){
	printf("\nChanging direction!\n");
	flag = flag ? 0 : 1;
}

void sigint_handler(int sig){
	printf("\nReceived SIGINT\n");
	exit(0);
}


int main(int argc, char* argv[]){

	struct sigaction act; 
	act.sa_handler = sigtstp_handler; 
	sigemptyset(&act.sa_mask); 
	act.sa_flags = 0;
 	
 	if(signal(SIGINT, sigint_handler) == SIG_ERR) printf("\nCan't catch SIGINT\n");
	if(sigaction(SIGTSTP,&act,NULL) < 0) printf("\nCan't catch SIGINT\n");

	char a_char;
	while(1){
	 	if (flag){
	 		a_char = 'A';
	 		while((int)a_char < 91){
	 			printf("%c ", a_char);
	 			fflush(stdout);
	 			usleep(100 * 1000);
	 			a_char++;
	 		}
	 		printf("\n");
	 	}
	 	else{
	 		a_char = 'Z';
	 		while((int)a_char > 64){
	 			printf("%c ", a_char);
	 			fflush(stdout);
	 			usleep(100 * 1000);
	 			a_char--;
	 		}
	 		printf("\n");
	 	}
	}
	
	 
}