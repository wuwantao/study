#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

double timediff(clock_t t1, clock_t t2)
{
    double elapsed;
    elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC;
    return elapsed;
}
