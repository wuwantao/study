#include <stdio.h>
#include <stdlib.h>
int main()
{
    int i = 25;
    printf("i/8:\t%d\n", i >> 3);   //除法 i / 2^n == i >> n
    i = 3; 
    printf("i*8:\t%d\n", i << 3);   //乘法 i * 2^n == i << n
    i = 26;
    printf("i%8:\t%d\n", i & 7);    //求余 x % 2^n == x & (2^n - 1)
    return 0;
}
