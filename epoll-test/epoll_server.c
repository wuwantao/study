#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define LISTENQ 20
#define SERV_PORT 5555

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
        perror("fcntl(sock, GETFL)");
        exit(1);
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}

int main()
{
    int32_t i, listenfd, connfd, sockfd, epfd, nfds;
    int16_t readlen = 0;
    char line[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    //声明epoll_event结构体的变量, ev用于注册事件, events数组用于回传要处理的事件
    struct epoll_event ev, events[20];
    //生成用于处理accept的epoll专用的文件描述符, 指定生成描述符的最大范围为256

    epfd = epoll_create(256);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    

    setnonblocking(listenfd);
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(SERV_PORT);
    bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
        (const char*)&serveraddr, sizeof(serveraddr));

    //把用于监听的socket设置为非阻塞方式
    ev.data.fd = listenfd;          //设置与要处理的事件相关的文件描述符
    ev.events = EPOLLIN | EPOLLET;  //设置要处理的事件类型
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);     //注册epoll读事件

    listen(listenfd, LISTENQ);

    for ( ; ; ) {
        nfds = epoll_wait(epfd, events, 20, -1);    //等待epoll事件的发生
        for (i = 0; i < nfds; ++i) {                //处理所发生的所有事件
            if (events[i].data.fd == listenfd) {    //监听事件
                connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clilen);
                if (connfd < 0) {
                    perror("connfd<0");
                    exit(1);
                }
                
                //把客户端的socket设置为非阻塞方式
                setnonblocking(connfd);           
                char *ipstr = inet_ntoa(clientaddr.sin_addr);
                printf("connect from %s\n", ipstr);
                
                //注册ev事件
                ev.data.fd = connfd;              //设置用于读操作的文件描述符
                ev.events = EPOLLIN | EPOLLET;    //设置用于注测的读操作事件
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
                
            } else if (events[i].events & EPOLLIN) { //读事件
                if ( (sockfd = events[i].data.fd) < 0) {
                    continue;
                }
                if ( (readlen = read(sockfd, line, MAXLINE)) < 0) {
                    /* If errno == EAGAIN, that means we have read all
                             data. So go back to the main loop. */
                    if (errno != EAGAIN) {
                        close(sockfd);
                        events[i].data.fd = -1;
                        perror("read");
                    }
                } else if (readlen == 0) {
                    /* End of file. The remote has closed the
                             connection. */
                    close(sockfd);
                    events[i].data.fd = -1;
                }
                ev.data.fd = sockfd;            //设置用于写操作的文件描述符
                ev.events = EPOLLOUT | EPOLLET; //设置用于注测的写操作事件
                //修改sockfd上要处理的事件为EPOLLOUT
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            } else if (events[i].events & EPOLLOUT) { //写事件
                sockfd = events[i].data.fd;
                write(sockfd, line, readlen);
                ev.data.fd = sockfd;               //设置用于读操作的文件描述符
                ev.events = EPOLLIN | EPOLLET;     //设置用于注册的读操作事件
                //修改sockfd上要处理的事件为EPOLIN
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
        }
    }
}
