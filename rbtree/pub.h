#ifndef __PUB_H
#define __PUB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/time.h>
#include <arpa/inet.h>

typedef signed char s8;
typedef unsigned char u8;
typedef unsigned char uint8_t;

typedef signed short s16;
typedef unsigned short u16;
typedef unsigned short uint16_t;

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned int uint32_t;

typedef signed long long s64;
typedef unsigned long long u64;

struct y_ip {
    union {
        uint32_t ip_num;
        uint8_t ip[4];
    };
};

// typedef unsigned long long uint64_t;

/**
 * reciprocal_scale - "scale" a value into range [0, ep_ro)
 * @val: value
 * @ep_ro: right open interval endpoint
 *
 * Perform a "reciprocal multiplication" in order to "scale" a value into
 * range [0, ep_ro), where the upper interval endpoint is right-open.
 * This is useful, e.g. for accessing a index of an array containing
 * ep_ro elements, for example. Think of it as sort of modulus, only that
 * the result isn't that of modulo. ;) Note that if initial input is a
 * small value, then result will return 0.
 *
 * Return: a result based on val in interval [0, ep_ro).
 */
static inline u32 reciprocal_scale(u32 val, u32 ep_ro)
{
    return (u32)(((u64) val * ep_ro) >> 32);
}

// #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})


#ifndef likely
#define likely(x) __builtin_expect ((x), 1)
#define unlikely(x) __builtin_expect ((x), 0)
//#define likely(x) __builtin_expect(!!(x), 1)
//#define unlikely(x)   __builtin_expect(!!(x), 0)
#endif /* likely and unlikely */

/*
#define ASSERT(x)   do {    \
        if (!(x))   \
            printf("[%s][%04d]", __func__, __LINE__);   \
        assert((x));    \
    } while (0)
*/

static inline long tv_diff_usec(struct timeval *tv1, struct timeval *tv2)
{
    long usec = 0, sec = 0;

    usec = tv1->tv_usec - tv2->tv_usec;
    sec = tv1->tv_sec - tv2->tv_sec;

    return ((sec * 1000000 + usec));
}

static inline int tv_diff(long tv1, long tv2)
{
    return (tv1 - tv2);
}

static inline long cur_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

static inline int y_tolower(int c)
{
    switch (c) {
    case 'A'...'Z':
        c += 'a' - 'A';
        break;
    default:
        break;
    }

    return c;
}

static inline int yprintf(char* buf, const char* fmt, ...)
{
    va_list vap;
    int n;
    va_start(vap, fmt);
    /*
    if(buf == NULL)
        n =  printf(fmt, vap);
    else
    */
    n =  sprintf(buf, fmt, vap);
    va_end(vap);
    return n;
}

static inline uint32_t ipv4_mask(int mask)
{
    uint32_t ip_num = 0;
    int i = 0;

    for (i = 1; i <= mask; i++) {
        ip_num |= (1 << (32 - i));
    }

    return htonl(ip_num);
}

#endif

