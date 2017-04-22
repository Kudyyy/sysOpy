#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "settings.h"

void send_msg_to(int server_que, msg_st *msg);
void echo_please(int que,char*);
void time_please(int que);
void upper_please(int que,char*);
void terminate_serv(int que);
void logout_from_serv(int que);
int get_private_queue_id(void);
void get_message_from(int que,msg_st *msg);
int log_in(int server_que, int priv_que);
int get_server_queue_id(void);


int main(int argc, char *argv[]){
	int servq_id;
    servq_id = get_server_queue_id();
    int privq_id;
    privq_id = get_private_queue_id();
    int session_id;
    printf("Connecting to server\n");
    session_id = log_in(servq_id, privq_id);

    printf("Connected to server, session id: %d\n", session_id);

    char *line = NULL;
    size_t size = 0;
    int is_running = 0;
    msg_st msg;
    while(is_running == 0){
    	ssize_t charsRead = getline(&line, &size, stdin);
		if(charsRead != -1){
            int todo = atoi(line);
            switch(todo){
                case 0:
                    getline(&line, &size, stdin);
                    echo_please(servq_id, line);
                    get_message_from(privq_id,&msg);
                    printf("%s\n", msg.content);
                    break;
                case 1:
                    time_please(servq_id);
                    get_message_from(privq_id,&msg);
                    printf("%s\n", msg.content);
                    break;
                case 2:
                    getline(&line, &size, stdin);
                    upper_please(servq_id,line);
                    get_message_from(privq_id,&msg);
                    printf("%s\n", msg.content);
                    break;
                case 3:
                    terminate_serv(servq_id);
                    is_running = 1;
                    break;
                case 4:
                    logout_from_serv(servq_id);
                    is_running = 1;
                    break;
                default:
                    break;
            }

		}
	}

	if((msgctl(privq_id, IPC_RMID, NULL)) < 0)
        fprintf(stderr,"msgctlerror:");
    
    return 0;
}

void echo_please(int que,char* line){
    msg_st msg;
    msg.type = ECHO;
    msg.sender_pid = getpid();
    sprintf(msg.content,"%s", line);
    send_msg_to(que, &msg);
}

void time_please(int que){
    msg_st msg;
    msg.type = TIME;
    msg.sender_pid = getpid();
    send_msg_to(que, &msg);
}

void upper_please(int que,char* line){
    msg_st msg;
    msg.type = UPPER;
    msg.sender_pid = getpid();
    sprintf(msg.content,"%s", line);
    send_msg_to(que, &msg);
}

void terminate_serv(int que){
    msg_st msg;
    msg.type = END;
    msg.sender_pid = getpid();
    send_msg_to(que, &msg);
}

void logout_from_serv(int que){
    msg_st msg;
    msg.type = LOGOUT;
    msg.sender_pid = getpid();
    send_msg_to(que, &msg);
}

int get_private_queue_id(void){
    int queue_id;
    queue_id = msgget(IPC_PRIVATE, 0600);
    if(queue_id < 0){
        fprintf(stderr,"Msgget error\n");
        exit(1);
    }
    return queue_id;
}

void get_message_from(int que,msg_st *msg){
    if(msgrcv(que, msg, MSG_SIZE, 0, 0) < 0) {
        fprintf(stderr,"Msgrcv error");
        exit(1);
    }
}

int log_in(int server_que, int priv_que){
    msg_st msg;
    msg.type = LOGIN;
    msg.sender_pid = getpid();
    sprintf(msg.content, "%d", priv_que);
    send_msg_to(server_que,&msg);
    get_message_from(priv_que,&msg);
    if(msg.type == DUMP){
        printf("Connection refused\n");
        exit(1);
    }
    if(msg.type == EST)
        printf("Connection estabilshed\n");
    int sess_id = atoi(msg.content);
    return sess_id;
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



// todo -> refine this
void send_msg_to(int server_que, msg_st *msg){
	if(msgsnd(server_que, msg, MSG_SIZE, 0) < 0) {
        fprintf(stderr,"Msgsnd error\n");
        exit(1);
    }
}
