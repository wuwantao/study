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


	printf("inet_pton: 0x%x\n", in4_addr.s_addr); 
	ret = inet_pton(AF_INET, p, &in4_addr);

	printf("ret:%d\n", ret);
	printf("inet_pton: 0x%x\n", in4_addr.s_addr); 

	if (ret != 1) {
		perror("inet_pton");
	}

	return 1;
}
