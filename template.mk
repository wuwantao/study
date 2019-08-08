BINARY := t
$(shell mkdir -p obj)
src := $(shell ls *.c)
objs := $(patsubst %.c,obj/%.o,$(src))
obj/%.o: %.c
	gcc -c -o $@ $<  $(CFLAGS)
all: $(BINARY)
#	cp $(BINARY) $(BINARY).strip
#	strip $(BINARY).strip

$(BINARY):$(objs)
	gcc $(wildcard obj/*.o) -o $@ $(CFLAGS)
clean:
	rm -rf obj
	rm -rf t t.strip
