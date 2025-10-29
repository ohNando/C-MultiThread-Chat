#ifndef COMMON_H
#define COMMON_H

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
#define PROMPT "> "

#define MAX_CLIENT 10

#define UNLOCK pthread_mutex_unlock
#define LOCK pthread_mutex_lock

extern pthread_mutex_t stdoutMutex;

#endif