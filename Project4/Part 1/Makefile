# Makefile for gpio_irq module

obj-m += gpio_irq.o  # Specify the object file for the module

# Default target
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# Clean target
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean