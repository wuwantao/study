#!/bin/bash
gcc -std=gnu99 -ggdb -o t t.c -I./ldns-1.7.1/include ./ldns-1.7.1/.libs/libldns.a   -lpthread  -lcrypto -lpcap
gcc -S t.c
