all:
	gcc -O2 -g -o t t.c t1.c
	gcc -O2 -S t.c -o t_O2.s
	gcc -S t.c -o t.s
clean:
	rm -rf t *.S *.s
