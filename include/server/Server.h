#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define NAME_LEN 32
#define MAX_CLIENT 10
#define MAX_BUFSIZE 1024
#define SA struct sockaddr 

typedef struct{
    int sockfd;
    char name[NAME_LEN];
    struct sockaddr_in addr;
    /*0: passive 1:active*/
    int active; 
}client_t;

client_t clients[MAX_CLIENT];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void error(const char*);
int initServer(int,char**);
void startServerLoop(int);

void chat(const char* , int);
void removeClient(int);
void* handleClient(void*);

#endif 