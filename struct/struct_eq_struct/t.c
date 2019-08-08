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

    A.a = 12;
    strcpy(A.b, "hello");

    B.a = 13;
    strcpy(B.b, "world");

#if 0
    if (A == B) {
        printf("A == B is true\n");
    } else {
        printf("A == B is false\n");
    }
#endif

    return 0;
}
