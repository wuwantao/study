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
#include <stdbool.h>
#include <pthread.h>
#include <linux/if.h>

#define MAXLINE 1024
#define LISTENQ 20
#define SERV_PORT 53


long long unsigned read_pps = 0;
long long unsigned write_pps = 0;


void *get_pps_loop(void *arg)
{
    time_t old_time;
    time_t new_time;
    uint64_t new_read_cnt = 0;
    uint64_t old_read_cnt = 0;


    uint64_t new_write_cnt = 0;
    uint64_t old_write_cnt = 0;

    sleep(2);
    old_read_cnt = read_pps;
    old_write_cnt = write_pps;
    old_time = time(NULL);
    
    for(;;) {
        sleep(2);
        new_time = time(NULL);
        new_read_cnt = read_pps;
        new_write_cnt = write_pps;
        long long unsigned sec = ((uint64_t)(new_time - old_time));
        printf("read_pps:%ld\twrite_pps:%ld\n",
            (new_read_cnt - old_read_cnt)/sec,
            (new_write_cnt - old_write_cnt)/sec);
        old_read_cnt = new_read_cnt;
        old_write_cnt = new_write_cnt;
        old_time = new_time;
    }
}

bool create_time_thread()
{
    uint32_t ret = 0;
    pthread_t pid;
    
    ret = pthread_create(&pid, NULL, get_pps_loop, NULL);
    
    if (ret < 0) {
        printf("创建获取实时时间线程失败\n");
        return false;
    }  
    pthread_detach(pid);
    return true;
}


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
    int ret;
    int16_t readlen = 0;
    char line[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    
    struct epoll_event ev, events[20];      //声明epoll_event结构体的变量, ev用于注册事件, events数组用于回传要处理的事件
    char datapacket[100] = {[0 ... 99] = 0x11};
    

    epfd = epoll_create(256);                           //生成用于处理accept的epoll专用的文件描述符, 指定生成描述符的最大范围为256
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    setnonblocking(listenfd);                           //把用于监听的socket设置为非阻塞方式
    printf("listenfd:%d\n", listenfd);

    ev.data.fd = listenfd;                              //设置与要处理的事件相关的文件描述符
    ev.events = EPOLLIN | EPOLLET;                      //设置要处理的事件类型
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);//注册epoll读事件
    printf("epoll_ctl return %d\n",ret);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(SERV_PORT);
    bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&serveraddr, sizeof(serveraddr));

    listen(listenfd, LISTENQ);
    create_time_thread();

    for ( ; ; ) {
        nfds = epoll_wait(epfd, events, 20, -1);    //等待epoll事件的发生
        for (i = 0; i < nfds; ++i) {                //处理所发生的所有事件
            if (events[i].events & EPOLLIN) {       //读事件
                if ( (sockfd = events[i].data.fd) < 0) {
                    printf("error:sockfd:%d\n", sockfd);
                    continue;
                }
                
                readlen = recvfrom(sockfd, line, MAXLINE, 0, (struct sockaddr *)&clientaddr, &clilen);
                if (readlen < 0) {
                    printf("error:readlen:%d\n", readlen);
                    /* If errno == EAGAIN, that means we have read all
                             data. So go back to the main loop. */
                    if (errno != ECONNRESET) {
                        close(sockfd);
                        events[i].data.fd = -1;
                        perror("read");
                    }
                } else if (readlen == 0) {
                    printf("error:readline:%d\n", readlen);
                    /* End of file. The remote has closed the
                             connection. */
                    close(sockfd);
                    events[i].data.fd = -1;
                }
                read_pps++;
                ev.data.fd = sockfd;            //设置用于写操作的文件描述符
                ev.events = EPOLLOUT | EPOLLET; //设置用于注测的写操作事件
                //修改sockfd上要处理的事件为EPOLLOUT
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            } else if (events[i].events & EPOLLOUT) { //写事件
                write_pps++;
                sockfd = events[i].data.fd;
                sendto(sockfd, datapacket, sizeof(datapacket), 0, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in));
                ev.data.fd = sockfd;               //设置用于读操作的文件描述符
                ev.events = EPOLLIN | EPOLLET;     //设置用于注册的读操作事件
                //修改sockfd上要处理的事件为EPOLIN
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
        }
    }
}
