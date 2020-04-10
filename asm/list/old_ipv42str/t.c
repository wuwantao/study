#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/*
    asm volatile(
            "movq $0x3139322e3136382e, %%rax;"
            "movq %%rax, %[ip_str];"
            :[ip_str]"=m"(ip_str)::"rax");
*/

#define UINT8_MAX_SIZE 255
typedef struct uint8_str {
    uint8_t str[UINT8_MAX_SIZE][8];
} uint8_str_t;

#define ADD_UINT8_NUM(x)\
memcpy(d, uint8_str->str[x], uint8_str->str[x][7]);\
d += uint8_str->str[x][7];

#define ADD_IPV4_STR(x)\
ip1 = (uint8_t *)x;\
ip2 = ip1 + 1;\
ip3 = ip2 + 1;\
ip4 = ip3 + 1;\
ADD_UINT8_NUM(*ip1);\
*d = '.';\
d += 1;\
ADD_UINT8_NUM(*ip2);\
*d = '.';\
d += 1;\
ADD_UINT8_NUM(*ip3);\
*d = '.';\
d += 1;\
ADD_UINT8_NUM(*ip4);

int main()
{
    uint32_t ip = 0xc0a8a8a8;   //192.168.1.1
    uint8_t ip_str[64] = {0};
    uint64_t i = 0;
    uint8_t *p = (uint8_t *)&ip;
    uint8_t *d = (uint8_t *)ip_str;
    uint8_t *ip1, *ip2, *ip3, *ip4;
    uint8_str_t *uint8_str = malloc(sizeof(uint8_str_t));

    for (i = 0; i < UINT8_MAX_SIZE; i++) {
        sprintf(uint8_str->str[i], "%d", i);
        uint8_str->str[i][7] = strlen((char *)uint8_str->str[i]);
    }

    for (i = 0; i < 10000ULL *10000ULL; i++) {
        d = (uint8_t *)ip_str;
        ADD_IPV4_STR(p);
    }
    printf("%s\n", ip_str);
    return 0;

}
