#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{

    int val = 2;
    int mask = 2;
    int ret = 0;
    int ret1 = 0;

    ret = val & mask == 2;
    ret1 = 2 == 2;

    printf("ret:%d\n", ret);
    printf("ret1:%d\n", ret1);
    return 0;
}
