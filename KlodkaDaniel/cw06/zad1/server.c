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
#include "settings.h"


int get_server_queue_id(void);
void execute_msg(msg_st *rec);
void log_in(msg_st* received);
void log_out(msg_st* received);
void echo_task(msg_st* received);
void upper_task(msg_st* received);
void time_task(msg_st* received);
void send_msg(int queue_id, msg_st* received);
int get_client_queue_id(pid_t c_pid);

int clients[MAX_CLIENTS][2];
int clients_count = 0; 
int client_id = 0;
int is_running;

int main(int argc, char *argv[]){
	int servq_id;
	servq_id = get_server_queue_id();
	msg_st buff; 
	is_running = 0;
	printf("Server pid: %d\n", getpid());
	while(is_running == 0){
	  	if(msgrcv(servq_id, &buff, MSG_SIZE, 0, 0) < 0) 
            continue;
        execute_msg(&buff);
	}
	if((msgctl(servq_id, IPC_RMID, NULL)) < 0){
		fprintf(stderr,"Msgctl error");
	}

    return 0;
}

void execute_msg(msg_st *rec){
	if(rec == NULL) return;
	switch((int)rec->type){
		case LOGIN:
			log_in(rec);
			break;
		case LOGOUT:
			log_out(rec);
			break;
		case ECHO:
			echo_task(rec);
			break;
		case UPPER:
			upper_task(rec);
			break;
		case TIME:
			time_task(rec);
			break;
		case END:
			is_running = 1;
			break;
		default:
			break;
	}
}

int get_server_queue_id(void){
	char *home = getenv("HOME");
	if(home == NULL){
		perror("Could not get $HOME variable");
		exit(1);
	}	
	key_t key = ftok(home, PROJECT_ID);
	if(key == -1){
		perror("Ftok error");	
		exit(1);
	}
	int id = msgget(key, IPC_CREAT | 0600);
	if(id == -1){
		perror("Msgget error");
	}
	return id;
}


void log_in(msg_st* received){
	printf("%d is connecting\n", received->sender_pid);
	msg_st response;
	response.sender_pid = getpid();
	int queue_id = atoi(received->content);
	if(clients_count == MAX_CLIENTS){
		response.type = DUMP;
		printf("%d connection refused, server is full\n", received->sender_pid);

	}
	else{
		clients[clients_count][0] = received->sender_pid;
		clients[clients_count][1] = queue_id;
		clients_count++;
		response.type = EST;
		sprintf(response.content, "%d", client_id++);
		printf("%d connected\n", received->sender_pid);
	}
	send_msg(queue_id, &response);
}

void log_out(msg_st* received){
	int pid = received->sender_pid;
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

void echo_task(msg_st* received){
	int queue_id = get_client_queue_id(received->sender_pid);
	received->sender_pid = getpid();
	send_msg(queue_id, received);
}

void upper_task(msg_st* received){
	int queue_id = get_client_queue_id(received->sender_pid);
	for(int i = 0; received->content[i] != '\0'; i++)
		received->content[i] = toupper(received->content[i]);
	received->sender_pid = getpid();
	send_msg(queue_id, received);
}

void time_task(msg_st* received){
	int queue_id = get_client_queue_id(received->sender_pid);
	time_t timer;
	struct tm *tm_info;
	time(&timer);
	tm_info = localtime(&timer);
	sprintf(received->content, "%s", asctime (tm_info));
	received->sender_pid = getpid();
	send_msg(queue_id, received);
}

void send_msg(int queue_id, msg_st* received){
	if(queue_id < 0)
		return;
	if(msgsnd(queue_id, received, MSG_SIZE, 0) == -1) {
        fprintf(stderr,"Msgsnd error");
        exit(1);
    }
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