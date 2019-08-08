#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT    8080

/* *
 * 模拟服务器进入 close_wait 状态
 * 客户端主动close(fd),服务端不执行close(fd)
 */
int main(int argc, char **argv)
{
    int fd = 0;
    int iRet = 0;
    struct sockaddr_in addr;
    char buf[1024] = {[0 ... sizeof(buf) - 1] = 0};

    fd =  socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == fd) {
        perror("socket");
        return errno;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&addr,
               sizeof(struct sockaddr_in));

    if (-1 == bind(fd, (struct sockaddr*)&addr, sizeof(addr))) {
        perror("bind");
        return errno;
    }

    if (-1 == listen(fd, 5)) {
        perror("listen");
        return errno;
    }

    struct sockaddr_in cli_addr;
    socklen_t len = sizeof(cli_addr);
    while (1) {
        int connfd = accept(fd, (struct sockaddr*)&cli_addr, &len);
        if (-1 == connfd) {
            perror("accept");
            return errno;
        }

//        printf("accept an client\n");
//
//        memset(buf, 0, 1024);
//        iRet = recv(connfd, buf, sizeof(buf), 0);
//        printf("recv content [%s]\n", buf);
//        printf("close client\n");
//        close(client);
    }
    close(fd);

    return 0;
}
