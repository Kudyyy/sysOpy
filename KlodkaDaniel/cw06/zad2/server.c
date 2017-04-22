#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <time.h>
#include <mqueue.h>
#include <sys/fcntl.h>
#include "settings.h"


int get_server_queue_id(void);
void execute_msg(char *rec);
void log_in(char* received);
void log_out(char* received);
void echo_task(char* received);
void upper_task(char* received);
void time_task(char* received);
void send_msg(int queue_id, char* received);
int get_client_queue_id(pid_t c_pid);

int clients[MAX_CLIENTS][2];
int clients_count = 0; 
int client_id = 0;
int is_running;

int main(int argc, char *argv[]){
	mqd_t mq;
    struct mq_attr attr;
	char buffer[MAX_MSG_LEN];
	attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_LEN;
    attr.mq_curmsgs = 0;
	memset(buffer, 0, MAX_MSG_LEN);
 
	mq = mq_open(SERVER_QUEUE, O_CREAT | O_RDONLY, 0644, &attr);
	if((mqd_t)-1 == mq)
		perror("Mq_open error");
	
	is_running = 0;
	printf("Server pid: %d\n", getpid());
	ssize_t bytes_read;
	while(is_running == 0){
		bytes_read = mq_receive(mq, buffer, MAX_MSG_LEN, NULL);
	  	if(bytes_read < 0) 
           perror("Mq_receive error");
        execute_msg(buffer);
	}
	mq_close(mq);
	mq_unlink(SERVER_QUEUE);

    return 0;
}

void execute_msg(char *buff){
	char* msg_type = strtok (buff,"|");
	if(msg_type == NULL) return;
	int rec = atoi(msg_type);
	switch(rec){
		case LOGIN:
			log_in(buff);
			break;
		case LOGOUT:
			log_out(buff);
			break;
		case ECHO:
			echo_task(buff);
			break;
		case UPPER:
			upper_task(buff);
			break;
		case TIME:
			time_task(buff);
			break;
		case END:
			is_running = 1;
			break;
		default:
			break;
	}
}


void log_in(char * received){
	char* msg_pid = strtok (NULL,"|");
	int rec_pid = atoi(msg_pid);
	printf("%d is connecting\n", rec_pid);
	char response[MAX_MSG_LEN];
	memset(response, 0, MAX_MSG_LEN);
	//char* msg_cont = strtok (NULL,"|");
	int queue_id = rec_pid;//atoi(msg_cont);
	if(clients_count == MAX_CLIENTS){
		sprintf(response,"%d|",DUMP);
		printf("%d connection refused, server is full\n", rec_pid);
		int pid = getpid();
		sprintf(response+strlen(response),"%d|",pid);

	}
	else{
		clients[clients_count][0] = rec_pid;
		clients[clients_count][1] = queue_id;
		clients_count++;
		int pid = getpid();
		sprintf(response, "%d|%d|%d",EST,pid,client_id++);
		printf("%d connected\n", rec_pid);
	}
	send_msg(queue_id, response);
}

void log_out(char * received){
	char* msg_pid = strtok (NULL,"|");
	int pid = atoi(msg_pid);
	int index;
	for(int i = 0; i < MAX_CLIENTS; i++){
		if(pid == clients[i][0]){
			printf("%d logged out\n", pid);
			index = i;
		}
	}
	for(int i = index ; i < MAX_CLIENTS - 1 ; i++){
		clients[i][0] = clients[i+1][0];
		clients[i][1] = clients[i+1][1];
	}
	clients_count--;
}

void echo_task(char * received){
	char* msg_pid = strtok (NULL,"|");
	char* msg_echo = strtok (NULL,"|");
	int pid = atoi(msg_pid);
	int queue_id = get_client_queue_id(pid);
	pid = getpid();
	char response[MAX_MSG_LEN];
	sprintf(response, "%d|%d|%s",ECHO,pid,msg_echo);
	send_msg(queue_id, response);
}

void upper_task(char * received){
	char* msg_pid = strtok (NULL,"|");
	int pid = atoi(msg_pid);
	char* msg_cont = strtok (NULL,"|");
	int queue_id = get_client_queue_id(pid);
	for(int i = 0; msg_cont[i] != '\0'; i++)
		msg_cont[i] = toupper(msg_cont[i]);
	pid = getpid();
	char response[MAX_MSG_LEN];
	sprintf(response, "%d|%d|%s",UPPER,pid,msg_cont);
	send_msg(queue_id, response);
}

void time_task(char * received){
	char* msg_pid = strtok (NULL,"|");
	int pid = atoi(msg_pid);
	int queue_id = get_client_queue_id(pid);
	time_t timer;
	struct tm *tm_info;
	time(&timer);
	tm_info = localtime(&timer);
	pid = getpid();
	char response[MAX_MSG_LEN];
	sprintf(response, "%d|%d|%s",TIME,pid,asctime(tm_info));
	send_msg(queue_id, response);
}

void send_msg(int queue_id, char* received){
	if(queue_id < 0)
		return;
	char queue_name[20];
	sprintf(queue_name,"/%d",queue_id);
	mqd_t mq_client;
    mq_client = mq_open(queue_name, O_WRONLY);
    if((mqd_t)-1 == mq_client)
		perror("Mq_open error");
	if(mq_send(mq_client, received, MAX_MSG_LEN, 0) < 0) {
        fprintf(stderr,"mq_send error");
        exit(1);
    }
    mq_close(mq_client);
}

int get_client_queue_id(pid_t c_pid){
	for(int i = 0; i < clients_count; i++){
		if(c_pid == clients[i][0]){
			return clients[i][1];
		}
	}
	fprintf(stderr,"There is no such a client");
	return -1;
}