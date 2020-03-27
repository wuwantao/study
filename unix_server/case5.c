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

#define SRV_PATHNAME "srv.unix.dg"
#define CLI_PATHNAME "cli.unix.dg"

#define PORT 7777

#define MAXLINE 2048
#define LISTENQ 20

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
#if 0
int main()
{
    int32_t i = 0, fd = 0, sockfd = 0, epfd = 0, nfds = 0;
    int ret = 0;
    int16_t readlen = 0;
    char line[MAXLINE] = {0};
    char line2[MAXLINE] = "ok";
    
    struct sockaddr_un serveraddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    struct epoll_event ev, events[20];                              //声明epoll_event结构体的变量, ev用于注册事件, events数组用于回传要处理的事件
    
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, PATHNAME);
    
    unlink(PATHNAME);
    
    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("bind failed.");
        exit(-1);
    }
    
    
    while (1) {
    
        readlen = read(sockfd, line, MAXLINE);
//        readlen = recvfrom(sockfd, line, sizeof(line), 0, (struct sockaddr *)&cliaddr, &len);

        if (readlen < 0) {
            perror("readlen < 0");
            exit(-1);
        }
        
        printf("readlen:%d line:%s\n", readlen, line);
        printf("cliaddr:%s\n", cliaddr.sun_path);
        write(sockfd, line2, strlen(line2));
//        sendto(sockfd, line2, strlen(line2), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    }
    
    
    return -1;
}
#else
#if 0
int main()
{
    int32_t sockfd = 0;
    int16_t readlen = 0;
    char recv_buf[MAXLINE] = {0};
    char send_buf[MAXLINE] = "ok";
    
    struct sockaddr_un srvaddr;
    socklen_t len = sizeof(srvaddr);
    
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sun_family = AF_UNIX;
    strcpy(srvaddr.sun_path, PATHNAME);
    
    unlink(PATHNAME);
    
    if (bind(sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0) {
        perror("bind failed.");
        exit(-1);
    }
    
    
    while (1) {
        readlen = read(sockfd, recv_buf, MAXLINE);
        
        if (readlen < 0) {
            perror("readlen < 0");
            exit(-1);
        }
        
        printf("readlen:%d line:%s\n", readlen, recv_buf);
        write(sockfd, send_buf, strlen(send_buf));
    }
    
    
    return -1;
}
#else

#if 0
int main()
{
    int32_t sockfd = 0;
    int16_t readlen = 0;
    char recv_buf[MAXLINE] = {0};
    char send_buf[MAXLINE] = "ok";
    
    struct sockaddr_un srvaddr;
    struct sockaddr_un cliaddr;
    socklen_t len = sizeof(srvaddr);
    
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sun_family = AF_UNIX;
    strcpy(srvaddr.sun_path, SRV_PATHNAME);
    
    unlink(SRV_PATHNAME);
    
    if (bind(sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0) {
        perror("bind failed.");
        exit(-1);
    }
    
    
    while (1) {
        readlen = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&cliaddr, &len);
        
        if (readlen < 0) {
            perror("readlen < 0");
            exit(-1);
        }
        
        printf("readlen:%d line:%s\n", readlen, recv_buf);
        sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
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
    char recv_buf[MAXLINE] = {0};
    char send_buf[MAXLINE] = "ok";
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
    
    unlink(SRV_PATHNAME);
    
    if (bind(sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0) {
        perror("bind failed.");
        exit(-1);
    }
    
    ev.data.fd = sockfd;
    ev.events = EPOLLIN;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    
    
    for (;;) {
        nfds = epoll_wait(epfd, events, 20, -1);
        
        for (i = 0; i < nfds; ++i) {
            if (events[i].events & EPOLLIN) {
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
                sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
            }
        }
    }
    
    
    return -1;
}
#else

#define ACL_IPV6_ENTRY_NUM_MAX     16
#define ACL_IP_ENTRY_NUM_MAX        16
#define ACL_IPV6_ACT_NUM_MAX        16
#define ACL_IP_ACT_NUM_MAX             16
#define IP_MAX_LEN          16	// IPv6 address is 16 bytes

typedef struct dns_acl_field_s { 
    uint8_t acl_field_ipsa[IP_MAX_LEN];
    uint8_t acl_field_ipsa_prefix;
    uint8_t acl_field_ipda[IP_MAX_LEN];
    uint8_t acl_field_ipda_prefix;
    uint8_t acl_field_ipproto;
    uint8_t acl_field_ipprotomsk;
    uint16_t acl_field_l4sport;
    uint16_t acl_field_l4sportmsk;
    uint16_t acl_field_l4dport;
    uint16_t acl_field_l4dportmsk;
} __attribute__((packed)) dns_acl_field_t;

typedef struct dns_acl_key_s { 
    dns_acl_field_t acl_field;
} __attribute__((packed)) dns_acl_key_t;

typedef struct dns_acl_act_s {
    uint8_t dns_acl_act_drop_en;
    uint8_t dns_acl_act_count_en;
} dns_acl_act_t;

typedef struct dns_acl_stats_s {
    uint64_t dns_acl_count_num;
} __attribute__((packed)) dns_acl_stats_t;


enum dns_sdk_cmd {
    cmd_drv_reg_read_e ,
    cmd_drv_reg_write_e,
    cmd_drv_io_read_e  ,
    cmd_drv_io_write_e ,
    cmd_drv_dma_write_e,

    cmd_acl_entry_add_e ,
    cmd_acl_entry_del_e ,
    cmd_acl_entry_mod_e ,
    cmd_acl_entry_get_e ,
    cmd_acl_action_get_e,
    cmd_acl_action_set_e,
    cmd_acl_stats_get_e ,

    cmd_func_max_e
};

struct cmd_hdr {
    uint16_t  seq;    // sequence number
    int16_t   cmd;    // command id
    uint16_t  len;    // length of data
    int16_t   rv;     // return value
} __attribute__((packed));
typedef struct cmd_hdr cmd_hdr_t;

typedef struct {
    cmd_hdr_t cmd_hdr;
    dns_acl_key_t dns_acl_key;
    dns_acl_act_t dns_acl_act;
    dns_acl_stats_t dns_acl_stats;
} cmd_t;

int main()
{
    int32_t i, listenfd, connfd, sockfd, epfd, nfds;
    uint32_t j = 0;
    uint32_t cnt = 0;
    int ret;
    int16_t readlen = 0;
    char send_buf[MAXLINE] = "ok";
    uint8_t recv_buf[MAXLINE] = {0};
    socklen_t clilen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    
    struct epoll_event ev, events[20];
    char datapacket[100] = {[0 ... 99] = 0x11};
    
    
    epfd = epoll_create(256);
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    setnonblocking(listenfd);
    printf("listenfd:%d\n", listenfd);
    
    ev.data.fd = listenfd;
    ev.events = EPOLLIN;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    printf("epoll_ctl return %d\n", ret);
    
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);
    bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&serveraddr, sizeof(serveraddr));
    
    listen(listenfd, LISTENQ);
    
    for ( ; ; ) {
        nfds = epoll_wait(epfd, events, 20, -1);
        
        for (i = 0; i < nfds; ++i) {
            if (events[i].events & EPOLLIN) {
                if ( (sockfd = events[i].data.fd) < 0) {
                    printf("error:sockfd:%d\n", sockfd);
                    continue;
                }
                
                readlen = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&clientaddr, (socklen_t*)&clilen);
                
                if (readlen < 0) {
                    printf("error:readlen:%d\n", readlen);
                    
                    if (errno != ECONNRESET) {
                        close(sockfd);
                        events[i].data.fd = -1;
                        perror("read");
                    }
                    
                    exit(-1);
                } else if (readlen == 0) {
                    printf("error:readline:%d\n", readlen);
                    close(sockfd);
                    events[i].data.fd = -1;
                    exit(-1);
                }
                printf("\n\n");
                for (j = 0; j < readlen; j++) {
                    printf("%02x ", recv_buf[j]);
                    if ((j + 1)%8 == 0) {
                        printf("\n");
                    }
                }
                printf("\n\n");
                cmd_t *cmd = (cmd_t *)&recv_buf;

                char ipsa_str[128] = {0};
                char ipda_str[128] = {0};
                if (inet_ntop(AF_INET, (void*)cmd->dns_acl_key.acl_field.acl_field_ipsa, ipsa_str, sizeof(ipsa_str)) == NULL) {
                    inet_ntop(AF_INET6, (void*)cmd->dns_acl_key.acl_field.acl_field_ipsa, ipsa_str, sizeof(ipsa_str));
                }

                if (inet_ntop(AF_INET, (void*)cmd->dns_acl_key.acl_field.acl_field_ipda, ipda_str, sizeof(ipda_str)) == NULL) {
                    inet_ntop(AF_INET6, (void*)cmd->dns_acl_key.acl_field.acl_field_ipda, ipda_str, sizeof(ipda_str));
                }                
            
                printf("seq:%d, cmd:%d, len:%d, rev:%d\n"
                    "ipsa:%s\n"
                    "ipsaprefix:%d\n"
                    "ipda:%s\n"
                    "ipdaprefix:%d\n"
                    "ipproto:%d\n"
                    "ipprotomsk:%d\n"
                    "sport:%d\n"
                    "sportmsk:%d\n"
                    "dport:%d\n"
                    "dportmsk:%d\n"
                    "drop:%d\n"
                    "stat:%d\n",
                    cmd->cmd_hdr.seq, cmd->cmd_hdr.cmd, cmd->cmd_hdr.len, cmd->cmd_hdr.rv,
                    ipsa_str,
                    cmd->dns_acl_key.acl_field.acl_field_ipsa_prefix,
                    ipda_str,
                    cmd->dns_acl_key.acl_field.acl_field_ipda_prefix,
                    cmd->dns_acl_key.acl_field.acl_field_ipproto,
                    cmd->dns_acl_key.acl_field.acl_field_ipprotomsk,
                    cmd->dns_acl_key.acl_field.acl_field_l4sport,
                    cmd->dns_acl_key.acl_field.acl_field_l4sportmsk,
                    cmd->dns_acl_key.acl_field.acl_field_l4dport,
                    cmd->dns_acl_key.acl_field.acl_field_l4dportmsk,
                    cmd->dns_acl_act.dns_acl_act_drop_en,
                    cmd->dns_acl_act.dns_acl_act_count_en
                    );
                
                printf("\n\n");

                cnt++;
                if (cnt > 14) {
                    exit(0);
                }                
                
                if (sendto(sockfd, send_buf, sizeof(send_buf), 0, (struct sockaddr *)&clientaddr, clilen) < 0) {
                    perror("send failed.");
                }
                
            }
        }
    }
}

#endif

#endif

#endif

#endif
//int main()
//{
//    int32_t i = 0, fd = 0, sockfd = 0, epfd = 0, nfds = 0;
//    int ret = 0;
//    int16_t readlen = 0;
//    char line[MAXLINE] = {0};
//
//    struct sockaddr_un serveraddr,cliaddr;
//    socklen_t len = sizeof(cliaddr);
//    struct epoll_event ev, events[20];                              //声明epoll_event结构体的变量, ev用于注册事件, events数组用于回传要处理的事件
//
//    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
//
//    memset(&serveraddr, 0, sizeof(serveraddr));
//    serveraddr.sun_family = AF_UNIX;
//    strcpy(serveraddr.sun_path, PATHNAME);
//
//    unlink(PATHNAME);

//    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
//        perror("bind failed.");
//        exit(-1);
//    }
//
//    ev.data.fd = sockfd;
//    ev.events = EPOLLIN;
//    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

//
//    for (;;) {
//        nfds = epoll_wait(epfd, events, 20, -1);                    //等待epoll事件的发生
//
//        for (i = 0; i < nfds; ++i) {                                //处理所发生的所有事件
//            if (events[i].events & EPOLLIN) {                       //读事件
//                if ( (fd = events[i].data.fd) < 0) {
//                    printf("error:sockfd:%d\n", fd);
//                    continue;
//                }
//
//                readlen = read(fd, line, MAXLINE);
//
//                if (readlen < 0) {
//                    printf("error:readlen:%d\n", readlen);
//
//                    if (errno != ECONNRESET) {                      /* If errno == EAGAIN, that means we have read all data. So go back to the main loop. */
//                        close(fd);
//                        events[i].data.fd = -1;
//                        perror("read");
//                    }
//                } else if (readlen == 0) {
//                    printf("error:readline:%d\n", readlen);
//                    close(fd);                                  /* End of file. The remote has closed the connection. */
//                    events[i].data.fd = -1;
//                }
//                printf("readlen:%d line:%s\n");
//            }
//        }
//    }



//    return -1;
//}

