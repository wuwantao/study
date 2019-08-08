#include<stdio.h>
#include<errno.h>
#include<netdb.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>

int main(int argc, char *argv[])
{
    int port = 0;
    int sockfd = 0;
    int nbytes = 0;
    struct hostent *host = NULL;
    struct sockaddr_in server_addr;
    char sendbuffer[200] = {[0 ... sizeof(sendbuffer) - 1] = 0};
    char recvbuffer[200] = {[0 ... sizeof(sendbuffer) - 1] = 0};

    if (3 != argc) {
        fprintf(stderr, "Usage :%s hostname portnumber\a\n", argv[0]);
        exit(1);
    }
    if ((host = gethostbyname(argv[1])) == NULL) {
        perror("Get host name error");
        exit(1);
    }
    if ((port = atoi(argv[2])) < 0) {
        fprintf(stderr, "Usage:%s hostname portnumber\a\n", argv[0]);
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("create socket error");
        exit(1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    if (connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1) {
        perror("Connect error");
        exit(1);
    }
//    while (1) {
//        printf("Please input your word:\n");
//        scanf("%s", sendbuffer);
//        printf("\n");
//        if (strcmp(sendbuffer, "quit") == 0)
//            break;
//        send(sockfd, sendbuffer, sizeof(sendbuffer), 0);
//        recv(sockfd, recvbuffer, 200, 0);
//        printf("recv data of my world is :%s\n", recvbuffer);
//    }
//    send(sockfd, sendbuffer, sizeof(sendbuffer), 0);

    close(sockfd);
    exit(0);
}

