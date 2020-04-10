#include <string.h>
#include <stdio.h>



void test()
{
    char array[2] = {0};

    strcpy(array, "stackwilloverflow");

}

int main(void)
{
    int a;

    a = 1;
    test();
    a = 2*a;
    printf("xxx");

    return 0;
}
