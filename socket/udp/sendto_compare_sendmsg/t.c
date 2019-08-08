#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


void Sendto()
{
    struct sockaddr_in receiver_addr;
    struct sockaddr_in ser_addr;
    static int sock_fd = 0;
    static int bind_flag = 0;
    char line[15] = "Hello World!";
    if (sock_fd != 0)
        sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    int ret = 0;
    struct in_pktinfo pktinfo;

    memset(&pktinfo, 0, sizeof (pktinfo));
    /* 没有效果 
     * 会走默认路由
     */
    pktinfo.ipi_ifindex = 3;

    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


    ser_addr.sin_family = AF_INET;
    //inet_pton(AF_INET, "192.168.147.131", &ser_addr.sin_addr);
    inet_pton(AF_INET, "0.0.0.0", &ser_addr.sin_addr);
    ser_addr.sin_port = htons(8888);
    if (bind_flag == 0) {
        bind(sock_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
        bind_flag = 1;
    }

    if (setsockopt(sock_fd, IPPROTO_IP,
                   IP_PKTINFO, (char *)&pktinfo,
                   sizeof(pktinfo)) < 0)
        printf("setsockopt: IP_PKTINFO: %s", strerror(errno));


    receiver_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "8.8.8.8", &receiver_addr.sin_addr);
    receiver_addr.sin_port = htons(5000);

    ret = sendto(sock_fd, line, 13, 0, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
    printf("ret:%d\n", ret);
    if (ret <= 0) {
        perror("sendto failed\n");
    }

    //close(sock_fd);
}

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
    struct in_pktinfo *pktinfo;
    struct cmsghdr *cmsg;
    struct sockaddr_in pkt_dst_tmp;

    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if ((ret = setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast))) < 0) {
        perror("setsockopt failed");
        exit(-1);
    }

    ser_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &ser_addr.sin_addr);
    ser_addr.sin_port = htons(8888);
    bind(sock_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));

    receiver_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "8.8.8.8", &receiver_addr.sin_addr);
    receiver_addr.sin_port = htons(5000);
    msg.msg_name = &receiver_addr;
    msg.msg_namelen = sizeof(receiver_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_iov->iov_base = line;
    msg.msg_iov->iov_len = 13;

    msg.msg_controllen = CMSG_SPACE(sizeof(struct in_pktinfo));
    msg.msg_control = malloc(msg.msg_controllen);
    cmsg = CMSG_FIRSTHDR(&msg);

    cmsg->cmsg_level = IPPROTO_IP;
    cmsg->cmsg_type = IP_PKTINFO;
    cmsg->cmsg_len = CMSG_LEN(sizeof(*pktinfo));
    pktinfo = (struct in_pktinfo *)CMSG_DATA(cmsg);
    memset(pktinfo, 0, sizeof(*pktinfo));
    inet_pton(AF_INET, "1.1.1.1", &pkt_dst_tmp.sin_addr);
//    pktinfo->ipi_addr = pkt_dst_tmp.sin_addr;
    pktinfo->ipi_spec_dst = pkt_dst_tmp.sin_addr;
    /* 为0 就走默认路由  可以指定源IP 源IP和默认路由出口设备上的IP可以不一样*/
    pktinfo->ipi_ifindex = 0; //eth0


    msg.msg_flags = 0;
    ret = sendmsg(sock_fd, &msg, 0);
    printf("ret:%d\n", ret);
    close(sock_fd);
}

int main()
{
    int i = 0;
    for (;;) {
        Sendto();
        Sendmsg();
        sleep(1);
    }
    return 0;
}
