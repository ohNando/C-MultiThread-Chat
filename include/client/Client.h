#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define MAX_BUFSIZE 1024
#define NAME_LEN 32
#define SA struct sockaddr 

int sockfd = -1;

void* receiveMsgHandler(void*);
void* sendMsgHandler(void*);
int initClient(const char*,int);
void startClientLoop();

#endif