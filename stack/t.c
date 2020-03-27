#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


void test()
{
	uint32_t a = 0x11223344;
	uint8_t b[100] = {0x11, 0x22, 0x33, 0x44};


	b[104] = 0x55;
	b[105] = 0x66;
	b[106] = 0x77;
	b[107] = 0x88;
	b[108] = 0x99;
	b[109] = 0x91;
	b[110] = 0x92;
	b[111] = 0x92;
	b[112] = 0x92;
	b[113] = 0x92;
	b[114] = 0x92;

}



void main() 
{
	uint32_t a = 0x11223344;
	uint8_t b[4] = {0x11, 0x22, 0x33, 0x44};


	b[4] = 0x55;
	b[5] = 0x66;
	b[6] = 0x77;
	b[7] = 0x88;
	char *p = malloc(0x10);
	memset(p, 0x11, 0x10);
	printf("p:%p\n", p);
	free(p);

	test();
	return;

}
