/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <stdlib.h>
#include <string.h>

#include <jansson.h>
typedef int (*http_call)();
#define callback(...) \
    char *_http_urls = #__VA_ARGS__;\
 \
http_call http_calls[] = {__VA_ARGS__};



int test1(){
}
int test2(){
}
int test3(){
}
callback(
        test1, 

        test2, 

        test3);
#define check_json_str(key) \
    \
if (!json_is_string(key)) {\
    snprintf(err_str, sizeof(err_str) - 1, #key" is not a string\n");\
    printf("error:%s\n", err_str); \
    exit(0);\
}

#define check_json_array(key) \
    \
if (!json_is_array(key)) {\
    snprintf(err_str, sizeof(err_str) - 1, #key" is not a array\n");\
    printf("error:%s\n", err_str); \
    exit(0);\
}

#define check_json_object(key) \
    \
if (!json_is_object(key)) {\
    snprintf(err_str, sizeof(err_str) - 1, #key" is not a object\n");\
    printf("error:%s\n", err_str); \
    exit(0);\
}

char http_urls[512];
int main()
{
    json_t *json;
    json_error_t error;
    char err_str[512] = {0};
    const char *key;
    json_t *value;
    void *tmp;
    char *p = NULL;
    int i = 0;

    strncpy(http_urls, _http_urls, sizeof(http_urls));
    p = strtok(http_urls, ",");
    do {    
        if (p == NULL || strncasecmp(p,"NULL", strlen(p)) == 0) {
            break;
        }
        printf("%d:%s\n", i, p);
        i++;
        p = strtok(NULL, ", ");
    } while(1);
    return 1;

    json = json_load_file("t.json", 0, &error);
    if (!json) {
        /* the error variable contains error information */
    }
    check_json_object(json);

    json_object_foreach_safe(json, tmp, key, value) {
        printf("key:%s\n", key);
    }

}
