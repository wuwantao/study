#include <stdio.h>
#include <stdint.h>
#include <time.h>
#define COUNT 100ULL * 10000ULL * 10000ULL

void test_bit()
{
    uint64_t i = 0;
    uint64_t j = 0;
    time_t begin;
    time_t end;
    begin = time(NULL);
    for (i = 0; i < COUNT; i++) {
        j = i >> 3;
    }
    end = time(NULL);
    printf(">> used:%lld\n", end - begin);
}


void test_base()
{
    uint64_t i = 0;
    uint64_t j = 0;
    time_t begin;
    time_t end;
    begin = time(NULL);
    for (i = 0; i < COUNT; i++) {
        j = i % 8;
    }
    end = time(NULL);
    printf("%% used:%lld\n", end - begin);
}

int main()
{
    test_bit();
    test_base();
    return 0;
}
