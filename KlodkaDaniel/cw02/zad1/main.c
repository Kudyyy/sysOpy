#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/times.h> 

#define CLK sysconf(_SC_CLK_TCK)

void checkTimes(struct tms*, clock_t*, struct tms*, clock_t*);
void fileGenerator(char*, int, int);
void libSort(char*, int, int);
void libShuffle(char*, int, int);
void sysSort(char*, int, int);
void sysShuffle(char*, int, int);

int main(int argc, char* argv[]) {

  int recSize;
  int recNum;

  if (argc != 6 ) {
    printf("Incorrect number of arguments.\n");
    return 1;
  }

  if (sscanf(argv[4], "%d", &recSize) != 1) {
    printf("Size of the record must be int.\n");
    return -1;
  }

  if (sscanf(argv[5], "%d", &recNum) != 1) {
    printf("Number of records must be int.\n");
    return -1;
  }

  struct tms prevTimes;
  clock_t prevReal;
  struct tms firstTimes;
  clock_t firstReal;
  prevTimes.tms_stime = -1;
  srand(time(NULL));

  checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);
  if (strcmp(argv[1], "sys") == 0) {
    if (strcmp(argv[2], "generate") == 0) fileGenerator(argv[3], recSize, recNum);
    else if (strcmp(argv[2], "sort") == 0) sysSort(argv[3], recSize, recNum);
    else if (strcmp(argv[2], "shuffle") == 0) sysShuffle(argv[3], recSize, recNum);
    else {
      printf("Bad format of arguments.\n Should be ./Program [function_type] [operation_type] [path_to_file] [size_of_record] [number_of_records]\n");
      return -1;
    }
  }
  else if (strcmp(argv[1], "lib") == 0) {
    if (strcmp(argv[2], "generate") == 0) fileGenerator(argv[3], recSize, recNum);
    else if (strcmp(argv[2], "sort") == 0) libSort(argv[3], recSize, recNum);
    else if (strcmp(argv[2], "shuffle") == 0) libShuffle(argv[3], recSize, recNum);
    else {
      printf("Bad format of arguments.\n Should be ./Program [function_type] [operation_type] [path_to_file] [size_of_record] [number_of_records]\n");
      return -1;
    }
  } else {
    printf("Bad format of arguments.\n Should be ./Program [function_type] [operation_type] [path_to_file] [size_of_record] [number_of_records]\n");
    return -1;
  }
  checkTimes(&prevTimes, &prevReal, &firstTimes, &firstReal);

  return 0;
}

void sysShuffle(char* filePath, int recSize, int recNum) {

  int fd = open(filePath, O_RDWR);
  if (fd == -1) {
    printf("Could not open the file %s",filePath);
    exit(-1);
  }

  char* buff1 = (char*)malloc(recSize);
  char* buff2 = (char*)malloc(recSize);
  int j;

  for (int i = 0; i < recNum; i++) {
    j = rand() % recNum;

    lseek(fd, i * recSize, SEEK_SET);
    read(fd, buff1, recSize);
    lseek(fd, j * recSize, SEEK_SET);
    read(fd, buff2, recSize);

    lseek(fd, i * recSize, SEEK_SET);
    write(fd, buff1, recSize);
    lseek(fd, j * recSize, SEEK_SET);
    write(fd, buff2, recSize);
  }

  free(buff1);
  free(buff2);
  close(fd);
}

void sysSort(char* filePath, int recSize, int recNum) {

  int fd = open(filePath, O_RDWR);
  if (fd == -1) {
    printf("Could not open the file %s",filePath);
    exit(-1);
  }

  char* buff1 = (char*)malloc(recSize);
  char* buff2 = (char*)malloc(recSize);

  for (int i = 0; i < recNum; ++i) {
    for (int j = i + 1; j < recNum; ++j) {

      lseek(fd, i * recSize, SEEK_SET);
      read(fd, buff1, recSize);
      lseek(fd, j * recSize, SEEK_SET);
      read(fd, buff2, recSize);

      if (((unsigned char) buff1[0] - (unsigned char) buff2[0]) > 0) {
        lseek(fd, i * recSize, SEEK_SET);
        write(fd, buff1, recSize);
        lseek(fd, j * recSize, SEEK_SET);
        write(fd, buff2, recSize);
      }
    }
  }

  free(buff1);
  free(buff2);
  close(fd);
}


void libShuffle(char* filePath, int recSize, int recNum) {

  FILE* file = fopen(filePath, "r+");
  if (!file) {
    perror("The requested file could not be opened");
    exit(-1);
  }

  char* buff1 = (char*)malloc(recSize);
  char* buff2 = (char*)malloc(recSize);
  int j;

  for (int i = 0; i < recNum; i++) {
    j = rand() % recNum ;

    fseek(file, i * recSize, SEEK_SET);
    fread(buff1, recSize, 1, file);
    fseek(file, j * recSize, SEEK_SET);
    fread(buff2, recSize, 1, file);

    fseek(file, i * recSize, SEEK_SET);
    fwrite(buff2, recSize, 1, file);
    fseek(file, j * recSize, SEEK_SET);
    fwrite(buff1, recSize, 1, file);
  }

  free(buff1);
  free(buff2);
  fclose(file);
}

void libSort(char* filePath, int recSize, int recNum) {

  FILE* file = fopen(filePath, "r+");
  if (!file) {
    printf("Could not open the file %s",filePath);
    exit(-1);
  }

  char* buff1 = (char*)malloc(recSize);
  char* buff2 = (char*)malloc(recSize);

  for (int i = 0; i < recNum; ++i) {
    for (int j = i + 1; j < recNum; ++j) {

      fseek(file, i * recSize, SEEK_SET);
      fread(buff1, recSize, 1, file);
      fseek(file, j * recSize, SEEK_SET);
      fread(buff2, recSize, 1, file);

      if (((unsigned char) buff1[0] - (unsigned char) buff2[0]) > 0) {
        fseek(file, i * recSize, SEEK_SET);
        fwrite(buff2, recSize, 1, file);
        fseek(file, j * recSize, SEEK_SET);
        fwrite(buff1, recSize, 1, file);
      }
    }
  }

  free(buff1);
  free(buff2);
  fclose(file);
}

void fileGenerator(char* filePath, int recSize, int recNum) {

  FILE* nfd = fopen(filePath, "w");
  FILE* rfd = fopen("/dev/random", "r+");
  if (!rfd || !nfd) {
    printf("Could not open the file");
    exit(-1);
  }

  char* buffor = (char*)malloc(recSize);
  size_t recCount = 0;

  while(fread(buffor, recSize, 1, rfd) > 0) {
    fwrite(buffor, recSize, 1, nfd);
    if (++recCount == recNum)
      break;
  }

  free(buffor);
  fclose(rfd);
  fclose(nfd);
}


void checkTimes(struct tms *prevTimes, clock_t *prevReal,
        struct tms *firstTimes, clock_t *firstReal) {

    struct tms now;
    times(&now);
    clock_t nowReal = clock();
    if (prevTimes->tms_stime == -1) {
        *firstTimes = now;
        *firstReal = nowReal;
        printf("\n\tTime:\t\t\tR %.6f\tS %.6f\tU %.6f\n\n",
            ((double) nowReal) / CLOCKS_PER_SEC,
            ((double) now.tms_stime) / CLK,
            ((double) now.tms_utime) / CLK);
    } else {
        printf("\n\tFrom the previous:\tR %.6f\tS %.6f\tU %.6f\n\n",
                ((double) (nowReal - *(prevReal))) / CLOCKS_PER_SEC,
                ((double) (now.tms_stime - prevTimes->tms_stime)) / CLK,
                ((double) (now.tms_utime - prevTimes->tms_utime)) / CLK);
    }
    *prevReal = nowReal;
    *prevTimes = now;

}

