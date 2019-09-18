#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

truct in6_addr {
union {
    __u8        u6_addr8[16];
    __be16      u6_addr16[8];
    __be32      u6_addr32[4];
} in6_u;
#define s6_addr         in6_u.u6_addr8
#define s6_addr16       in6_u.u6_addr16
#define s6_addr32       in6_u.u6_addr32
};
struct in_addr {
    __be32  s_addr;
};
*/
int
main(int argc, char *argv[])
{
    unsigned char buf[sizeof(struct in6_addr)];
    int domain, s;
    char str[INET6_ADDRSTRLEN];
    struct in_addr *p = NULL;
    p = (struct in_addr*) buf;

    /*
        if (argc != 3) {
            fprintf(stderr, "Usage: %s {i4|i6|<num>} string\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        domain = (strcmp(argv[1], "i4") == 0) ? AF_INET :
                 (strcmp(argv[1], "i6") == 0) ? AF_INET6 : atoi(argv[1]);

        s = inet_pton(domain, argv[2], buf);
        */
    domain = AF_INET;
    s = inet_pton(domain, "192.168.1.2", buf);
    if (s <= 0) {
        if (s == 0)
            fprintf(stderr, "Not in presentation format");
        else
            perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (inet_ntop(domain, buf, str, INET_ADDRSTRLEN) == NULL) {
        perror("xxinet_ntop");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", str);

    exit(EXIT_SUCCESS);
}

