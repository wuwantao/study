#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "list.h"


void *__mem_malloc(uint32_t size, char *filename, uint32_t linename);
void __mem_memcpy(void *dst, void *src, uint32_t len, char *file, uint32_t line);
void mem_free(void *p);
void mem_print();

#define mem_malloc(x) __mem_malloc(x, __FILE__, __LINE__)
#define mem_memcpy(dst, src, len) __mem_memcpy(dst, src, len, __FILE__, __LINE__)
