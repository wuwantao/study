#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

void Sendto()
{
    struct sockaddr_in receiver_addr;
    struct sockaddr_in ser_addr;
    int sock_fd;
    char line[15] = "Hello World!";
    sock_fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    int broadcast = 1;
    int ret = 0;

    sock_fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if((ret = setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast))) < 0)
    {
        perror("setsockopt failed");
        exit(-1);
    }

    ser_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.147.131", &ser_addr.sin_addr);
    ser_addr.sin_port = htons(8888);
    bind(sock_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));

    receiver_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "255.255.255.255", &receiver_addr.sin_addr);
    receiver_addr.sin_port = htons(5000);
    sendto(sock_fd, line, 13, 0,(struct sockaddr*)&receiver_addr,sizeof(receiver_addr));
    close(sock_fd);
}

int main()
{
    int i = 0;
    for(;;)
    {
        Sendto();
        sleep(1);
    }
    return 0;
}
