#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct test {
    uint8_t  msg_type:7;  //SYNC_FINISH or LEASE_UPDATE
    uint16_t  ack:1;       //回复ACK 标记
} test_t;
typedef struct bit{
    char x:2;
    int y:6;
    long long z:10;
}bit_t;
int main()
{

    printf("%d\n", sizeof(test_t));
    printf("%d\n", sizeof(bit_t));
    return 0;
}
