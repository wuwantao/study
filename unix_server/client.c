#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/un.h>
#include <errno.h>


#define CLI_PATHNAME "cli.unix.dg"
#define SRV_PATHNAME "srv.unix.dg"

#define PORT 7777

#define MAXLINE 2048
#define LISTENQ 20

//int main()
//{
//    int32_t i = 0, fd = 0, sockfd = 0, epfd = 0, nfds = 0;
//    int ret = 0;
//    int16_t readlen = 0;
//    char line[MAXLINE] = "hello world!";
//    char line2[MAXLINE];
//
//    struct sockaddr_un serveraddr, cliaddr;
//    socklen_t len = sizeof(cliaddr);
//    struct epoll_event ev, events[20];                              //声明epoll_event结构体的变量, ev用于注册事件, events数组用于回传要处理的事件
//
//    epfd = epoll_create(256);                                       //生成用于处理accept的epoll专用的文件描述符, 指定生成描述符的最大范围为256
//    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

//    memset(&cliaddr, 0, sizeof(cliaddr));
//    cliaddr.sun_family = AF_UNIX;
//    strcpy(cliaddr.sun_path, tmpnam(NULL));
//    ret =  bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
//    if (ret == -1) {
//        printf ("ensure the server is up\n");
//        perror ("bind");
//        exit (EXIT_FAILURE);
//    }

//    memset(&serveraddr, 0, sizeof(serveraddr));
//    serveraddr.sun_family = AF_UNIX;
//    strcpy(serveraddr.sun_path, PATHNAME);

//    ret = connect(sockfd, (struct sockaddr *)&serveraddr, len);
//    if (ret == -1) {
//        printf ("ensure the server is up\n");
//        perror ("connect");
//        exit (EXIT_FAILURE);
//    }
//

//    ev.data.fd = sockfd;                                          //设置与要处理的事件相关的文件描述符
//    ev.events = EPOLLOUT|EPOLLIN;                                           //设置要处理的事件类型
//    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);            //注册epoll读事件
//
//    for (;;) {
//        nfds = epoll_wait(epfd, events, 20, -1);                    //等待epoll事件的发生
//
//        for (i = 0; i < nfds; ++i) {                                //处理所发生的所有事件
//            if (events[i].events & EPOLLOUT) {
//                if ( (fd = events[i].data.fd) < 0) {
//                    printf("error:sockfd:%d\n", fd);
//                    continue;
//                }

//                write(fd, line, strlen(line));
//                sendto(fd, line, strlen(line), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
//                printf("write success\n");
//                sleep(1);
//            } else if (events[i].events & EPOLLIN) {
//                printf("EPOLLIN\n");
//                readlen = recvfrom(sockfd, line2, sizeof(line2), 0, (struct sockaddr *)&cliaddr, &len);
//                readlen = read(sockfd, line2, sizeof(line2));
//                if (readlen > 0) {
//                    printf("recv:%s\n", line2);
//                }
//            }
//        }
//    }

//    return -1;
//}

#if 0
int main()
{
    int32_t sockfd = 0;
    int ret = 0;
    int16_t readlen = 0;
    char send_buf[MAXLINE] = "hello world!";
    char recv_buf[MAXLINE];
    
    struct sockaddr_un srvaddr;
    struct sockaddr_un cliaddr;
    socklen_t srv_len = sizeof(srvaddr);
    
    
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sun_family = AF_UNIX;
    strcpy(srvaddr.sun_path, SRV_PATHNAME);
    
    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sun_family = AF_UNIX;
    strcpy(cliaddr.sun_path, CLI_PATHNAME);
    
    unlink(CLI_PATHNAME);
    
    if (bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0) {
        perror("bind failed.");
        exit(-1);
    }
    
    
    for (;;) {
        sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&srvaddr, sizeof(srvaddr));
        
        printf("write success\n");
        sleep(1);
        readlen = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&srvaddr, &srv_len);
        printf("recv:%s\n", recv_buf);
    }
    
    return -1;
}
#else
#if 0
int main()
{
    int sockfd = 0;
    int fd = 0;
    int epfd = 0;
    int nfds = 0;
    int readlen = 0;
    int i = 0;
    int ret = 0;
    char send_buf[MAXLINE] = "hello world!";
    char recv_buf[MAXLINE] = {0};
    struct epoll_event ev, events[20];
    
    struct sockaddr_un srvaddr;
    struct sockaddr_un cliaddr;
    socklen_t len = sizeof(srvaddr);
    
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sun_family = AF_UNIX;
    strcpy(srvaddr.sun_path, SRV_PATHNAME);
    
    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sun_family = AF_UNIX;
    strcpy(cliaddr.sun_path, CLI_PATHNAME);
    
    unlink(CLI_PATHNAME);
    
    if (bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0) {
        perror("bind failed.");
        exit(-1);
    }
    
    ev.data.fd = sockfd;
    ev.events = EPOLLOUT | EPOLLIN;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    
    
    for (;;) {
        nfds = epoll_wait(epfd, events, 20, -1);
        
        for (i = 0; i < nfds; ++i) {
            if (events[i].events & EPOLLOUT) {
                if ( (fd = events[i].data.fd) < 0) {
                    printf("error:sockfd:%d\n", fd);
                    continue;
                }
                
                sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&srvaddr, sizeof(srvaddr));
                
            } else if (events[i].events & EPOLLIN) {
                if ( (fd = events[i].data.fd) < 0) {
                    printf("error:sockfd:%d\n", fd);
                    continue;
                }
                
                readlen = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&cliaddr, &len);
                
                if (readlen < 0) {
                    printf("error:readlen:%d\n", readlen);
                    
                    if (errno != ECONNRESET) {
                        close(fd);
                        events[i].data.fd = -1;
                        perror("read");
                    }
                } else if (readlen == 0) {
                    printf("error:readline:%d\n", readlen);
                    close(fd);
                    events[i].data.fd = -1;
                }
                
                printf("readlen:%d line:%s\n");
            }
        }
    }
    
    
    return -1;
}
#else

#define TIMEOUT 10*1000

int main()
{
    int32_t i = 0;
    int32_t sockfd = 0;
    int32_t epfd = 0;
    int32_t nfds = 0;
    int32_t fd = 0;
    int ret = 0;
    int16_t readlen = 0;
    char send_buf[MAXLINE] = "hello world!";
    char recv_buf[MAXLINE] = {0};
    socklen_t clilen;
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    struct epoll_event ev, events[20];
    
    epfd = epoll_create(256);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    ev.data.fd = sockfd;
    ev.events = EPOLLOUT;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);
    
    if (connect(sockfd, (struct sockaddr *)(&serveraddr), sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "Connect error:%s\n", strerror(errno));
        exit(1);
    }
    
    for (;;) {
        nfds = epoll_wait(epfd, events, 20, TIMEOUT);
        
        if (nfds < 0) {
            perror("epoll_wait");
            return 1;
        }
        
        if (nfds == 0) {
            printf("recv timeout\n");
            break;
        }
        
        for (i = 0; i < nfds; ++i) {
            if (events[i].events & EPOLLOUT) {
                if ( (fd = events[i].data.fd) < 0) {
                    printf("error:sockfd:%d\n", fd);
                    continue;
                }
                
                sendto(fd, send_buf, sizeof(send_buf), 0, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in));
                ev.data.fd = fd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
            } else if (events[i].events & EPOLLIN) {
                readlen = recvfrom(fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&serveraddr, &clilen);
                
                if (readlen < 0) {
                    printf("error:readlen:%d\n", readlen);
                    
                    /* If errno == EAGAIN, that means we have read all
                             data. So go back to the main loop. */
                    if (errno != ECONNRESET) {
                        close(fd);
                        events[i].data.fd = -1;
                        perror("read");
                    }
                } else if (readlen == 0) {
                    printf("error:readline:%d\n", readlen);
                    /* End of file. The remote has closed the
                             connection. */
                    close(fd);
                    events[i].data.fd = -1;
                }
                
                printf("recv:%s\n", recv_buf);
                ev.data.fd = fd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
                break;
            }
        }
    }
}

#endif


#endif
