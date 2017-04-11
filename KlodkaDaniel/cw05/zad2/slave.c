#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <complex.h>
#include <math.h>

int get_iter(double, double, int);

const int R_MIN = -2;
const int R_MAX = 1;
const int I_MIN = -1;
const int I_MAX = 1;

int main(int argc, char *argv[]){

    if(argc != 4){
        printf("%s\n", "Invalid number of arguments");
        exit(1);
    }
    char path[100];
    if(sscanf(argv[1], "%s", path) != 1){
        printf("Path must be string.\n");
        exit(1);
    }

    int points_no;
    if (sscanf(argv[2], "%d", &points_no) != 1) {
        printf("Points number must be int.\n");
        exit(1);
    }
    int iter_max; 
    if (sscanf(argv[3], "%d", &iter_max) != 1) {
        printf("Iters max number must be int.\n");
        exit(1);
    }
    srand(time(NULL)); 
    char buffer[100];
    int fd = open(path, O_WRONLY);
    if (fd < 0){
        printf("Could not open pipe\n");
        exit(1);
    }

    for(int i = 0; i < points_no; i++){
        double re = ((double) rand() / (double) RAND_MAX) * (double)(R_MAX - R_MIN) + R_MIN;
        double im = ((double) rand() / (double) RAND_MAX) * (double)(I_MAX - I_MIN) + I_MIN;
        int iter = get_iter(re,im,iter_max);
        sprintf(buffer, "%lf %lf %d\n", re, im, iter);
        write(fd, buffer, 100);               
    }
    close(fd);
    
    return 0;
    
}


int get_iter(double re, double im, int limit){
    double complex c = re + im * I;
    double complex zn = 0.0;
    int i;
    for (i = 0; i < limit && cabs(zn) < 2.0; i++){
        zn = cpow(zn, 2.0) + c;
    }
    return i;
}
