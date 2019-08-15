#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct test {
    int a;
    char b[20];
} test_t;

int main()
{
    test_t C;
    test_t *A=&C;


    test_t *B = A+1;

    printf("B-A:%d\n",B-A);

    return 0;
}
