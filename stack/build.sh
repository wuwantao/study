#!/bin/bash
#gcc -g3 -O0 -o t  t.c  -fstack-protector-all 
#gcc -g3 -O0 -o t  t.c  -fstack-protector
gcc -std=c99 -g3 -O0 -o t1  t.c  -fstack-protector-all
gcc -std=c99 -g3 -O0 -o t2  t.c 
objdump -S t1 > t1.s
objdump -S t2 > t2.s
