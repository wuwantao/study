#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "rte_ring.h"

#define BUF_LEN 2048
#define COUNT 10000000ULL
#define THREADS 2

typedef struct {
    uint8_t buff[BUF_LEN];
} buff_t;

void *__test_malloc(void *arg)
{
    uint64_t i = 0;
    uint64_t n = 0;
    char *p[200] = {NULL};
    time_t begin;
    time_t end;

    for (i = 0; i < COUNT; i++) {
        p[n] = malloc(BUF_LEN);
        if (NULL != p[n]) {
            n++;
        } else {
            printf("malloc failed\n");
        }

        if (n >= 200) {
            while (n--) {
                free(p[n]);
            }
        }

    }
    return NULL;
}

void test_malloc()
{
    time_t x;
    pthread_t tid[THREADS];
    int i = 0;

    x = time(NULL);
    for (i = 0; i < THREADS; i++) {
        pthread_create(&tid[i], NULL, __test_malloc, NULL);
    }

    for (i = 0; i < THREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    printf("malloc used time:%ld\n", time(NULL) - x);
}

void *__test_ring_thread(void *arg)
{
    buff_t *dep;
    struct rte_ring *ring = (struct rte_ring *)arg;
    uint64_t i = 0;

    for (i = 0; i < COUNT; i++) {
        if ((rte_ring_mc_dequeue(ring, (void **)&dep)) != 0) {
            printf("出队失败\n");
            continue;
        }

        if ((rte_ring_mp_enqueue(ring, dep)) != 0) {
            printf("入队失败\n");
            continue;
        }
    }

    return NULL;
}

void test_ring()
{
    uint64_t i = 0;
    char *p = NULL;
    buff_t *pbuff = NULL;
    struct rte_ring *test_queue = NULL;
    time_t x;
    pthread_t tid[THREADS];

    test_queue = rte_ring_create("test_queue", 1 << 21, 0); //200万
    p = malloc(BUF_LEN * 10000ULL);

    pbuff = (buff_t *)p;

    for (i = 0; i < 10 * 10000ULL; i++) {

        if ((rte_ring_sp_enqueue(test_queue, pbuff + i)) != 0) {
            printf("入队失败\n");
            free(p);
            continue;
        }
    }

    x = time(NULL);

    for (i = 0; i < THREADS; i++) {
        pthread_create(&tid[i], NULL, __test_ring_thread, test_queue);
    }
    for (i = 0; i < THREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    printf("ring used time:%ld\n", time(NULL) - x);
}

int main()
{
    test_ring();
    test_malloc();
    return 0;
}

