#ifndef SERVER_H
#define SERVER_H

#include "common.h"
#include "utility.h"
#include "network.h"

typedef struct{
    int sockfd;
    char name[NAME_LEN];
    struct sockaddr_in addr;
    /*0: passive 1:active*/
    int active; 
    int pending_removal;
}client_t;

client_t clients[MAX_CLIENT];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void startServerLoop(int);

void chat(const char* , int);
void removeClient(int);
void* handleClient(void*);

#endif 