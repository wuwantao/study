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


#define MAX_COUNT 1000 * 10000 // 千万次

int sum = 0;
pthread_mutex_t mutex;
pthread_spinlock_t spinlock;

volatile int lock = 0;

/**
 * real	0m5.396s
 * mutex占用40个字节
 */
void* adder0(void *p)
{
    int i = 0;

    for (i = 0; i < MAX_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        sum++;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

/**
 * real	0m7.631s
 * 在竞争激烈的情况下:
 * 主要是交换失败的,就要进入循环,反而消耗更多时间
 */
void* adder1(void *p)
{
    int i = 0;
    for (i = 0; i < MAX_COUNT; i++) {
        //交换失败,就一直等到交换成功
        for(;;) {
            if (0 != __sync_bool_compare_and_swap(&lock, 0, 1)) { 
                sum++;
                lock = 0;
                break;
            }
        }
    }

    return NULL;
}
/**
 * real	0s
 * 在竞争激烈的情况下:
 * 主要是交换失败的,就要进入循环,反而消耗更多时间
 */
void* adder7(void *p)
{
    int i = 0;
    for (i = 0; i < MAX_COUNT; i++) {
        for(;;) {
            if (0 != __sync_bool_compare_and_swap(&lock, 0, 1)) { 
                sum++;
                lock = 0;
                break;
            }
            usleep(1);
        }
    }

    return NULL;
}

void* adder8(void *p)
{
    int old = sum;
    int i = 0;

    for (i = 0; i < MAX_COUNT; i++) {
        old = sum;//加这个快,是因为只有一个线程跑时,__sync不会多执行一次失败,在进入执行old = sum,造成慢
        while (0 != __sync_bool_compare_and_swap(&sum, old, old +1)) { //成功进入 0 是失败, 0 是失败, 加个!是失败
            //故意让不成功进入
            old = sum;
        }
    }

    return NULL;
}

/**
 * real	0m2.092s
 *
 */
void* adder2(void *p)
{
    int i = 0;

    for (i = 0; i < MAX_COUNT; i++) {
        if (__sync_bool_compare_and_swap(&lock, 0, 1)) {
            sum++;
            //__atomic_store_n(&lock, 0, __ATOMIC_SEQ_CST);
            lock = 0;
        }
    }

    return NULL;
}

/**
 * real	0m15.929s
 *
 */
void* adder3(void *p)
{
    int i = 0;

    for (i = 0; i < MAX_COUNT; i++) {
        pthread_spin_lock(&spinlock);
        sum++;
        pthread_spin_unlock(&spinlock);
    }

    return NULL;
}

/**
 * real	0m2.285s
 *
 */
void* adder4(void *p)
{
    int i = 0;

    for (i = 0; i < MAX_COUNT; i++) {
        if (0 == pthread_spin_trylock(&spinlock)) {
            sum++;
            pthread_spin_unlock(&spinlock);
        }
    }

    return NULL;
}

/**
 * real	0m2.076s
 */
void* adder5(void *p)
{
    int i = 0;

    for (i = 0; i < MAX_COUNT; i++) {
        __sync_add_and_fetch(&sum, 1);
    }

    return NULL;
}

/**
 * real	0m2.076s
 */
void* adder6(void *p)
{
    int i = 0;
    static volatile int lock = 0;
    
    for (i = 0; i < MAX_COUNT; i++) {
        if (__sync_lock_test_and_set(&lock, 1) == 0) {
            sum++;
            __sync_lock_release(&lock); 
        }
    }

    return NULL;
}


void* (*funcs[])(void *) = {
    adder0,
    adder1,
    adder2,
    adder3,
    adder4,
    adder5,
    adder6,
    adder7,
    adder8
};

char *str[] = {
    "pthread_mutex_lock",
    "while __sync_bool_compare_and_swap",
    "if __sync_bool_compare_and_swap",
    "pthread_spin_lock",
    "pthread_spin_trylock",
    "__sync_add_and_fetch",
    "__sync_lock_test_and_set",
    "new while __sync_bool_compare_and_swap",
    "new old while __sync_bool_compare_and_swap",
};

#define thread_num 4

void test(void *p)
{
    int i = 0;
    pthread_t threads[thread_num];
    pthread_mutex_init(&mutex, NULL);
    pthread_spin_init(&spinlock, 0);

    for ( i = 0; i < thread_num; i++) {
        pthread_create(&threads[i], NULL, p, NULL);
    }

    for ( i = 0; i < thread_num; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_spin_destroy(&spinlock);
}

int main()
{
    int i = 0;
    int size = 0;
    clock_t end;
    clock_t start;
    time_t a1;
    time_t a2;
    size = sizeof(funcs) / sizeof(void*);

    for (i = 0; i < size; i++) {
        start = clock();
        a1 = time(NULL);
        sum = 0;
        lock = 0;
        test(funcs[i]);
        a2 = time(NULL);
        end = clock();
        printf("adder%d sum:%10ld\t%41s:%f\ttime:%lld\n",
               i,
               sum,
               str[i],
               timediff(start, end), a2 - a1);
    }
}
