#!/bin/bash
gcc -std=gnu99 -ggdb -o t t.c
gcc -S t.c
