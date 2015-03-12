export ARCH = arm
export CROSS_COMPILE = /group/labworks/toolchain/buildroot.git/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabihf-
LINUXVERSION = 3.17.2

ifneq ($(KERNELRELEASE),)
obj-m   := timer.o # generierung als Modultreiber obj-m

else
#KDIR    := /lib/modules/$(shell uname -r)/build # buildroot/output
KDIR	:= /group/SYSO_WS1415_9/syso_ws1415_9/v3/buildroot/output/build/linux-$(LINUXVERSION) 
PWD     := $(shell pwd)

.PHONY:install clean all

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	
install:
	cp *.ko $(MODULEDIR)
		
clean:
	rm -rf *.ko *.o .*.cmd .tmp_versions Module.symvers modules.order *.mod.c
endif
