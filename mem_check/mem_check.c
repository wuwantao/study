#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "list.h"

typedef struct { 
    struct list_head mem_check_node;
    char filename[10];
    uint32_t linename;
    uint32_t size;
    uint8_t  data[0];
} mem_node_t;

struct list_head g_mem_check_list;
pthread_mutex_t  g_mem_check_lock = PTHREAD_MUTEX_INITIALIZER;

void __attribute__((constructor)) mem_init()
{
    INIT_LIST_HEAD(&g_mem_check_list);
}

void *__mem_malloc(uint32_t size, char *filename, uint32_t linename)
{

    mem_node_t *mem_node = NULL;
    void *t = NULL;
    
    mem_node = malloc(sizeof(mem_node_t) + size + 4);
    
    if (mem_node == NULL) {
        printf("malloc_node_t failed.\n");
        return NULL;
    }
    
    
    memcpy(mem_node->data + size, "aaaa", 4);
    
    mem_node->linename = linename;
    memcpy(mem_node->filename, filename, 10);
    mem_node->size = size;
    pthread_mutex_lock(&g_mem_check_lock);
    list_add(&mem_node->mem_check_node, &g_mem_check_list);
    pthread_mutex_unlock(&g_mem_check_lock);
    
    return mem_node->data;
}

void mem_free(void *p)
{
    mem_node_t *mem_node = NULL;
    
    mem_node = container_of(p, mem_node_t, data);

    if (memcmp(mem_node->data + mem_node->size, "aaaa", 4) != 0) {
        printf("error: file:%s  line:%d\n", mem_node->filename, mem_node->linename);
    }
    
    pthread_mutex_lock(&g_mem_check_lock);
    list_del(&mem_node->mem_check_node);
    pthread_mutex_unlock(&g_mem_check_lock);
    free(mem_node);
}

void mem_print()
{
    mem_node_t *pos = NULL;
    list_for_each_entry(pos, &g_mem_check_list, mem_check_node) {
        if (memcmp(pos->data + pos->size, "aaaa", 4) != 0) {
            printf("error: file:%s  line:%d\n", pos->filename, pos->linename);
        }
    }
}

void __mem_memcpy(void *dst, void *src, uint32_t len, char *file, uint32_t line)
{
    mem_node_t *mem_node = NULL;

    mem_node = container_of(dst, mem_node_t, data);
    mem_node->linename = line;
    memcpy(mem_node->filename, file, 10);
    memcpy(dst, src, len);
    if (len > mem_node->size) {
        printf("error: file:%s  line:%d\n", mem_node->filename, mem_node->linename);
        exit(-1);
    }
}

