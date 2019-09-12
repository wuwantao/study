/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <stdlib.h>
#include <string.h>

#include <jansson.h>

#define check_json_str(key) \
    \
        if (!json_is_string(key)) {\
                        snprintf(err_str, sizeof(err_str) - 1, #key" is not a string\n");\
                        printf("line:%d error:%s\n", __LINE__, err_str); \
                        exit(0);\
                    }

#define check_json_array(key) \
    \
        if (!json_is_array(key)) {\
                        snprintf(err_str, sizeof(err_str) - 1, #key" is not a array\n");\
                        printf("line:%d error:%s\n", __LINE__, err_str); \
                        exit(0);\
                    }

#define check_json_object(key) \
    \
        if (!json_is_object(key)) {\
                        snprintf(err_str, sizeof(err_str) - 1, #key" is not a object\n");\
                        printf("line:%d error:%s\n", __LINE__, err_str); \
                        exit(0);\
                    }

int main()
{
    json_t *json;
    json_error_t error;
    char err_str[512] = {0};
    void *iter;

    json = json_load_file("t.json", 0, &error);
    if (!json) {
        /* the error variable contains error information */
    }
    check_json_object(json);

    iter = json_object_iter(json);
    while(iter) {
        const char *key;
        json_t *value;

        key = json_object_iter_key(iter);
        value = json_object_iter_value(iter);
        printf("key:%s\n", key);
        iter = json_object_iter_next(json, iter);
    }

    json_decref(json);

}
