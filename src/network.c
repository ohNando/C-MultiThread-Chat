#include "../include/network.h"

int connectToServer(const char* serverIP, int port){
    int new_sockfd;
    struct sockaddr_in server_addr;

    new_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(new_sockfd < 0){
        error("(-)| Error opening socket");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if(inet_pton(AF_INET, serverIP, &server_addr.sin_addr) <= 0){
        error("(-)| Invalid address | Address not supported.\n");
        close(new_sockfd);
        return -1;
    }

    if(connect(new_sockfd, (SA*)&server_addr, sizeof(server_addr)) < 0){
        error("(-)| Error connecting to server");
        close(new_sockfd);
        return -1;
    }
    printf("(+)| Connected to server %s:%d\n",serverIP,port);

    return new_sockfd;
}

int start_listening(int iPort){
    int listen_sockfd;
    struct sockaddr_in server_addr;

    listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sockfd == -1){ error("(-)| Socket creating failed!");}
    printf("(+)| Socket created successfully!\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(iPort);

    int optval = 1;
    setsockopt(listen_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if(bind(listen_sockfd, (SA*)&server_addr, sizeof(server_addr)) != 0){
        close(listen_sockfd);
        error("(-)| Socket bind failed...");
    }
    printf("(+)| Socket successfully binded to port :%d...\n",iPort);

    if(listen(listen_sockfd, MAX_CLIENT) != 0){
        close(listen_sockfd);
        error("(-)| Listen failed...");
    }

    printf("(+)| Listening on port %d\n", iPort);
    return listen_sockfd;
}
