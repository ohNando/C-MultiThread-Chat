#include "Server.h"

void error(const char* szMsg){
    perror(szMsg);
    exit(1);
}

int initServer(int argc,char** argv){
    if(argc < 2){
        fprintf(stderr,"(-)| Port number not provided! Program terminated..\n");
        exit(1);
    }

    int listen_sockfd;
    int iPortNo = atoi(argv[1]);
    struct sockaddr_in serv_addr;

    for(int i = 0; i < MAX_CLIENT; i++){
        clients[i].active = 0;
    }

    listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sockfd == -1) error("(-)| Socket creation failed!\n");
    printf("(+)| Socket successfully created!\n");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(iPortNo);

    int optval = 1;
    setsockopt(listen_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if(bind(listen_sockfd, (SA*) &serv_addr, sizeof(serv_addr)) != 0){
        close(listen_sockfd);
        error("(-)|Socket bind failed...\n");
    }
    printf("(+)| Socket successfully binded to port :%d...\n",iPortNo);


    if(listen(listen_sockfd,MAX_CLIENT) != 0){
        close(listen_sockfd);
        error("(-)|Listen failed...\n");
    }
    printf("(+)| Server is listening on %d...\n",iPortNo);

    return listen_sockfd;
}

void chat(const char* szMessage, int sender_sockfd){
    pthread_mutex_lock(&clients_mutex);
    for(int i = 0; i < MAX_CLIENT; i++){
        if(clients[i].active && clients[i].sockfd != sender_sockfd){
            if (write(clients[i].sockfd, szMessage, strlen(szMessage)) < 0) {
                perror("(-)| Error writing to chat socket");
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void removeClient(int rem_sockfd) {
    char leaveMsg[MAX_BUFSIZE] = {0}; 
    struct sockaddr_in temp_addr;
    char nameBuf[NAME_LEN] = {0};
    int found = 0;

    int cli_index_debug = -1;

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (clients[i].active && clients[i].sockfd == rem_sockfd) {
            cli_index_debug = i;
            clients[i].active = 0;
            strncpy(nameBuf, clients[i].name, NAME_LEN -1);
            nameBuf[NAME_LEN - 1] = '\0';
            temp_addr = clients[i].addr;

            clients[i].active = 0;
            close(clients[i].sockfd);
            clients[i].sockfd = -1; 
            memset(clients[i].name, 0, NAME_LEN);

            found = 1;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);


    if (found) {
        char ip_str[INET_ADDRSTRLEN];
        strncpy(ip_str, inet_ntoa(temp_addr.sin_addr), INET_ADDRSTRLEN - 1);
        ip_str[INET_ADDRSTRLEN - 1] = '\0';

        printf("(!)| Client \'%s\' (slot: %d, original fd: %d) from %s:%d has been removed!\n",
        (strlen(nameBuf) > 0 ? nameBuf : "[NoName]"),
        cli_index_debug,
        rem_sockfd,
        ip_str,
        ntohs(temp_addr.sin_port)
        );

        if(strlen(nameBuf) > 0){
            char leaveMsg[MAX_BUFSIZE];
            snprintf(leaveMsg, sizeof(leaveMsg),
                     "SERVER: %s has left the chat.\n", nameBuf);
            chat(leaveMsg, -1);
        }
    }else{
        printf("[WARN] removeClient called for sockfd %d,but no active client found.\n"
            , rem_sockfd);
    }
}

void* handleClient(void* arg){
    client_t* pClient = (client_t*)arg;
    int client_sockfd = pClient->sockfd;
    char clientName[NAME_LEN];
    char buffer[MAX_BUFSIZE];
    int n;

    const char* nameMsg = "SERVER: Welcome! Please enter your name: ";
    if(write(client_sockfd, nameMsg, strlen(nameMsg)) < 0){
        perror("(-)| Error writing name message !");
        removeClient(client_sockfd);
        pthread_exit(NULL);
    }

    n = read(client_sockfd, clientName, NAME_LEN-1);
    if(n <= 0){
        if(n == 0) printf("(-)| Client (fd:%d) disconneting before sending name!\n",client_sockfd);
        else perror("(-)| Error reading name!");
        removeClient(client_sockfd);
        pthread_exit(NULL);
    }

    clientName[strcspn(clientName, "\r\n")] = 0;
    strncpy(pClient->name, clientName, NAME_LEN - 1);
    pClient->name[NAME_LEN - 1] = '\0';

    printf("(+)| %s (fd: %d) connected from %s:%d.\n",
        pClient->name,client_sockfd,
        inet_ntoa(pClient->addr.sin_addr),
        ntohs(pClient->addr.sin_port)
    );

    char joinMsg[MAX_BUFSIZE];
    char message[MAX_BUFSIZE + NAME_LEN + 20]; 
    snprintf(joinMsg, sizeof(joinMsg), "SERVER: %s has joined the char.\n",pClient->name);
    chat(joinMsg,client_sockfd);

    while(1){
        memset(buffer, 0, MAX_BUFSIZE);
        n = read(client_sockfd, buffer, MAX_BUFSIZE - 1);

        if(n <= 0){
            if(n == 0) printf("(-)| Client has disconnected gracefully..\n");
            else perror("(-)| Error reading from socket..");

            removeClient(client_sockfd);
            break;
        }

        buffer[strcspn(buffer,"\r\n")] = 0;

        if(strcmp(buffer, "/exit") == 0){
            printf("SERVER: Client %s sent /exit command.\n", pClient->name);
            removeClient(client_sockfd);
            break;
        }

        memset(message,0,sizeof(message));
        snprintf(message, sizeof(message), "%s: %s\n",
            pClient->name,
            buffer
        );
        printf("Sending = %s\n", message);
        chat(message,client_sockfd);
    }
    pthread_exit(NULL);
}

void startServerLoop(int listen_sockfd){
    int newConnSockfd;
    struct sockaddr_in cli_addr;
    socklen_t cliLen;
    pthread_t tid;

    while(1){
        cliLen = sizeof(cli_addr);
        newConnSockfd = accept(listen_sockfd, (SA*)&cli_addr, &cliLen);

        if(newConnSockfd < 0){
            perror("(-)| Server accept failed...\n");
            if(newConnSockfd == -1 ) continue;
            else error("(-)| Fatal error in accept loop!\n");
        }

        int i;
        pthread_mutex_lock(&clients_mutex);
        for(i = 0; i < MAX_CLIENT; i++){
            if(!clients[i].active){
                clients[i].sockfd = newConnSockfd;
                clients[i].addr = cli_addr;
                clients[i].active = 1;
                memset(clients[i].name, 0, NAME_LEN);
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        if(i == MAX_CLIENT){
            printf("(-)| Max clients reached. Connection rejected for: %s:%d.\n",
                    inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
            const char* reject_msg = "SERVER: Chat room is full.";
            write(newConnSockfd, reject_msg, strlen(reject_msg));
            close(newConnSockfd);
        }else{
            if(pthread_create(&tid, NULL, &handleClient, (void*)&clients[i]) != 0){
                perror("(-)| Error creating thread!\n");
                pthread_mutex_lock(&clients_mutex);
                clients[i].active = 0;
                pthread_mutex_unlock(&clients_mutex);
                close(newConnSockfd);
            }else{
                pthread_detach(tid);
                printf("(+)| New connection accepted. Client %d (fd: %d).\n",i,newConnSockfd);
            }
        }
    }
}

int main(int argc,char** argv){
    int listen_sockfd = initServer(argc,argv);
    startServerLoop(listen_sockfd);
    close(listen_sockfd);
    pthread_mutex_destroy(&clients_mutex);
    printf("(-)| Server determinated!\n");
    return 0;
}