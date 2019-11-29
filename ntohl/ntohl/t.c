#include "t.h"
#define INT_NUM (10000ULL * 1000)
typedef struct val {
    uint64_t value[INT_NUM];
} val_t;
//#define NTOHL(x) asm volatile("bswap %q0" : "=r" (x) : "0" (x));
#define NTOHL(x) asm volatile("bswap %0" : "=r" (x) : "0" (x));
int main()
{
#if 0
    uint32_t a = random();
      a = ntohl(a);
//        NTOHL(a);
        printf("a:%x\n",a);
        return 0;
#endif        
    //init num
    val_t *val = malloc(sizeof(val_t));
    uint64_t res = 0;
    if (val == NULL) {
        printf("malloc failed.\n");
        return -1;
    }
    for (int64_t idx = 0; idx < INT_NUM; ++idx) {
        val->value[idx] = random();
    }

    struct timeval t_start, t_end;
    long start, end;

    //get ntohl time
    gettimeofday(&t_start, NULL);
    start = t_start.tv_sec * 1000000 + t_start.tv_usec;
    for (int64_t idx = 0; idx < INT_NUM; ++idx) {
        val->value[idx] = ntohl(val->value[idx]);
    }
    gettimeofday(&t_end, NULL);
    end = t_end.tv_sec * 1000000 + t_end.tv_usec;
    printf("ntohl time:%ld\n", end - start);

    //get NTOHL time
    gettimeofday(&t_start, NULL);
    start = t_start.tv_sec * 1000000 + t_start.tv_usec;
    for (int64_t idx = 0; idx < INT_NUM; ++idx) {
        NTOHL(val->value[idx]);
    }
    gettimeofday(&t_end, NULL);
    end = t_end.tv_sec * 1000000 + t_end.tv_usec;
    printf("NTOHL time:%ld\n", end - start);

    return 0;
}
