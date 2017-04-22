#ifndef SETTINGS_H
#define SETTINGS_H

#define MAX_CLIENTS 4
#define MAX_MSG_LEN 100
#define SERVER_QUEUE "/SERVER_QUEUE\0"

typedef enum msg_type
{
	LOGIN = 1, LOGOUT = 2, DUMP = 3, EST = 4, ECHO = 5, UPPER = 6, TIME = 7, END = 8
} msg_type;

typedef struct msg_st
{
	float type;
	pid_t sender_pid;
	char content[MAX_MSG_LEN];
} msg_st;
	
static const int PROJECT_ID = 7;
static const int MSG_SIZE = sizeof(msg_st) - sizeof(float);

#endif