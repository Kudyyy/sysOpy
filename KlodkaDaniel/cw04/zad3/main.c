#define _XOPEN_SOURCE 500
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


int kid_pid;

void sigint_handler(int sig){
  printf("Parent got SIGINT\n");
  fflush(stdout);
  kill(kid_pid,SIGKILL);
  exit(0);
}

int par = 0;
void sigusr1_parent(int sig){
  printf("Parent got %d signals SIGUSR1\n", ++par);
  fflush(stdout);
}
void sigusr1_parentR(int sig){
  printf("Parent got %d signals SIGRTMIN\n", ++par);
  fflush(stdout);
}

int kidoo = 0;
void sigusr1_kidoo(int sig){
	printf("Child got %d signals SIGUSR1\n", ++kidoo);
  fflush(stdout);
  kill(getppid(),SIGUSR1);
  printf("Child sent %dth signal to parent\n", kidoo);
  fflush(stdout);
}

void sigusr2_end(int sig){
  printf("Child got SIGUSR2, bye\n");
  fflush(stdout);
  exit(0);
}

void sigusr1_kidooR(int sig){
  printf("Child got %d signals SIGRTMIN\n", ++kidoo);
  fflush(stdout);
  kill(getppid(),SIGRTMIN);
  printf("Child sent %dth signal to parent\n", kidoo);
  fflush(stdout);
}

void sigusr2_endR(int sig){
  printf("Child got SIGRTMAX, bye\n");
  fflush(stdout);
  exit(0);
}



int main(int argc, char* argv[]){

  int type;
  int no_signals;
	 
	if (argc != 3 ) {
    	perror("Incorrect number of arguments.\n");
    	return 1;
  	}

  	if (sscanf(argv[1], "%d", &no_signals) != 1) {
    	printf("Number of signals must be int.\n");
    	return 1;
  	}

  	if (sscanf(argv[2], "%d", &type) != 1) {
    	printf("Type must be int.\n");
    	return 1;
  	}

  	if (type > 3 || type < 1){
  		printf("Type must be 1, 2 or 3.\n");
    	return 1;
  	}

    kid_pid = fork();
    if (kid_pid > 0){
          if(signal(SIGINT,sigint_handler) == SIG_ERR) printf("\nCan't catch SIGINT\n");
          switch(type){
            case 1:
            case 2:
              if(signal(SIGUSR1,sigusr1_parent) == SIG_ERR) printf("\nCan't catch SIGUSR1\n");
              break;
            case 3:
              if(signal(SIGRTMIN,sigusr1_parentR) == SIG_ERR) printf("\nCan't catch SIGRTMIN\n");
              break;
          }
          sleep(2);
          union sigval value;
          switch(type){
              case 1:
                for (int i = 0; i < no_signals; ++i){
                  kill(kid_pid,SIGUSR1);
                  printf("Parent is sending %dth SIGUSR1\n", i + 1);
                  fflush(stdout);
                  //usleep(10000);
                }
                kill(kid_pid,SIGUSR2);
                break;
              case 2:
                value.sival_int = 0;
                value.sival_ptr = NULL;
                for (int i = 0; i < no_signals; ++i){
                  sigqueue(kid_pid,SIGUSR1,value);
                  printf("Parent is sending %dth SIGUSR1\n", i + 1);
                  fflush(stdout);
                  //usleep(10000);
                }
                sigqueue(kid_pid,SIGUSR2,value);
                break;
              case 3:
                for (int i = 0; i < no_signals; ++i){
                  kill(kid_pid,SIGRTMIN);
                  printf("Parent is sending %dth SIGRTMIN\n", i + 1);
                  fflush(stdout);
                  //usleep(10000);
                }
                kill(kid_pid,SIGRTMAX);
                break;
            }
        
    }
    else if (kid_pid < 0) perror("\nCould not create kidoo\n");
    else{
        switch(type){
          case 1:
          case 2:
            if(signal(SIGUSR1, sigusr1_kidoo) == SIG_ERR) printf("\nCan't catch SIGUSR1\n");
            if(signal(SIGUSR2, sigusr2_end) == SIG_ERR) printf("\nCan't catch SIGIUSR2\n");
            break;
          case 3:
            if(signal(SIGRTMIN, sigusr1_kidooR) == SIG_ERR) printf("\nCan't catch SIGRTMIN\n");
            if(signal(SIGRTMAX, sigusr2_endR) == SIG_ERR) printf("\nCan't catch SIGRTMAX\n");
            break;
        }
        sleep(2);
        while (1){
          usleep(100000);
          //printf("%d\n", kidoo);
        }  
    }
    printf("KONIEC\n");
  	return 0;
  
}