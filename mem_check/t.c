#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "mem_check.h"

int main()
{
    char *p1 = (char *)mem_malloc(5);
    char *p2 = (char *)mem_malloc(10);
    
    mem_memcpy(p1, "12345", 5);
    mem_memcpy(p2, "1234567890", 10);
    
    mem_print();
    
    mem_free(p1);
    mem_free(p2);
    
    return 0;
}
