#include "../include/client.h"

void* receiveMsgHandler(void* arg){
    int cli_sockfd = *(int*)arg;
    char message[MAX_BUFSIZE];
    ssize_t bytes_recv;

    while(1){
        memset(message,0,MAX_BUFSIZE - 1);
        if(cli_sockfd <= 0) break;

        bytes_recv = read(cli_sockfd, message, MAX_BUFSIZE - 1);

        if(bytes_recv > 0){
            message[bytes_recv] = '\0';
            printToConsole(message,1);
        }else{
            if(bytes_recv == 0) printToConsole("(!)| Server Disconnected.\n",0);
            else{
                char errMsg[MAX_BUFSIZE];
                snprintf(errMsg,MAX_BUFSIZE,"(-)| Error reading from socket: %s\n",strerror(errno));
                printToConsole(errMsg, 0);
            }
            if(cli_sockfd > 0) shutdown(cli_sockfd, SHUT_RD);
            break;
        }
    }
    pthread_exit(NULL);
}

void* sendMsgHandler(void* arg){
    int cli_sockfd = *(int*)arg;
    char buffer[MAX_BUFSIZE];
    ssize_t bytes_written;

    while(1){
        LOCK(&stdoutMutex);
        printf("%s",PROMPT);
        fflush(stdout);
        UNLOCK(&stdoutMutex);

        if(fgets(buffer, MAX_BUFSIZE, stdin) == NULL){
            printToConsole("(!)| Input closed. Disconnecting...\n",0);
            if(cli_sockfd > 0) shutdown(cli_sockfd,SHUT_WR);    
            break;
        }

        if(strncmp(buffer, "/exit", 5) == 0){
            printToConsole("(!)| Exiting with \'/Exit\' command..\n",0);
            if(cli_sockfd > 0){
                write(cli_sockfd, buffer, strlen(buffer));
                shutdown(cli_sockfd,SHUT_WR);
            }
            break;
        }

        if(cli_sockfd <= 0) break;

        bytes_written = write(cli_sockfd, buffer, strlen(buffer));
        if(bytes_written < 0){
            char errMsg[MAX_BUFSIZE];
            snprintf(errMsg, MAX_BUFSIZE, "(-)| Error writing to socket: %s\n",
                strerror(errno));
            printToConsole(errMsg, 0);
                
            break;
        }
    }
    
    pthread_exit(NULL);
}

int initClient(const char* serverIP, int port){
    int new_sockfd;
    char nameBuffer[NAME_LEN];
    char serverPrompt[MAX_BUFSIZE];
    ssize_t n;

    new_sockfd = connectToServer(serverIP, port);
    if(new_sockfd < 0)
        return -1;

    memset(serverPrompt, 0, sizeof(serverPrompt));    
    n = read(new_sockfd, serverPrompt, MAX_BUFSIZE - 1);
    if (n <= 0) {
        if(n == 0) printf("(!)| Server closed connection before name prompt.\n");
        else perror("(-)| Error reading name prompt from server");
        close(new_sockfd);
        return -1;
    }
    serverPrompt[n] = '\0';

    LOCK(&stdoutMutex);
    printf("\r\033[K%s", serverPrompt);
    fflush(stdout);
    UNLOCK(&stdoutMutex);

    if (fgets(nameBuffer, NAME_LEN, stdin) == NULL) {
        printf("(!)| Name input closed. Exiting.\n");
        close(new_sockfd);
        return -1;
    }
    nameBuffer[strcspn(nameBuffer, "\r\n")] = 0; //Removing newline character

    if (write(new_sockfd, nameBuffer, strlen(nameBuffer)) < 0) {
        perror("(-)| Error writing name to socket");
        close(new_sockfd);
        return -1;
    }

    return new_sockfd;
}

void startClientLoop(int loop_sockfd){
    pthread_t sendTid = 0 , recvTid = 0;

    if(loop_sockfd <= 0){
        fprintf(stderr, "(-)| Invalid sockfd.\n");
        return;
    }
    printToConsole("(+)| Main: You can now chat. Type \'/Exit\' to leave.\n",1);

    if(pthread_create(&recvTid, NULL, &receiveMsgHandler, (void*)&loop_sockfd) != 0){
        perror("(-)| Error creating receive thread!\n");
        close(loop_sockfd);
        return;
    }
    if(pthread_create(&sendTid, NULL, &sendMsgHandler, (void*)&loop_sockfd) != 0){
        perror("(-)| Error creating send thread!\n");
        if(loop_sockfd > 0) close(loop_sockfd);
        //Waiting for the other thread to finish 
        if(recvTid != 0) pthread_join(recvTid, NULL);
        close(loop_sockfd);
        return;
    }

    if(sendTid && pthread_join(sendTid, NULL) != 0)
        perror("(-)| Error joining send thread!\n");
    
    if (recvTid && pthread_join(recvTid, NULL) != 0) {
        perror("(-)| Error joining receive thread!\n");
    }
}

int main(int argc, char** argv){
    if(argc < 3){
        fprintf(stderr, "(-)| Usage: %s <server_ip> <port>\n", argv[0]);
        exit(1);
    }

    char* serverIP = argv[1];
    int port = atoi(argv[2]);
    int main_sockfd;

    main_sockfd = initClient(serverIP, port);
    if(main_sockfd < 0){
        fprintf(stderr, "(-)| Failed to setup client connection. Exiting.\n");
        exit(1);
    }

    startClientLoop(main_sockfd);
    if(main_sockfd > 0) close(main_sockfd);

    return 0;
}