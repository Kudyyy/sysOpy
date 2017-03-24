#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>


void getFilesFromDir(char*);
char* getPermissions(mode_t); 

int MAX_FILE_SIZE = 0;

int main(int argc, char* argv[]) {

  int maxFileSize;

  if (argc != 3) {
    printf("Incorrect number of arguments.\n");
    return 1;
  }

  if (sscanf(argv[2], "%d", &maxFileSize) != 1) {
    printf("Size of file must be int.");
    return 1;
  }

  MAX_FILE_SIZE = maxFileSize;
  getFilesFromDir(argv[1]);

  return 0;
}

static int showInformations(const char* filePath, const struct stat* info, int tflag, struct FTW* ftwbuf) {
  if (S_ISREG(info -> st_mode) && info -> st_size <= MAX_FILE_SIZE) {
    char* permissions = getPermissions(info -> st_mode);
    printf("Path: %s\n Size in bytes: %lld\n Permissions: %s\n Last modified: %s\n", filePath, info->st_size, permissions, asctime(localtime(&info->st_mtime)));
    free(permissions);
  }
  return 0;
}

void getFilesFromDir(char* filePath) {
  if(nftw(filePath, showInformations, 20, FTW_PHYS) == -1) { 
    printf("Something went wrong.\n");
    exit(-1);
  }
}

char* getPermissions(mode_t permissions) {
  char* result = malloc(11);
  strcpy(result, (S_ISDIR(permissions)) ? "d" : "-");
  strcat(result, (permissions & S_IRUSR) ? "r" : "-");
  strcat(result, (permissions & S_IWUSR) ? "w" : "-");
  strcat(result, (permissions & S_IXUSR) ? "x" : "-");
  strcat(result, (permissions & S_IRGRP) ? "r" : "-");
  strcat(result, (permissions & S_IWGRP) ? "w" : "-");
  strcat(result, (permissions & S_IXGRP) ? "x" : "-");
  strcat(result, (permissions & S_IROTH) ? "r" : "-");
  strcat(result, (permissions & S_IWOTH) ? "w" : "-");
  strcat(result, (permissions & S_IXOTH) ? "x" : "-");
  return result;
}


