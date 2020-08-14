#include <stdio.h>
#include <string.h>
#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)


void test(char *p)
{
	printf("sizeof(p):%d\n", sizeof(p));
	printf("p[100]:%d\n", p[100]);


}

void main()
{
	long long i = 0;
	i = random();

	if (unlikely(i == 0)) {
		i = 1;
	} else {
		i = 2;
	}

	printf("i:%d\n", i);


	char p[1024];
	memset(p, 1, sizeof(p));

	printf("main: sizeof(p):%d\n", sizeof(p));

	test(p);

}
