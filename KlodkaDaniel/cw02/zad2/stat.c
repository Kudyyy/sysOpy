#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

char* getPermissions(mode_t);
char* concatPath(char*, char*, char*);
void getFilesFromDir(char*, int);

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

  getFilesFromDir(argv[1], maxFileSize);

  return 0;
}


void getFilesFromDir(char* filePath, int maxFileSize) {
  struct dirent* pDirent;
  DIR* pDir;
  struct stat info;

  if ((pDir = opendir(filePath)) == NULL) {
    printf("Cannot open directory '%s'\n", filePath);
    return;
  }

  while ((pDirent = readdir(pDir)) != NULL) {
    if (strcmp(pDirent -> d_name, ".") != 0 && strcmp(pDirent -> d_name, "..") != 0) {
      char* newPath = concatPath(filePath,"/",pDirent->d_name);

      if (lstat(newPath, &info) == -1) {
        printf("Could not get file informations\n");
        free(newPath);
        continue;
      }
      if (S_ISDIR(info.st_mode) && !S_ISLNK(info.st_mode)) getFilesFromDir(newPath, maxFileSize);
      else if (S_ISREG(info.st_mode) && info.st_size <= maxFileSize) {
        char* permissions = getPermissions(info.st_mode);
        printf("Path: %s\n Size in bytes: %lld\n Permissions: %s\n Last modified: %s\n", newPath, info.st_size, permissions, asctime(localtime(&info.st_mtime)));
        free(permissions);
      }
      free(newPath);
    }
  }
  free(pDir);
  free(pDirent);
}

char* concatPath(char* parent, char* slash, char* dirName) {
  char* result = (char*)malloc(strlen(parent) + strlen(slash) + strlen(dirName) + 1);
  strcpy(result, parent);
  strcat(result, slash);
  strcat(result, dirName);
  return result;
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
