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


void test_loop()
{
    uint64_t i = 0;
    asm ("push %rax");
    asm ("push %rcx");
    asm ("xor %rax, %rax");
    asm ("xor %rcx, %rcx");
    asm ("mov $0x5, %rcx");
    asm ("s:");
    asm ("add $0x2, %rax");
    asm ("loop s");
    asm ("mov %%rax, %0":"=r"(i));
    asm ("pop %rcx");
    asm ("pop %rax");
    printf("i:%d\n", i);

}

void test_one_byte_cpy1()
{
    char *src = "hello world!\n";
    char *dst = malloc(128);
    uint64_t i = 0;
    
    asm volatile("push %rsi");
    asm volatile("push %rdi");
    asm volatile("push %rcx");
    asm volatile("movq %0, %%rsi"::"r"(src));
    asm volatile("movq %0, %%rdi"::"r"(dst));
    asm volatile("mov $14, %rcx");
    asm volatile("cld");
    asm volatile("rep movsd");
    asm volatile("pop %rcx");
    asm volatile("pop %rdi");
    asm volatile("pop %rsi");
    printf("dst:%s\n", dst);
}


void test_one_byte_cpy2()
{
    uint8_t *src = "hello world!\n";
    uint8_t dst[128];
    uint64_t i = 0;
    
    asm volatile("push %rsi");
    asm volatile("push %rdi");
    asm volatile("push %rcx");
    asm volatile("movq %0, %%rsi"::"r"(src));
    asm volatile("movq %0, %%rdi"::"r"(dst));
    asm volatile("mov $14, %rcx");
    asm volatile("cld");
    asm volatile("rep movsd");
    asm volatile("pop %rcx");
    asm volatile("pop %rdi");
    asm volatile("pop %rsi");
    printf("dst:%s\n", dst);
}


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
