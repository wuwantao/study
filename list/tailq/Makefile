CFLAGS +=  
cur_path = 
define get_template_path
$(if $(root_dir),,\
	$(eval tmp_str =$(cur_path)../)\
	$(eval root_dir = $(shell find $(tmp_str) -iname template.mk|head -n 1))\
	$(eval cur_path=$(tmp_str))\
	$(if $(root_dir),,$(call get_template_path))\
	)
endef


ifeq ($(root_dir),) 
$(call get_template_path)
	include $(root_dir)
else
	include $(root_dir)/template.mk
endif