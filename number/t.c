#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

int main()
{
	//0x10000;
	int i = 0;
	i = 0x1;
	printf("0x1:%d\n", i);

	i = 0x10;
	printf("0x10:%d\n", i);

	i = 0x100;
	printf("0x100:%d\n", i);

	i = 0x800;
	printf("0x800:%d\n", i);

	i = 0x1000;
	printf("0x1000:%d\n", i);

	i = 0x10000;
	printf("0x10000:%d\n", i);

	i = 0x100000;
	printf("0x10000:%d\n", i);
	printf("0x100000/1024k:%d\n", i / 1024);
	printf("0x100000/1024/1024m:%d\n", i / 1024 / 1024);

	i = 0x1000000;
	printf("0x1000000:%d\n", i);

	i = 0x1000000;
	printf("0x1000000/1024k:%d\n", i / 1024);
	printf("0x1000000/1024/1024m:%d\n", i / 1024 / 1024);

	i = 1 << 0;
	printf("1<<0 == 2^0:%d\n", i);

	i = 1 << 1;
	printf("1<<1 == 2^1:%d\n", i);

	i = 1 << 2;
	printf("1<<2 == 2^2:%d\n", i);

	i = 1 << 3;
	printf("1<<3 == 2^3:%d\n", i);

	i = 1 << 4;
	printf("1<<4 == 2^4:%d\n", i);

	i = 1 << 5;
	printf("1<<5 == 2^5:%d\n", i);
	for (i = 6; i < 30; i++) {
		int tmp = 0;
		tmp = 1 << i;
		printf("1<<%d == 2^%d:%d\n", i, i, tmp);
	}
    return 0;
	int j = 0;
	for (j = 0; j < 100; j++) {
		printf("字体颜色:%d\t\e[0;%dmexample\e[0m\n", j, j);
	}

	for (j = 0; j < 48; j++) {
		printf("背景颜色:%d\t\e[%d;37mexample\e[0m\n", j, j);
	}
	for(;;)
	{
		printf("\e[1;92m%d\e[0m",rand()%2);
	}


	return 0;
}
test()
{
	int m = 0;
	int n = 2;
	int k = 0;
	int tmp_k = 0;
	int j = 0;
	for(;;)
	{
		printf("\e[2J");
		m = 0;
		n = 2;
		k = 0;
		tmp_k = 0;
		for (k = 0 ; k < 9; k++) {
			usleep(1000*50);
			printf("\e[2J");
			m = 1;
			for (m = 1; m < 15; m = m + n) {
				tmp_k = 0;
				for (tmp_k = 0; tmp_k < k; tmp_k++)
				{
					printf("\t\t");
				}
				for (j = 0; j < m; j++) {
					printf("\e[1;92m*\e[0m");
				}
				printf("\n");
			}
			m = 15;
			for (m = 15; m > 0; m = m - n) {
				tmp_k = 0;
				for (tmp_k = 0; tmp_k < k; tmp_k++)
				{
					printf("\t\t");
				}
				for (j = 0; j < m; j++) {
					printf("\e[1;92m*\e[0m");
				}
				printf("\n");
			}
		}
	}

	//IPV4地址
	uint32_t ipv4;
	//IPV6地址

}
