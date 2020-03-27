include ../src/Make.defines

PATCH_LEVEL = $(shell if [ -e patch_level ]; then cat patch_level;fi)
COMMIT = $(shell git rev-list HEAD|head -1|cut -c 1-6)
RPM_TOP_DIR = $(shell rpm -E %{_topdir})
NEWVERSION = $(shell [ ! -z "`find version -newer $(ROOT)/include/fpga_agentd.h`" ] && cat version)
VERSION = $(shell cat version)

ifneq ($(PATCH_LEVEL),)
    RPM_RELEASE = $(shell git branch --no-color 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/\1/' -e 's/-/_/g')_$(PATCH_LEVEL)_$(COMMIT)
else
    RPM_RELEASE = $(shell git branch --no-color 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/\1/' -e 's/-/_/g')_$(COMMIT)
endif

PROGRAM = fpga_agentd

BINARY = fpga_agent

.PHONY: all install clean

all: $(BINARY)
	cp $(BINARY) $(BINARY).strip
	strip $(BINARY).strip
    
$(BINARY): build
	$(LINKCC) $(wildcard obj/*.o) -o $@ $(CFLAGS)

build:
	@if [ ! -z "$(NEWVERSION)" ]; then \
        sed -i 's/FPGA_AGENTD_VERSION.*/FPGA_AGENTD_VERSION "$(NEWVERSION)"/' $(ROOT)/include/fpga_agentd.h ; fi
	@$(MAKE) -C $(ROOT)/src

install:
	mkdir -p $(BUILD_PATH)/usr/bin $(BUILD_PATH)/etc/init.d $(BUILD_PATH)/usr/lib64
	cp -af $(ROOT)/build/$(BINARY) $(BUILD_PATH)/usr/bin
	cp -af $(ROOT)/conf/$(PROGRAM).ini $(BUILD_PATH)/etc
	cp -af $(ROOT)/conf/$(PROGRAM) $(BUILD_PATH)/etc/init.d/

clean:
	@$(MAKE) -C $(ROOT)/src clean
	rm -rf $(BINARY) $(BINARY).strip $(OBJS_DIR)

rpm:
	@echo [RPM] ;\
sed -e "s/@VERSION@/$(VERSION)/g" -e "s/@RELEASE@/$(RPM_RELEASE)/g" $(PROGRAM).spec.tmp > ${RPM_TOP_DIR}/SPECS/$(PROGRAM).spec ;\
cp -a -r ${ROOT} /tmp/$(PROGRAM)-$(VERSION) ;\
cp -rf $(ROOT)/conf/$(PROGRAM) ${RPM_TOP_DIR}/SOURCES/;\
cd /tmp ;\
tar cvzf ${RPM_TOP_DIR}/SOURCES/$(PROGRAM)-$(VERSION).tar.gz $(PROGRAM)-$(VERSION) ;\
rm -rf $(PROGRAM)-$(VERSION) ;\
rpmbuild -bb ${RPM_TOP_DIR}/SPECS/$(PROGRAM).spec
