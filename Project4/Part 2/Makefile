# Name of the module (without the .ko extension)
obj-m := kernel_encoder.o

# Specify the path to the kernel headers
KDIR := /lib/modules/$(shell uname -r)/build

# The default target to build the kernel module
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Clean up the build files
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean