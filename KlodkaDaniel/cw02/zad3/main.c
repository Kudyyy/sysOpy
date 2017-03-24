#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void lock(int, int, int, int);
void readByte(int, int);
void writeByte(int, int);
void list(int);


int main(int argc, char const *argv[]) {

  int holder;

  if (argc != 2 ) {
    printf("Incorrect number of arguments.\n");
    return 1;
  }

  if ((holder = open(argv[1], O_RDWR)) == -1) {
    printf(("Could not open the file\n") );
    exit(1);
  }

  char* input = (char*)malloc(100);
  int byte;

  while (strcmp(input, "q") != 0) {

    scanf("%s",input);

   
    if (strcmp(input, "rlnb") == 0) {
      printf("Write a byte in file that you want to use: \n");
      scanf("%d",&byte);
      lock(holder, byte, F_RDLCK, F_SETLK);
    } 
    else if (strcmp(input, "rlb") == 0) {
      printf("Write a byte in file that you want to use: \n");
      scanf("%d",&byte);
      lock(holder, byte, F_RDLCK, F_SETLKW);
    } 
    else if (strcmp(input, "wlnb") == 0) {
      printf("Write a byte in file that you want to use: \n");
      scanf("%d",&byte);
      lock(holder, byte, F_WRLCK, F_SETLK);
    } 
    else if (strcmp(input, "wlb") == 0) {
      printf("Write a byte in file that you want to use: \n");
      scanf("%d",&byte);
      lock(holder, byte, F_WRLCK, F_SETLKW);
    } 
    else if (strcmp(input, "fl") == 0) {
      printf("Write a byte in file that you want to use: \n");
      scanf("%d",&byte);
      lock(holder, byte, F_UNLCK, F_SETLK);
    } 
    else if (strcmp(input, "r") == 0) {
      printf("Write a byte in file that you want to use: \n");
      scanf("%d",&byte);
      readByte(holder, byte);
    }
    else if (strcmp(input, "w") == 0) {
      printf("Write a byte in file that you want to use: \n");
      scanf("%d",&byte);
      writeByte(holder, byte);
    }
    else if (strcmp(input, "l") == 0) {
      list(holder);
    } 
    else {
       printf("Usage: [rlnb | rlb | wlnb | wlb | fl | r | w | l | q]\n");
    }   
    
  }

  free(input);
  close(holder);
  return 0;
}

void lock(int holder, int byte, int lockType, int command) {
  struct flock *lock = (struct flock*)malloc(sizeof(struct flock));

  lock -> l_start = byte;
  lock -> l_whence = SEEK_SET;
  lock -> l_len = 1;
  lock -> l_type = lockType;

  fcntl(holder, command, lock) == -1 ? printf("fail\n") : printf("success\n"); 
  free(lock);
}

void list(int holder) {
  struct flock *lock = (struct flock*)malloc(sizeof(struct flock));
  lock -> l_whence = SEEK_SET;
  lock -> l_len = 1;
  lock -> l_type = F_WRLCK;

  int offset;
  if ((offset = lseek(holder, 0, SEEK_END)) == -1) {
    perror("Could not read the offset of file while performing lseek");
    return;
  }
  for (int byte = 0; byte <= offset; byte++) {
    lock -> l_start = byte;
    lock -> l_type = F_WRLCK;

    if (fcntl(holder, F_GETLK, lock) == -1) printf("Could not access the byte %d\n", byte);
    else if (lock -> l_type == F_RDLCK) printf("PID %d read locked the byte number %d\n",lock -> l_pid, byte);
    else if (lock -> l_type == F_WRLCK) printf("PID %d write locked the byte number %d\n",lock -> l_pid, byte);
  }
  free(lock);
}

void readByte(int holder, int byte) {
  char buffer;
  if (lseek(holder, byte, SEEK_SET) == -1) printf("Could not write the offset");
  else if (read(holder, &buffer, 1) != 1) printf("Could not read the byte");
  else printf("%c\n", buffer);
}

void writeByte(int holder, int byte) {
  char buffer;
  printf("Type a char that you want to write\n");
  scanf("%s", &buffer);
  if (lseek(holder, byte, SEEK_SET) == -1) printf("Could not write the offset");
  else if (write(holder, &buffer, 1) != 1) printf("Could not write to file");
}

