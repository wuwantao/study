#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

int i = 1;
struct in_addr  in4_addr;
struct in6_addr in6_addr;
char str1[128];
char str2[128] ="hello";
int main() 
{
	int ret = 0;
	const char *p = "192.168.1.1/24";


	printf("sizeof(in4_addr): %d \n", sizeof(in4_addr));
	printf("sizeof(struct in_addr): %d \n", sizeof(struct in_addr));

	printf("sizeof(in6_addr): %d \n", sizeof(in6_addr));
	printf("sizeof(struct in6_addr): %d \n", sizeof(struct in6_addr));

	printf("sizeof(str1):%d\n", sizeof(str1));


	return 1;
}
