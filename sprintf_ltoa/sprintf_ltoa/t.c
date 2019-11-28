#include "t.h"
const int64_t INT_NUM = 10000 * 200;
int main()
{
    //init num
    int value[INT_NUM];
    for (int64_t idx = 0; idx < INT_NUM; ++idx) {
        value[idx] = random();
    }
    const int64_t MAX_CONST_LENGTH = 22;
    char str[MAX_CONST_LENGTH];

    struct timeval t_start, t_end;
    long start, end;

    //get snprintf time
    gettimeofday(&t_start, NULL);
    start = t_start.tv_sec * 1000000 + t_start.tv_usec;
    for (int64_t idx = 0; idx < INT_NUM; ++idx) {
        snprintf(str, MAX_CONST_LENGTH, "%ld", value[idx]);
    }
    gettimeofday(&t_end, NULL);
    end = t_end.tv_sec * 1000000 + t_end.tv_usec;
    printf("snprintf time:%ld\n", end - start);

    //get sprintf time
    gettimeofday(&t_start, NULL);
    start = t_start.tv_sec * 1000000 + t_start.tv_usec;
    for (int64_t idx = 0; idx < INT_NUM; ++idx) {
        sprintf(str, "%ld", value[idx]);
    }
    gettimeofday(&t_end, NULL);
    end = t_end.tv_sec * 1000000 + t_end.tv_usec;
    printf("sprintf time:%ld\n", end - start);


    //get ltoa10 time
    gettimeofday(&t_start, NULL);
    start = t_start.tv_sec * 1000000 + t_start.tv_usec;
    for (int64_t idx = 0; idx < INT_NUM; ++idx) {
        ltoa10(value[idx], str, true);
    }
    gettimeofday(&t_end, NULL);
    end = t_end.tv_sec * 1000000 + t_end.tv_usec;
    printf("ltoa use - time:%ld\n", end - start);
    //get ltoa10_2 time
    gettimeofday(&t_start, NULL);
    start = t_start.tv_sec * 1000000 + t_start.tv_usec;
    for (int64_t idx = 0; idx < INT_NUM; ++idx) {
        ltoa10_2(value[idx], str, true);
    }
    gettimeofday(&t_end, NULL);
    end = t_end.tv_sec * 1000000 + t_end.tv_usec;
    printf("ltoa_2 use %% time:%ld\n", end - start);
    return 0;
}
