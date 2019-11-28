.PHONY:all clean
subdirs := $(subst ./,,$(shell find . -maxdepth 5 -type d|sed -e '1d'))
all: $($(dname)_objs) 
	@for dir in $(subdirs); do if [ -f $$dir/Makefile ]; then $(MAKE) -C $$dir; fi; done
clean: $($(dname)_objs)
	@for dir in $(subdirs); do if [ -f $$dir/Makefile ]; then $(MAKE) -C $$dir clean; fi; done
