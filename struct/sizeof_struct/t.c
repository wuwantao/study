#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct x {
    int a;
    int b;
    int b1;
    int b2;
    int b3;
    int b4;
};

int main()
{
    struct x *bb = NULL;

    printf("%d\n", sizeof(*bb));
    printf("%d\n", sizeof(struct x));
    printf("%d\n", sizeof(bb));
    return 0;
}
//24
//24
//8
