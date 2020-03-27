#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct test {
    int a;
    char b[20];
} test_t;

int main()
{
    test_t A;

    test_t B;

    A.a = 65;
    strcpy(A.b, "AAAAAAAAA");

    B.a = 66;
    strcpy(B.b, "BBBBBBBBB");

    B=A;
    
    printf("B.a:%d\n",B.a);
    printf("B.b:%s\n",B.b);

    B.a = 66;
    strcpy(B.b, "BBBBBBBBB");

    test_t *a = &A;
    test_t *b = &B;


    *b = *a;

    printf("B.a:%d\n",B.a);
    printf("B.b:%s\n",B.b);

    return 0;
}
