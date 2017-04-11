#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const int R_MIN = -2;
const int R_MAX = 1;
const int I_MIN = -1;
const int I_MAX = 1;
const int KIDS_NO = 10;

int scale(char*,int, int, int);
void read_pipe(int**, char*, int);
void draw_gnuplot(int);
void save_array(int**, int);



int main(int argc, char *argv[]){

    int R;
    char path[100];

    if(argc != 3){
        printf("%s\n", "Invalid number of arguments");
        exit(1);
    }
    

    if(sscanf(argv[1], "%s", path) != 1){
        printf("Path must be string.\n");
        exit(1);
    }

    if (sscanf(argv[2], "%d", &R) != 1) {
        printf("Size must be int.\n");
        exit(1);
    } 

    if(mkfifo(path,0666) == -1){
        printf("Could not create pipe\n");      
        exit(1);
    }

    int **results = (int **) calloc(R, sizeof(int*));
    for(int i = 0; i < R; i++) results[i] = (int *) calloc(R, sizeof(int));

    for (int i = 0; i < KIDS_NO; ++i){
        char* args[5] = {"slave",path,"1000000","100",0};
        int x = fork();
        if (x == 0){
            if (execv(args[0],args) == -1 ) printf("cos nie tak\n"); 
            exit(0);
        }
    }
    

    read_pipe(results, path, R);
    save_array(results,R);
    draw_gnuplot(R);
    
    unlink(path);

    for(int i = 0; i < R; i++)  free(results[i]);
    free(results);
    return 0;   
}

int scale(char* value,int min, int max, int arr_size){
    double val;
    if (sscanf(value, "%lf", &val) != 1) 
        printf("Could not get value\n");
    return (int)(((double)(val - min) / (double)(max - min)) * (double)(arr_size));
}


void read_pipe(int **results, char *path, int R){

    int fd = open(path, O_RDONLY);
    if(fd < 0){
        printf("Could not open pipe\n");      
        exit(1);
    }
    sleep(10);

    char line[100];
    while(read(fd, line, 100)>0){
        char* real = strtok(line, " \n\t\r");
        char* ima = strtok(NULL, " \n\t\r");
        char* iters = strtok(NULL, " \n\t\r");
        int real_int = scale(real,R_MIN,R_MAX,R);
        int ima_int = scale(ima,I_MIN,I_MAX,R);
        int iters_int;
        if (sscanf(iters, "%d", &iters_int) != 1) 
            printf("Could not get value\n");
        results[real_int][ima_int] = iters_int;   
    }
    close(fd);
}

void save_array(int **results, int R){
    FILE* data = fopen("data", "w");
    if(data == NULL){
        printf("Could not open file\n");
        exit(1);
    }
    for(int i = 0; i < R; i++){
        for(int j = 0; j < R; j++){
            fprintf(data, "%d %d %d\n", i, j, results[i][j]);
        }
    }
    fclose(data);
}

void draw_gnuplot(int R){
    FILE *gplot = popen("gnuplot", "w");
    if(gplot == NULL){
        printf("GNUPLOT ERROR\n");
        exit(1);
    }
    fprintf(gplot, "set view map\n");
    fprintf(gplot, "set xrange[0:%d]\n", R);
    //fprintf(gplot, "set terminal epslatex size 20,17 color colortext\n");
    //fprintf(gplot, "set output 'introduction.tex'\n");
    //fprintf(gplot, "set yrange[0:%d]\n", R);
    fprintf(gplot, "plot 'data' with image\n");
    fflush(gplot);
    getchar();
    pclose(gplot);  
}