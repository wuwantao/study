#include <stdio.h>
typedef struct xxx {
    int a;
    int b;
}xxx_t;

int main(void)
{
    xxx_t a[22];

    a[0].a = 2;
    memset(a, 0, sizeof(xxx_t)*22);

    printf("a.a:%d\n",a[0].a);
    return 0;
}
