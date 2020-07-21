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
	int sockfd;
	char sendbuffer[200];
	char recvbuffer[200];
	struct sockaddr_in server_addr;
	struct hostent *host;
	int portnumber, nbytes;
	if (argc != 3) {
		fprintf(stderr, "Usage :%s hostname portnumber\a\n", argv[0]);
		exit(1);
	}
	if ((host = gethostbyname(argv[1])) == NULL) {
		herror("Get host name error\n");
		exit(1);
	}
	if ((portnumber = atoi(argv[2])) < 0) {
		fprintf(stderr, "Usage:%s hostname portnumber\a\n", argv[0]);
		exit(1);
	}
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));
		exit(1);
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portnumber);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);

	memset(sendbuffer, 1, sizeof(sendbuffer));
	while (1) {
		sendto(sockfd, sendbuffer, sizeof(sendbuffer), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
	}

	close(sockfd);
	exit(0);
}

