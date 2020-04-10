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

typedef struct uint8_str {
    uint8_t str[7];
    uint8_t len;
} uint8_str_t;
uint8_str_t *uint8_str = NULL;

int main()
{
    uint32_t ip = 0xc0a80101;   //192.168.1.1
    char ip_str[64] = {0};
    uint8_t i = 0;
    uint8_str_t *p = NULL;

    uint8_str = (uint8_str_t*) malloc(sizeof(uint8_str_t) * 255);

    for (i = 0; i < 255; i++) {
        p = uint8_str + i * sizeof(uint8_str_t);
        sprintf(p->str, "%d", i);
        p->len = strlen(p->str);
    }


    uint8_t t = 0;
    uint8_t t1 = 0;
    asm volatile(
        "movw $0xc0, %%ax;"
        "movb $100, %%cl;"
        "div %%cl;"
        "movb %%al, %0;"
        "movb %%ah, %1;"
        :"=r"(t),"=r"(t1)::"ax");

    printf("t:%d\n", t);
    printf("t1:%d\n", t1);

}
