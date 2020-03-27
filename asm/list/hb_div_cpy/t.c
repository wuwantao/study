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
void test_div()
{
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

int main()
{
	uint32_t ip = 0xc0a80201;   //192.168.2.1
	char ip_str[64] = {0};
	uint64_t i = 0;


	for (i = 0; i < 10000ULL * 10000ULL; i++) {

		asm volatile(""::"b"(ip):);
		asm volatile("movq %rbx, %r15");

		asm volatile("movq %r15, %rbx");
		asm volatile("shr $24, %rbx");
		asm volatile("xor %rdx, %rdx");
		asm volatile("movb $0x2e, %dh");
		asm volatile("xor %rax, %rax");
		asm volatile("movb %bl, %al");
		asm volatile("movb $10, %cl");
		asm volatile("div %cl");
		asm volatile("movb %ah, %dl"); //   1
		asm volatile("add $0x30, %dl");//   1
		asm volatile("shl $16, %edx");
		asm volatile("movb $0, %ah");  //   19
		asm volatile("div %cl");
		asm volatile("movb %ah, %dh"); //9
		asm volatile("add $0x30, %dh");//9
		asm volatile("movb %al, %dl");
		asm volatile("add $0x30, %dl");
		asm volatile("movl %%edx, %0":"=m"(ip_str)::);

		asm volatile("movq %r15, %rbx");
		asm volatile("shr $16, %rbx");
		asm volatile("xor %rdx, %rdx");
		asm volatile("movb $0x2e, %dh");
		asm volatile("xor %rax, %rax");
		asm volatile("movb %bl, %al");
		//      asm volatile("movb $10, %cl");
		asm volatile("div %cl");
		asm volatile("movb %ah, %dl"); //   1
		asm volatile("add $0x30, %dl");//   1
		asm volatile("shl $16, %edx");
		asm volatile("movb $0, %ah");  //   19
		asm volatile("div %cl");
		asm volatile("movb %ah, %dh"); //9
		asm volatile("add $0x30, %dh");//9
		asm volatile("movb %al, %dl");
		asm volatile("add $0x30, %dl");
		asm volatile("movl %%edx, %0":"=m"(ip_str[4])::);

		asm volatile("movq %r15, %rbx");
		asm volatile("shr $8, %rbx");
		asm volatile("xor %rdx, %rdx");
		asm volatile("movb $0x2e, %dh");
		asm volatile("xor %rax, %rax");
		asm volatile("movb %bl, %al");
		//       asm volatile("movb $10, %cl");
		asm volatile("div %cl");
		asm volatile("movb %ah, %dl"); //   1
		asm volatile("add $0x30, %dl");//   1
		asm volatile("shl $16, %edx");
		asm volatile("movb $0, %ah");  //   19
		asm volatile("div %cl");
		asm volatile("movb %ah, %dh"); //9
		asm volatile("add $0x30, %dh");//9
		asm volatile("movb %al, %dl");
		asm volatile("add $0x30, %dl");
		asm volatile("movl %%edx, %0":"=m"(ip_str[8])::);

		asm volatile("movq %r15, %rbx");
		asm volatile("xor %rdx, %rdx");
		asm volatile("movb $0x2e, %dh");
		asm volatile("xor %rax, %rax");
		asm volatile("movb %bl, %al");
		//        asm volatile("movb $10, %cl");
		asm volatile("div %cl");
		asm volatile("movb %ah, %dl"); //   1
		asm volatile("add $0x30, %dl");//   1
		asm volatile("shl $16, %edx");
		asm volatile("movb $0, %ah");  //   19
		asm volatile("div %cl");
		asm volatile("movb %ah, %dh"); //9
		asm volatile("add $0x30, %dh");//9
		asm volatile("movb %al, %dl");
		asm volatile("add $0x30, %dl");
		asm volatile("movl %%edx, %0":"=m"(ip_str[12])::);

	}
	printf("ip_str:%s\n", ip_str);
}
