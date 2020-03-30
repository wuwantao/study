#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>



void test()
{

}

int main(void)
{
    mallopt(M_MMAP_THRESHOLD, 0);
    char *p = malloc(100);
    printf("p:%#x\n", p);
    free(p);

    strcpy(p, "hello world!\n");
    printf("p:%s\n", p);

    return 0;
}
