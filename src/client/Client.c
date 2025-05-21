#include "Client.h"

void* receiveMsgHandler(void* arg){
    (void)arg;
    char message[MAX_BUFSIZE];
    int n;
    
    while(1){
        memset(message,0,MAX_BUFSIZE - 1);
        if(sockfd < 0) break;

        n = read(sockfd, message, MAX_BUFSIZE - 1);

        if(n > 0){
            message[n] = '\0';
            printf("%s",message);
            fflush(stdout);
        }else if(n == 0){
            printf("(!)| Server disconnected.\n");
            if(sockfd > 0) shutdown(sockfd,SHUT_RD);
        }else{
            perror("(-)| Error reading from socket.\n");
            if(sockfd > 0) shutdown(sockfd,SHUT_RD);
        }
    }
    pthread_exit(NULL);
}

void* sendMsgHandler(void* arg){
    (void)arg;
    char buffer[MAX_BUFSIZE];

    while(1){
        printf("> ");
        fflush(stdout);

        if(fgets(buffer, MAX_BUFSIZE, stdin) == NULL){
            printf("(!)| Input closed.\n");
            if(sockfd > 0) shutdown(sockfd,SHUT_RD);
            break;
        }

        if(strncmp(buffer, "/exit", 5) == 0){
            printf("(!)| Exiting...");
            if(sockfd > 0){
                if(write(sockfd, buffer, strlen(buffer)) < 0)
                    perror("(-)| Error writing \'/exit\'");
                shutdown(sockfd,SHUT_WR);
            }
            break;
        }

        if(sockfd <= 0) break;

        if(write(sockfd, buffer, strlen(buffer)) < 0){
            perror("(-)| Error writing...\n");
            if(sockfd > 0) shutdown(sockfd, SHUT_WR);
            break;
        }
    }
    
    pthread_exit(NULL);
}

int initClient(const char* serverIP, int port){
    struct sockaddr_in serv_addr;
    char nameBuffer[NAME_LEN];
    char serverPrompt[MAX_BUFSIZE];
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("(-)| Error opening socket!\n");
        return -1;
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    printf("Attempting to connect inside initClient to IP: %s, Port: %d\n", serverIP, port);
    serv_addr.sin_port = htons(port);

    if(inet_pton(AF_INET, serverIP, &serv_addr.sin_addr) <= 0){
        fprintf(stderr, "Invalid address | address not supported..\n");
        close(sockfd);
        sockfd = -1;
        return -1;
    }

    if(connect(sockfd, (SA*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("(-)| Error connecting to server..\n");
        close(sockfd);
        sockfd = -1;
        return -1;
    }
    printf("(+)| Connected to server %s:%d\n",serverIP,port);

    memset(serverPrompt, 0, sizeof(serverPrompt));
    n = read(sockfd, serverPrompt, MAX_BUFSIZE - 1);
    if (n <= 0) {
        if (n == 0) printf("(!)| Server closed connection before name prompt.\n");
        else perror("(-)| Error reading name prompt from server");
        close(sockfd);
        sockfd = -1;
        return -1;
    }
    serverPrompt[n] = '\0';
    printf("%s", serverPrompt);
    fflush(stdout);

    if (fgets(nameBuffer, NAME_LEN, stdin) == NULL) {
        printf("(!)| Name input closed. Exiting.\n");
        close(sockfd);
        sockfd = -1;
        return -1;
    }

    if (write(sockfd, nameBuffer, strlen(nameBuffer)) < 0) {
        perror("(-)| Error writing name to socket");
        close(sockfd);
        sockfd = -1;
        return -1;
    }

    printf("Name sent. You can chat now ,have fun!\n");
    return 0;
}

void startClientLoop(){
    pthread_t sendTid , recvTid;

    if(pthread_create(&recvTid, NULL, &receiveMsgHandler, NULL) != 0){
        perror("(-)| Error creating receive thread!\n");
        if(sockfd > 0) close(sockfd);
        exit(1);
    }
    if(pthread_create(&sendTid, NULL, &sendMsgHandler, NULL) != 0){
        perror("(-)| Error creating send thread!\n");
        if(sockfd > 0) close(sockfd);
        exit(1);
    }

    printf("(+)| Threads created! Waiting for them joining...\n");

    if(sendTid && pthread_join(sendTid, NULL) != 0)
        perror("(-)| Error joining send thread!\n");
    
    if (recvTid && pthread_join(recvTid, NULL) != 0) {
        perror("(-)| Error joining receive thread!\n");
    }

    printf("(+)| Both threads have joined!\n");
}

int main(int argc, char** argv){
    if(argc < 3){
        fprintf(stderr, "(-)| Usage: %s <server_ip> <port>\n", argv[0]);
        exit(1);
    }

    char* serverIP = argv[1];
    int port = atoi(argv[2]);

    if(initClient(serverIP,port) != 0){
        fprintf(stderr, "(-)| Failed to setup client connection. Exiting.\n");
        exit(1);
    }

    startClientLoop();

    if(sockfd > 0){
        close(sockfd);
        sockfd = -1;
    }

    return 0;
}