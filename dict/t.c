#include <stdio.h>
#include <stdint.h>
#include <string.h>
struct pro_dict {
	char pro_str[255][16];
	uint8_t pro_int[255];
} g_pro_dict = {
	{"icmp", "tcp", "udp"},
	{1, 6, 17}
};
int main()
{
	uint8_t i = 0;
	for(i = 0; i< 10;i++) {
		printf("%s:%d\n", g_pro_dict.pro_str[i], g_pro_dict.pro_int[i]);
	}

	return 0;
}
