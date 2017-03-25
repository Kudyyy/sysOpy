#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 5

void runScript(char*);
void runVar(char*);
void runProg(char*);

int main(int argc, char* argv[]) {

  if (argc != 2 ) {
    perror("Incorrect number of arguments.\n");
    return 1;
  }

  runScript(argv[1]);

  return 0;
}



void runScript(char* filePath) {


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
      else runProg(line);                   // Prog
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

void runProg(char* prog){
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
  int status;
  pid_t pid = fork();
  if(pid == 0){
    if(execv(prog,arguments) == -1 && execvp(prog,arguments) == -1)
      fprintf(stderr,"Could not run program %s\n",prog);
      exit(1);
  }
  else if(pid > 0){
    wait(&status);
    if(WIFEXITED(status))
            printf("Child %d with program %s exited normally with code %d\n",pid,prog, WEXITSTATUS(status));
        else
            printf("Child %d with program %s did not exit normally\n",pid,prog);
  }
  else{
    fprintf(stderr,"Could not create process for program %s\n",prog);
    exit(1);
  }
}










