obj-m := pwmDriverPin18.o

KDIR = /usr/src/linux-headers-$(shell uname -r)
all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	gcc -o reader tempReader.c
	sudo insmod pwmDriverPin18.ko
	sudo chmod 777 /sys/pwm/pwm
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	sudo rmmod  pwmDriverPin18
	sudo rm reader
	sudo rm /dev/pwmDriverPin18
	
