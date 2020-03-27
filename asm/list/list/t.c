#include <stdio.h>
#include <stdlib.h>

#define NTOHS(x)\
asm volatile("xchgb %b[x1],%h[x2]"\
          : [x1] "=Q" (x)\
          : [x2] "0" (x)\
          ),x


int main()
{

    int a = 0x1122;
    int b =0;
    printf("a:%x\n", a);
    memcpy(a,b,1);

}
