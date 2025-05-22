#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define MAX_BUFSIZE 1024
#define NAME_LEN 32
#define SA struct sockaddr

#define UNLOCK pthread_mutex_unlock
#define LOCK pthread_mutex_lock

#define PROMPT "> "
pthread_mutex_t stdoutMutex = PTHREAD_MUTEX_INITIALIZER;

void error(const char*);
void printToConsole(const char*,int);

void* receiveMsgHandler(void*);
void* sendMsgHandler(void*);
int initClient(const char*,int);
void startClientLoop(int);

#endif