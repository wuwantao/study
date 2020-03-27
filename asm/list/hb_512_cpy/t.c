#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define CNT (10000ULL * 10000ULL * 5 * 5)
uint8_t *s = "012345678901234567890120123456789012345678901201234567890123456\n"; //64
uint8_t d[1024];

void test_64_cpy()
{
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

    asm volatile("movq 24(%rsi), %rax");
    asm volatile("movq %rax, 16(%rdi)");

    asm volatile("movq 32(%rsi), %rax");
    asm volatile("movq %rax, 16(%rdi)");

    asm volatile("movq 40(%rsi), %rax");
    asm volatile("movq %rax, 16(%rdi)");  

    asm volatile("movq 48(%rsi), %rax");
    asm volatile("movq %rax, 16(%rdi)"); 

    asm volatile("movq 56(%rsi), %rax");
    asm volatile("movq %rax, 16(%rdi)"); 

    asm volatile("movq 64(%rsi), %rax");
    asm volatile("movq %rax, 16(%rdi)");     
    
    asm volatile("pop %rdi");
    asm volatile("pop %rsi");
    asm volatile("pop %rax");
//	printf("%s: d:%s", __FUNCTION__, d);
}


void test_128_cpy()
{
    asm volatile("push %rax");
    asm volatile("push %rsi");
    asm volatile("push %rdi");
    asm volatile("movq %0, %%rsi"::"r"(s));
    asm volatile("movq %0, %%rdi"::"r"(d));
    
    asm volatile("movups (%rsi), %xmm0");
    asm volatile("movups %xmm0, (%rdi)");

    asm volatile("movups 16(%rsi), %xmm0");
    asm volatile("movups %xmm0, 16(%rdi)"); 

    asm volatile("movups 16(%rsi), %xmm0");
    asm volatile("movups %xmm0, 16(%rdi)");    

    asm volatile("movups 32(%rsi), %xmm0");
    asm volatile("movups %xmm0, 32(%rdi)");    

    asm volatile("movups 48(%rsi), %xmm0");
    asm volatile("movups %xmm0, 48(%rdi)");        
    
    asm volatile("pop %rdi");
    asm volatile("pop %rsi");
    asm volatile("pop %rax");
//	printf("%s: d:%s", __FUNCTION__, d);
}


void test_256_cpy()
{
    asm volatile("push %rax");
    asm volatile("push %rsi");
    asm volatile("push %rdi");
    asm volatile("movq %0, %%rsi"::"r"(s));
    asm volatile("movq %0, %%rdi"::"r"(d));
    
    asm volatile("vmovups (%rsi), %ymm0");
    asm volatile("vmovups %ymm0, (%rdi)");

    asm volatile("vmovups 32(%rsi), %ymm0");
    asm volatile("vmovups %ymm0, 32(%rdi)");    
    
    asm volatile("pop %rdi");
    asm volatile("pop %rsi");
    asm volatile("pop %rax");
//	printf("%s: d:%s", __FUNCTION__, d);
}


void test_512_cpy()
{
    asm volatile("push %rax");
    asm volatile("push %rsi");
    asm volatile("push %rdi");
    asm volatile("movq %0, %%rsi"::"r"(s));
    asm volatile("movq %0, %%rdi"::"r"(d));
    
    asm volatile("VMOVDQU64 (%rsi), %zmm0");
    asm volatile("VMOVDQU64 %zmm0, (%rdi)");
    
    asm volatile("pop %rdi");
    asm volatile("pop %rsi");
    asm volatile("pop %rax");
//	printf("%s: d:%s", __FUNCTION__, d);
}


#define ADD_TEST(x)\
start = clock();\
for (i = 0; i < CNT; i++) {\
    x();\
}\
finish = clock();\
duration = (double)(finish - start) / CLOCKS_PER_SEC;\
printf( #x": %f seconds\n", duration );


int main()
{
    uint64_t i = 0;
    clock_t start, finish;
    double duration;

    ADD_TEST(test_64_cpy);
    ADD_TEST(test_128_cpy);
    ADD_TEST(test_256_cpy);
    ADD_TEST(test_512_cpy);

    return 0;
    
}
