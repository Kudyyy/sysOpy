#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAX_ARGS 5

void runScript(char*,char*,char*);
void runVar(char*);
void runProg(char*,char*,char*);
float getSec(struct timeval);

int main(int argc, char* argv[]) {

  if (argc != 4 ) {
    perror("Incorrect number of arguments.\n");
    return 1;
  }

  runScript(argv[1],argv[2],argv[3]);

  return 0;
}



void runScript(char* filePath,char* cpuLimit,char* memLimit) {


  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  FILE* file = fopen(filePath, "r+");
  if (!file) {
    perror("The requested file could not be opened");
    exit(-1);
  }

  while ((nread = getline(&line, &len, file)) != -1) {
    
    if(nread > 1){
      if(line[0] == '#') runVar(line+1);     // Var
      else runProg(line,cpuLimit,memLimit);                   // Prog
    }
  }

  free(line);
  fclose(file);
  exit(EXIT_SUCCESS);
}

void runVar(char* var){
  var = strtok (var," \t\r\n");
  char* val = strtok(NULL," \t\r\n");
  if (val != NULL){
    if (setenv(var,val,1)) 
      fprintf(stderr,"Could not set env var %s with val %s\n",var,val);
  }
  else{
    if(unsetenv(var))
      fprintf(stderr,"Could not unset env var %s\n",var);
  }
}

void runProg(char* prog,char* cpuLimit,char* memLimit){
  prog = strtok(prog," \t\n\r");
  char* arg = strtok(NULL," \t\n\r");
  int args_num;
  char* arguments[MAX_ARGS+1];
  memset(arguments,0,(MAX_ARGS+1)*sizeof(char*)); 
  arguments[0] = prog;
  for (args_num = 1; args_num < MAX_ARGS && arg != NULL; ++args_num){
    arguments[args_num] = arg;
    arg = strtok(NULL," \t\n\r");
  }

  pid_t pid = fork();
  if(pid == 0){
    rlim_t memL = (rlim_t)atoll(memLimit);
    rlim_t cpuL = (rlim_t)atoll(cpuLimit);
    struct rlimit rlC;
    struct rlimit rlM;
    
    getrlimit(RLIMIT_CPU, &rlC);
    if(cpuL <= rlC.rlim_max){
      rlC.rlim_cur = 1;
      rlC.rlim_max = cpuL;
    }
    else perror("Could not set CPU LIMIT you have to set lower limit");
    if(setrlimit(RLIMIT_CPU, &rlC) != 0) perror("Could not set CPU LIMIT");

    getrlimit(RLIMIT_AS, &rlM);
    if(memL <= rlM.rlim_max){
      rlM.rlim_cur = memL*1024*768; 
      rlM.rlim_max = memL*1024*1024;
    }
    else perror("Could not set MEM LIMIT you have to set lower limit");
    if(setrlimit(RLIMIT_AS, &rlM) != 0) perror("Could not set MEM LIMIT");
    
    if(execv(prog,arguments) == -1 && execvp(prog,arguments) == -1){
      fprintf(stderr,"Could not run program %s\n",prog);
      exit(1);
    }
  }
  else if(pid > 0){
    int status;
    struct rusage usage;
    
    wait3(&status,0,&usage);
    if(WIFEXITED(status)){
      printf("Child %d with program %s exited normally with code %d\n",pid,prog, WEXITSTATUS(status));
      printf("\tChild took %fs of sys time and %fs of user time\n"
        ,getSec(usage.ru_stime),getSec(usage.ru_utime));    
    }
    else{
      printf("Child %d with program %s did not exit normally\n",pid,prog);
      printf("\tChild took %fs of sys time and %fs of user time\n"
        ,getSec(usage.ru_stime),getSec(usage.ru_utime));
    }
  }
  else{
    fprintf(stderr,"Could not create process for program %s\n",prog);
    exit(1);
  }
}

float getSec(struct timeval t){
  return (float)(((float)t.tv_usec)/1000000 +t.tv_sec);
}










