#!/bin/bash
gcc -g -O0 -o t t.c -fsanitize=address -fno-omit-frame-pointer

