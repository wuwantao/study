#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

void test_one_byte_cpy()
{
    uint8_t *s = "01234567890123456789012\n"; //24
    uint8_t d[128] = {0};
    
    asm volatile("push %rsi");
    asm volatile("push %rdi");
    asm volatile("push %rcx");
    asm volatile("movq %0, %%rsi"::"r"(s));
    asm volatile("movq %0, %%rdi"::"r"(d));
    asm volatile("mov $14, %rcx");
    asm volatile("cld");
    asm volatile("rep movsd");
    asm volatile("pop %rcx");
    asm volatile("pop %rdi");
    asm volatile("pop %rsi");
//	printf("%s: d:%s", __FUNCTION__, d);
}

void test_four_byte_cpy()
{
    uint8_t *s = "01234567890123456789012\n"; //24
    uint8_t d[128] = {0};
    
    asm volatile("push %rax");
    asm volatile("push %rsi");
    asm volatile("push %rdi");
    asm volatile("movq %0, %%rsi"::"r"(s));
    asm volatile("movq %0, %%rdi"::"r"(d));
    
    asm volatile("movl (%rsi), %eax");
    asm volatile("movl %eax, (%rdi)");
    
    asm volatile("movl 4(%rsi), %eax");
    asm volatile("movl %eax, 4(%rdi)");
    
    asm volatile("movl 8(%rsi), %eax");
    asm volatile("movl %eax, 8(%rdi)");
    
    asm volatile("movl 12(%rsi), %eax");
    asm volatile("movl %eax, 12(%rdi)");
    
    asm volatile("movl 16(%rsi), %eax");
    asm volatile("movl %eax, 16(%rdi)");
    
    asm volatile("movl 20(%rsi), %eax");
    asm volatile("movl %eax, 20(%rdi)");
    
    asm volatile("pop %rdi");
    asm volatile("pop %rsi");
    asm volatile("pop %rax");
//	printf("%s: d:%s", __FUNCTION__, d);
}



void test_eight_byte_cpy()
{
    uint8_t *s = "01234567890123456789012\n"; //24
    uint8_t d[128] = {0};
    uint64_t i = 0;
    
    asm volatile("push %rax");
    asm volatile("push %rsi");
    asm volatile("push %rdi");
    asm volatile("movq %0, %%rsi"::"r"(s));
    asm volatile("movq %0, %%rdi"::"r"(d));
    
    asm volatile("movq (%rsi), %rax");
    asm volatile("movq %rax, (%rdi)");
    
    asm volatile("movq 8(%rsi), %rax");
    asm volatile("movq %rax, 8(%rdi)");
    
    asm volatile("movq 16(%rsi), %rax");
    asm volatile("movq %rax, 16(%rdi)");
    
    asm volatile("pop %rdi");
    asm volatile("pop %rsi");
    asm volatile("pop %rax");
//	printf("%s: d:%s", __FUNCTION__, d);
}

#define CNT (10000ULL * 10000ULL * 5 * 5)

#define UINT16_MAX_SIZE 65536
char *uint16_str = NULL;

#define ADD_PORT_NUM(x)\
    memcpy(d, uint16_str + x*8, *(uint16_str + x*8 + 7));\
    d += *(uint16_str + x*8 + 7);


void init_str()
{
    uint32_t i = 0;
    uint16_str = malloc(UINT16_MAX_SIZE * 8);
    if (uint16_str == NULL) {
        printf("uint16_str malloc failed.\n");
    }
    memset(uint16_str, 0, UINT16_MAX_SIZE * 8);
    
    for (i = 0; i < UINT16_MAX_SIZE; i++) {
        sprintf(uint16_str + i * 8, "%d", i);
        *(uint16_str + i * 8 + 7) = strlen((uint16_str + i * 8));
    }    
}

void test_c_add_port_num(uint16_t i)
{
    char str[128] = {0};
    char *d = str;
    ADD_PORT_NUM(i);
//    printf("%s\tstr:%s\n", __FUNCTION__, str);
}

void test_hb_add_port_num(uint16_t i)
{
    char str[128] = {0};
    char *d = str;

    asm volatile("push %rax");
    asm volatile("push %rsi");
    asm volatile("push %rdi");
    asm volatile("movq %0, %%rsi"::"r"(uint16_str + i * 8));
    asm volatile("movq %0, %%rdi"::"r"(d));
    
    asm volatile("movq (%rsi), %rax");
    asm volatile("movq %rax, (%rdi)");
    
    asm volatile("pop %rdi");
    asm volatile("pop %rsi");
    asm volatile("pop %rax");
    d += *(uint16_str + i*8 + 7);

//    printf("%s\tstr:%s\n", __FUNCTION__, str);
}


int main()
{
    uint64_t i = 0;
    clock_t start, finish;
    double duration;
    uint16_t port = 0;
    char str[128] = {0};
    char *d = str;    

    init_str();

//    start = clock();
//    
//    for (i = 0; i < CNT; i++) {
//        memcpy(str, uint16_str + port * 8, *(uint16_str + port*8 + 7));\
//        d += *(uint16_str + port*8 + 7);
//        port++;
//    }
//    
//    finish = clock();
//    duration = (double)(finish - start) / CLOCKS_PER_SEC;
//    printf( " test_c_add_port_num: %f seconds\n", duration );


//        asm volatile("push %rax");
//        asm volatile("push %rbx");
//        asm volatile("push %rcx");
//        asm volatile(""::"b"(uint16_str + port * 8));
//        //d += *(uint16_str + port*8 + 7);  
//        asm volatile("movq %rbx, %rax");
//        asm volatile("add $7, %rax");
//        asm volatile("movzxb (%rax), %eax");
//        asm volatile("addq %%rax, %0"::"m"(d));


//        asm volatile(""::"c"(str));

//        asm volatile("movq (%rbx), %rax");
//        asm volatile("movq %rax, (%rcx)");
//        asm volatile("pop %rcx");
//        asm volatile("pop %rbx");
//        asm volatile("pop %rax");
//        printf("str:%s\n", str);


    port = 0;
    d = str;
    start = clock();
    for (i = 0; i < CNT; i++) {
        asm volatile("push %rax");
        asm volatile("push %rsi");
        asm volatile("push %rdi");
        asm volatile("movq %0, %%rsi"::"r"(uint16_str + port * 8));
/*      d += *(uint16_str + port*8 + 7);  */
        asm volatile("movq %0, %%rdi"::"r"(d));
        asm volatile("movq %rsi, %rax");
        asm volatile("add $7, %rax");
        asm volatile("movzxb (%rax), %eax");
/*      asm volatile("movsx %al, %rax"); */
       asm volatile("addq %%rax, %0"::"m"(d));   /*实际使用*/
/*        asm volatile("addq $0, %0"::"m"(d)); */ /*测试用*/

        asm volatile("movq (%rsi), %rax");
        asm volatile("movq %rax, (%rdi)");
        asm volatile("pop %rdi");
        asm volatile("pop %rsi");
        asm volatile("pop %rax");
//        printf("str:%s\n", str);
        
        port++;
    }
    
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "test_hb_add_port_num: %f seconds\n", duration );


    return 0;
    start = clock();
    
    for (i = 0; i < CNT; i++) {
        test_one_byte_cpy();
    }
    
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "  one: %f seconds\n", duration );
    
    start = clock();
    
    for (i = 0; i < CNT; i++) {
        test_four_byte_cpy();
    }
    
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( " four: %f seconds\n", duration );
    
    start = clock();
    
    for (i = 0; i < CNT; i++) {
        test_eight_byte_cpy();
    }
    
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "eight: %f seconds\n", duration );
    
    return 0;
    
}
