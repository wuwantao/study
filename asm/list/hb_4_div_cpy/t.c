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

inline static bool ltoa10(int64_t val, uint8_t **d)
{
	return true;
}
int main()
{
	uint64_t i = 0;
	uint8_t buffer[65];
	uint64_t uval = (uint64_t) val;
	uint8_t *dst = *d;

	for (i = 0; i < 10000ULL * 10000ULL; i++) {

		register char *p = &buffer[sizeof(buffer) - 1];
		*p = '\0';
		int64_t new_val = (int64_t) (uval / 10);
		*--p = (uint8_t)('0' + (uval - (uint64_t) new_val * 10));
		val = new_val;

		while (val != 0) {
			new_val = val / 10;
			*--p = (char)('0' + (val - new_val * 10));
			val = new_val;
		}

		while ((*dst++ = *p++) != 0) ;

		dst = dst - 1;
		*d = dst;

	}
	printf("ip_str:%s\n", ip_str);
}
