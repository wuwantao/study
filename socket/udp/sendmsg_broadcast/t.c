#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

void Sendmsg()
{
    struct sockaddr_in receiver_addr;
    struct sockaddr_in ser_addr;
    int sock_fd;
    char line[15] = "Hello World!";
    struct msghdr msg;
    struct iovec iov;
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
    msg.msg_name = &receiver_addr;
    msg.msg_namelen = sizeof(receiver_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_iov->iov_base = line;
    msg.msg_iov->iov_len = 13;
    msg.msg_control = 0;
    msg.msg_controllen = 0;
    msg.msg_flags = 0;
    sendmsg(sock_fd,&msg,0);
    close(sock_fd);
}

int main()
{
    int i = 0;
    for(;;)
    {
        Sendmsg();
        sleep(1);
    }
    return 0;
}
