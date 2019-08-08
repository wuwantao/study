#include <stdio.h>
#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

void main()
{
    long long i = 0;
    int step = time(NULL) >> 30;

    for (i = 0; i < 50 * 10000 * 10000ULL; i++) {
        if (unlikely(i + step < 0)) {
//        if (i + step < 0) {
            test1();
        } else {
            test2();
        }
    }
}
