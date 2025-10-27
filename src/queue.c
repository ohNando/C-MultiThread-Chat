#include "queue.h"

void init_queue(MessageQueue* queue) {
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond_has_data, NULL);
    pthread_cond_init(&queue->cond_has_space, NULL);
}

int enqueue(MessageQueue* queue, const char* message){
    if(!message) return -1;
    size_t len = strlen(message);
    if(len >= MAX_BUFSIZE) return -1;

    LOCK(&queue->mutex);
    while(queue->count == MAX_QUEUE_SIZE){
        pthread_cond_wait(&queue->cond_has_space, &queue->mutex);
    }

    strncpy(queue->messages[queue->tail], message, MAX_BUFSIZE - 1);
    queue->messages[queue->tail][MAX_BUFSIZE - 1] = '\0';
    
    queue->tail = (queue->tail + 1) % MAX_QUEUE_SIZE;
    queue->count++;

    pthread_cond_broadcast(&queue->cond_has_data);
    UNLOCK(&queue->mutex);
    return 0;
}

int dequeue(MessageQueue* queue, char* message, size_t bufferSize){
    if(bufferSize < MAX_BUFSIZE) return -1;

    LOCK(&queue->mutex);
    while(queue->count == 0){
        pthread_cond_wait(&queue->cond_has_data, &queue->mutex);
    }

    strncpy(message, queue->messages[queue->head], MAX_BUFSIZE - 1);
    message[MAX_BUFSIZE - 1] = '\0';

    queue->head = (queue->head + 1) % MAX_QUEUE_SIZE;
    queue->count--;

    pthread_cond_broadcast(&queue->cond_has_space);
    UNLOCK(&queue->mutex);
    return 0;
}

int dequeue_nonblocking(MessageQueue *queue, char *buffer, size_t buffer_size){
    if(buffer_size < MAX_BUFSIZE) return -1;

    LOCK(&queue->mutex);
    if(queue->count == 0){
        UNLOCK(&queue->mutex);
        return -1; // Queue is empty
    }

    strncpy(buffer, queue->messages[queue->head], MAX_BUFSIZE - 1);
    buffer[MAX_BUFSIZE - 1] = '\0';

    queue->head = (queue->head + 1) % MAX_QUEUE_SIZE;
    queue->count--;

    pthread_cond_broadcast(&queue->cond_has_space);
    UNLOCK(&queue->mutex);

    return 0;
}