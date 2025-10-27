#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

#define MAX_QUEUE_SIZE 50

typedef struct {
    char messages[MAX_QUEUE_SIZE][MAX_BUFSIZE];
    int head, tail, count;
    
    pthread_mutex_t mutex;
    pthread_cond_t cond_has_data; //for indicating data is available
    pthread_cond_t cond_has_space; //for indicating available space is available
}MessageQueue;

void init_queue(MessageQueue*);
int enqueue(MessageQueue*, const char*);
int dequeue(MessageQueue*, char*, size_t);
int dequeue_nonblocking(MessageQueue*, char*, size_t);

#endif