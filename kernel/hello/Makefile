obj-m += hello.o                         #由此目标文件生成模块 
CURRENT_PATH := $(shell pwd)             #当前所在的路径
LINUX_KERNEL := $(shell uname -r)        #内核的版本号

#内核代码所在的路径    
#LINUX_KERNEL_PATH := /usr/src/linux-headers-$(LINUX_KERNEL)
LINUX_KERNEL_PATH := /root/rpmbuild/BUILD/kernel-3.10.0-862.el7/linux-3.10.0-862.el7.x86_64

#编译
all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules   

#清除编译生成的文件
clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean
