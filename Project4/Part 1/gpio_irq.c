#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eggert");
MODULE_DESCRIPTION("A kernel module to copy the input state from GPIO12 to GPIO17 on rising and falling edges");

#define GPIO_INPUT 524   // Input GPIO pin
#define GPIO_OUTPUT 529   // Output GPIO pin

static unsigned int irq_number;

static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    int value = gpio_get_value(GPIO_INPUT);
    gpio_set_value(GPIO_OUTPUT, value);
    printk(KERN_INFO "gpio_irq: Input value: %d copied to GPIO %d\n", value, GPIO_OUTPUT);
    return IRQ_HANDLED;
}

static int __init ModuleInit(void) {
    printk(KERN_INFO "gpio_copy: Loading module...\n");

    if (gpio_request(GPIO_INPUT, "gpio_input") || gpio_request(GPIO_OUTPUT, "gpio_output")) {
        printk(KERN_ERR "gpio_copy: Error allocating GPIO pins\n");
        gpio_free(GPIO_INPUT);
        gpio_free(GPIO_OUTPUT);
        return -EBUSY;
    }

    if (gpio_direction_input(GPIO_INPUT) || gpio_direction_output(GPIO_OUTPUT, 0)) {
        printk(KERN_ERR "gpio_copy: Error setting GPIO pin directions\n");
        gpio_free(GPIO_INPUT);
        gpio_free(GPIO_OUTPUT);
        return -EINVAL;
    }

    irq_number = gpio_to_irq(GPIO_INPUT);
    if (request_irq(irq_number, gpio_irq_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "gpio_copy_irq", NULL)) {
        printk(KERN_ERR "gpio_copy: Error requesting IRQ number: %d\n", irq_number);
        gpio_free(GPIO_INPUT);
        gpio_free(GPIO_OUTPUT);
        return -EIO;
    }

    printk(KERN_INFO "gpio_copy: Module loaded! GPIO %d mapped to IRQ %d\n", GPIO_INPUT, irq_number);
    return 0;
}

static void __exit ModuleExit(void) {
    printk(KERN_INFO "gpio_copy: Unloading module...\n");
    free_irq(irq_number, NULL);
    gpio_free(GPIO_INPUT);
    gpio_free(GPIO_OUTPUT);
}

module_init(ModuleInit);
module_exit(ModuleExit);