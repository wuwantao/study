#include <stdio.h>
#include <stdint.h>
#include <string.h>
int main()
{
    uint16_t t = 0xc00c;
    uint16_t t1 = t << 2;
    uint16_t t2 = t1 >> 2;

    printf("t:%d\n", t << 2);
    return 0;
    printf("t2:%d\n", t2);

    char a = 0xb6;
    short b = 0xb600;
    int c = 0xb6000000;
    if (a == 0xb6)      //char类型，会整形提升，提升完为0xffffffb6
        printf("a");
    if (b == 0xb600)        //short类型，也会整形提升，提升完为0xffffb600
        printf("b");
    if (c == 0xb6000000) //不用整形提升
        printf("c");


    return 0;
}
