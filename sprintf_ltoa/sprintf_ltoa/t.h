#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

char *ltoa10_2(int64_t val, char *dst, const bool is_signed);
char *ltoa10(int64_t val, char *dst, const bool is_signed);
inline char *ltoa10_2(int64_t val, char *dst, const bool is_signed)
{
    char buffer[65];
    uint64_t uval = (uint64_t) val;

    if (is_signed) {
        if (val < 0) {
            *dst++ = '-';
            uval = (uint64_t)0 - uval;
        }
    }

    register char *p = &buffer[sizeof(buffer) - 1];
    *p = '\0';
    while (val != 0) {
        *--p = val % 10;
        val = val / 10;
    }

    while ((*dst++ = *p++) != 0) ;
    return dst - 1;
}
inline char *ltoa10(int64_t val, char *dst, const bool is_signed)
{
    char buffer[65];
    uint64_t uval = (uint64_t) val;

    if (is_signed) {
        if (val < 0) {
            *dst++ = '-';
            uval = (uint64_t)0 - uval;
        }
    }

    register char *p = &buffer[sizeof(buffer) - 1];
    *p = '\0';
    int64_t new_val = (int64_t) (uval / 10);
    *--p = (char)('0' + (uval - (uint64_t) new_val * 10));
    val = new_val;

    while (val != 0) {
        new_val = val / 10;
        *--p = (char)('0' + (val - new_val * 10));
        val = new_val;
    }
    while ((*dst++ = *p++) != 0) ;
    return dst - 1;
}


