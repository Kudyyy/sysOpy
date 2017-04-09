#define _XOPEN_SOURCE 500
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>

int sig_counter = 0;
int requests;
int* pending;
int locked = 1;

void sigint_handler(int sig){
  printf("Parent got SIGINT\n");
  fflush(stdout);
  for (int i = 0; i < sig_counter; ++i){
        printf("Killing %d\n",pending[i]);
        kill(pending[i], SIGKILL);
  }
  exit(0);
}

void sigusr1_handler(int sig,siginfo_t * lel, void * ptr){
	printf("\nGot SIGUSR1 signal from %d\n",lel->si_pid);
	if (sig_counter < requests){
		pending[sig_counter++] = lel->si_pid;
		printf("\nWaiting procesess \n");
		for (int i = 0; i < sig_counter && pending[i] != 0; ++i){
			printf("%d ",pending[i]);
		}
		printf("\n");
		fflush(stdout);
	}
	else{
    if(locked){
      int last = sig_counter;
      for (int i = 0; i < last; ++i){
        //usleep(1000);
        kill(pending[i], SIGUSR2);
      }
      locked = 0;
    }
  		kill(lel->si_pid, SIGUSR2);
      pending[sig_counter++] = lel->si_pid;
	}
}

void sigrt_handler(int sig,siginfo_t * lel, void * ptr){
  int status;
  if ( waitpid(lel->si_pid, &status, 0) == -1 ) 
            perror("waitpid failed in handler");
        
      if (WIFEXITED(status))
        printf("Child %d exited with code %d\n",lel->si_pid, WEXITSTATUS(status));
        fflush(stdout);  

}


int main(int argc, char* argv[]){


	int no_kids;
	 
	if (argc != 3 ) {
    	perror("Incorrect number of arguments.\n");
    	return 1;
  	}

  	if (sscanf(argv[1], "%d", &no_kids) != 1) {
    	printf("Number of kids must be int.\n");
    	return 1;
  	}

  	if (sscanf(argv[2], "%d", &requests) != 1) {
    	printf("Number of requests must be int.\n");
    	return 1;
  	}

  	if (requests > no_kids){
  		printf("Number of kids must be bigger or equal number of requests.\n");
    	return 1;
  	}

  	int* pids_of_kids = (int*)calloc(no_kids,sizeof(int));
  	pending = (int*)calloc(no_kids,sizeof(int));

  	struct sigaction act;
	act.sa_sigaction = sigusr1_handler;
	act.sa_flags = SA_SIGINFO; 
  sigemptyset(&act.sa_mask); 

  struct sigaction rt;
  rt.sa_sigaction = sigrt_handler;
  rt.sa_flags = SA_SIGINFO; 
  sigemptyset(&rt.sa_mask); 

  	if(sigaction(SIGUSR1,&act,NULL) < 0) printf("\nCan't catch SIGUSR1\n");

  
    for(int sig=SIGRTMIN;sig<=SIGRTMAX;sig++){
        if(sigaction(sig,&rt,NULL) < 0) printf("\nCan't catch SIGRT\n");
    }


  	for (int i = 0; i < no_kids; ++i){
  		int x = fork();
      srand(x);
  		if (x > 0) pids_of_kids[i] = x;
  		else if (x < 0) perror("\nCould not create kidoo\n");
  		else{

  			//printf("\nStworzylem %d\n", getpid());
          struct timespec start, finish;
  				sigset_t mask;
  				sigemptyset(&mask);
  				sigaddset(&mask, SIGUSR2);
  				sigprocmask(SIG_BLOCK, &mask, NULL);
  				fflush(stdout);
  				sleep((rand() + i)%11);
  				kill(getppid(),SIGUSR1);
          clock_gettime(CLOCK_MONOTONIC, &start);
  				if (sigwaitinfo(&mask, NULL) == -1) {
            		perror("sigwaitinfo() failed");
            		continue;
        		}
          usleep(10000*(rand()%10));
          int SIGRANDOM = SIGRTMIN + rand()%(SIGRTMAX + 1 - SIGRTMIN);
          clock_gettime(CLOCK_MONOTONIC, &finish);
          double elapsed = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
          //elapsed *= 1000;
          //printf("\nsending %d\n",(int)elapsed);
  				kill(getppid(),SIGRANDOM);
  				fflush(stdout);
  				exit((int)elapsed); // s
  			
  		}
  	}
    if(signal(SIGINT,sigint_handler) == SIG_ERR) printf("\nCan't catch SIGINT\n");


  	while(1){
  		usleep(1000);
  	}

  	free(pids_of_kids);
  	free(pending);
    wait(NULL);
  	printf("\nKONIEC MAIN\n");

  	return 0;
  
}