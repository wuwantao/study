dname := $(notdir $(shell pwd))
$(dname)_srcs := $(wildcard *.c)
$(dname)_objs = $(patsubst %.c,$(OBJS_DIR)/%.o,$($(dname)_srcs))

subdirs := $(subst ./,,$(shell find . -maxdepth 1 -type d|sed -e '1d'))

all: $($(dname)_objs)
	@for dir in $(subdirs); do if [ -f $$dir/Makefile ]; then $(MAKE) -C $$dir; fi; done

$(OBJS_DIR)/%.o: %.c
	$(COMPILE) $@ $< $(CFLAGS)

-include $($(dname)_objs:.o=.d)

.PHONY: clean

clean:
	rm -rf $($(dname)_objs) $(patsubst %.o,%.d,$($(dname)_objs))
	@for dir in $(subdirs); do if [ -f $$dir/Makefile ]; then $(MAKE) -C $$dir clean; fi; done
