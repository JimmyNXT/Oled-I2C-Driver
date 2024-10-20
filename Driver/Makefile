obj-m += SH1106.o
 
KDIR = /lib/modules/$(shell uname -r)/build
 
 
all:
	make -C $(KDIR)  M=$(shell pwd) modules
 
clean:
	make -C $(KDIR)  M=$(shell pwd) clean

install: all
	insmod SH1106.ko

uninstall:
	rmmod SH1106
