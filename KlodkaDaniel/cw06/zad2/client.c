#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>
#include <sys/fcntl.h>
#include "settings.h"

void send_msg_to(int server_que, char *msg);
void echo_please(int que,char*);
void time_please(int que);
void upper_please(int que,char*);
void terminate_serv(int que);
void logout_from_serv(int que);
int get_private_queue_id(void);
void get_message_from(int que,char *msg);
int log_in(int server_que, int priv_que);


int main(int argc, char *argv[]){

	mqd_t mq_priv;
    struct mq_attr attr;
	attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_LEN;
    attr.mq_curmsgs = 0;
	char priv_queue_name[50];
	memset(priv_queue_name,'\0',10);
	sprintf(priv_queue_name,"/%d",getpid());
	
	mq_priv = mq_open(priv_queue_name, O_CREAT | O_RDONLY, 0644, &attr);
	if((mqd_t)-1 == mq_priv)
		perror("Mq_open error");
	
	mqd_t mq_serv;
    mq_serv = mq_open(SERVER_QUEUE, O_WRONLY);
	if((mqd_t)-1 == mq_serv)
		perror("Mq_open error");
	
    int session_id;
    printf("Connecting to server\n");
    session_id = log_in(mq_serv, mq_priv);

    printf("Connected to server, session id: %d\n", session_id);

    char *line = NULL;
    size_t size = 0;
    int is_running = 0;
    char msg[MAX_MSG_LEN];
    char* msg_type;
    char* msg_pid;
    char* msg_cont;
    while(is_running == 0){
        memset(msg, 0, MAX_MSG_LEN);
    	ssize_t charsRead = getline(&line, &size, stdin);
		if(charsRead != -1){
            int todo = atoi(line);
            switch(todo){
                case 0:
                    getline(&line, &size, stdin);
                    echo_please(mq_serv, line);
                    get_message_from(mq_priv,msg);
                    msg_type = strtok (msg,"|");
                    msg_pid = strtok (NULL,"|");
                    msg_cont = strtok (NULL,"|");
                    printf("%s\n", msg_cont);
                    break;
                case 1:
                    time_please(mq_serv);
                    get_message_from(mq_priv,msg);
                    msg_type = strtok (msg,"|");
                    msg_pid = strtok (NULL,"|");
                    msg_cont = strtok (NULL,"|");
                    printf("%s\n", msg_cont);
                    break;
                case 2:
                    getline(&line, &size, stdin);
                    upper_please(mq_serv,line);
                    get_message_from(mq_priv,msg);
                    msg_type = strtok (msg,"|");
                    msg_pid = strtok (NULL,"|");
                    msg_cont = strtok (NULL,"|");
                    printf("%s\n", msg_cont);
                    break;
                case 3:
                    terminate_serv(mq_serv);
                    is_running = 1;
                    break;
                case 4:
                    logout_from_serv(mq_serv);
                    is_running = 1;
                    break;
                default:
                    break;
            }

		}
	}

	mq_close(mq_priv);
    mq_unlink(priv_queue_name);
    
    return 0;
}

void echo_please(int que,char* line){
    char buffer[MAX_MSG_LEN];
    memset(buffer, 0, MAX_MSG_LEN);
    sprintf(buffer,"%d|%d|%s",ECHO,getpid(),line);
    send_msg_to(que, buffer);
}

void time_please(int que){
    char buffer[MAX_MSG_LEN];
    memset(buffer, 0, MAX_MSG_LEN);
    sprintf(buffer,"%d|%d|",TIME,getpid());
    send_msg_to(que, buffer);
}

void upper_please(int que,char* line){
    char buffer[MAX_MSG_LEN];
    memset(buffer, 0, MAX_MSG_LEN);
    sprintf(buffer,"%d|%d|%s",UPPER,getpid(),line);
    send_msg_to(que, buffer);
}

void terminate_serv(int que){
    char buffer[MAX_MSG_LEN];
    memset(buffer, 0, MAX_MSG_LEN);
    sprintf(buffer,"%d|%d|",END,getpid());
    send_msg_to(que, buffer);
}

void logout_from_serv(int que){
    char buffer[MAX_MSG_LEN];
    memset(buffer, 0, MAX_MSG_LEN);
    sprintf(buffer,"%d|%d|",LOGOUT,getpid());
    send_msg_to(que, buffer);
}

void get_message_from(int que,char *msg){
    ssize_t bytes_read = mq_receive(que, msg, MAX_MSG_LEN, NULL);
        if(bytes_read < 0) 
           perror("Mq_receive error");
}

int log_in(int server_que, int priv_que){
	char buffer[MAX_MSG_LEN];
	memset(buffer, 0, MAX_MSG_LEN);
    int pid = getpid();
    sprintf(buffer, "%d|%d|%d",LOGIN,pid,priv_que);
    send_msg_to(server_que,buffer);
	memset(buffer, 0, MAX_MSG_LEN);
    get_message_from(priv_que,buffer);
    char* msg_type = strtok (buffer,"|");
    int rec = atoi(msg_type);
    if(rec == DUMP){
        printf("Connection refused\n");
        exit(1);
    }
    if(rec == EST)
        printf("Connection estabilshed\n");
    char* msg_pid = strtok (NULL,"|");
    char* msg_cont = strtok (NULL,"|");
    int sess_id = atoi(msg_cont);
    return sess_id;
}   


void send_msg_to(int server_que, char *msg){
    if(mq_send(server_que, msg, MAX_MSG_LEN, 0) < 0) {
        fprintf(stderr,"mq_send error");
        exit(1);
    }
}
