#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arnar");
MODULE_DESCRIPTION("Encoder counter using GPIO interrupts with speed calculation");

#define ENCODER_GPIO_PIN 529 // GPIO 17
#define PULSES_PER_REVOLUTION 1400  // Set this to your encoder's pulses per revolution
#define FIXED_POINT_FACTOR 1000  // Factor to scale values to simulate fixed-point

static int irq_number;
static volatile int encoder_count = 0;  // Encoder pulse count
static volatile int previous_encoder_count = 0; // Previous encoder count for speed calculation
static volatile int encoder_speed = 0;  // Speed in RPM in fixed-point format (multiplied by FIXED_POINT_FACTOR)

static dev_t dev_number;
static dev_t dev_number_speed;
static struct cdev encoder_cdev;
static struct cdev encoder_cdev_speed;
static struct class *encoder_class;
static struct device *encoder_device;
static struct device *encoder_device_speed;
static struct timer_list encoder_timer;

// Interrupt handler for encoder pulse
static irqreturn_t encoder_isr(int irq, void *dev_id) {
    encoder_count++;
    return IRQ_HANDLED;
}

// Timer callback to calculate the speed based on pulse count using fixed-point arithmetic
void timer_callback(struct timer_list *timer) {
    int pulse_difference = encoder_count - previous_encoder_count;
    previous_encoder_count = encoder_count;

    // Calculate speed in fixed-point (pulses per second)
    encoder_speed = (pulse_difference)*10;  // assuming 100ms interval

    // Restart the timer
    mod_timer(&encoder_timer, jiffies + msecs_to_jiffies(100));
}

// Function to expose encoder pulse count to user space
static ssize_t read_encoder_count(struct file *file, char __user *user_buffer, size_t count, loff_t *pos) {
    char buffer[16];
    int len;

    // Format the current encoder count into a string
    len = snprintf(buffer, sizeof(buffer), "%d\n", encoder_count);

    // Copy the data to user space
    if (copy_to_user(user_buffer, buffer, len)) {
        return -EFAULT;
    }
    return len;
}

// Function to expose encoder speed to user space using fixed-point arithmetic
static ssize_t read_encoder_speed(struct file *file, char __user *user_buffer, size_t count, loff_t *pos) {
    char buffer[16];
    int len;

    // Convert fixed-point speed to integer (divide by FIXED_POINT_FACTOR)
    len = snprintf(buffer, sizeof(buffer), "%d\n", encoder_speed);

    // Copy the data to user space
    if (copy_to_user(user_buffer, buffer, len)) {
        return -EFAULT;
    }
    return len;
}

// Reset the encoder count when a write operation is performed
static ssize_t write_encoder_reset(struct file *file, const char __user *user_buffer, size_t count, loff_t *pos) {
    encoder_count = 0;
    previous_encoder_count = 0;
    printk(KERN_INFO "Encoder count reset to 0\n");
    return count;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = read_encoder_count,
    .write = write_encoder_reset,  // to reset encoder count
};

static const struct file_operations fops_speed = {
    .owner = THIS_MODULE,
    .read = read_encoder_speed,
};

// Module init function
static int __init encoder_init(void) {
    int ret;

    printk(KERN_INFO "Initializing encoder module...\n");

    // Allocate device number for encoder count
    ret = alloc_chrdev_region(&dev_number, 0, 1, "encoder");
    if (ret) {
        printk(KERN_ERR "Failed to allocate device number\n");
        return ret;
    }

    // Allocate device number for encoder speed
    ret = alloc_chrdev_region(&dev_number_speed, 0, 1, "encoder_speed");
    if (ret) {
        printk(KERN_ERR "Failed to allocate speed device number\n");
        unregister_chrdev_region(dev_number, 1);
        return ret;
    }

    // Initialize character devices
    cdev_init(&encoder_cdev, &fops);
    encoder_cdev.owner = THIS_MODULE;

    cdev_init(&encoder_cdev_speed, &fops_speed);
    encoder_cdev_speed.owner = THIS_MODULE;

    // Add character devices
    ret = cdev_add(&encoder_cdev, dev_number, 1);
    if (ret) {
        printk(KERN_ERR "Failed to add character device\n");
        unregister_chrdev_region(dev_number, 1);
        return ret;
    }

    ret = cdev_add(&encoder_cdev_speed, dev_number_speed, 1);
    if (ret) {
        printk(KERN_ERR "Failed to add speed character device\n");
        unregister_chrdev_region(dev_number_speed, 1);
        unregister_chrdev_region(dev_number, 1);
        return ret;
    }

    // Create class and device for encoder count
    encoder_class = class_create("encoder");
    if (IS_ERR(encoder_class)) {
        printk(KERN_ERR "Failed to create class\n");
        cdev_del(&encoder_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(encoder_class);
    }

    encoder_device = device_create(encoder_class, NULL, dev_number, NULL, "encoder");
    if (IS_ERR(encoder_device)) {
        printk(KERN_ERR "Failed to create encoder device\n");
        class_destroy(encoder_class);
        cdev_del(&encoder_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(encoder_device);
    }

    // Create device for encoder speed
    encoder_device_speed = device_create(encoder_class, NULL, dev_number_speed, NULL, "encoder_speed");
    if (IS_ERR(encoder_device_speed)) {
        printk(KERN_ERR "Failed to create encoder speed device\n");
        device_destroy(encoder_class, dev_number);
        class_destroy(encoder_class);
        cdev_del(&encoder_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(encoder_device_speed);
    }

    // Request GPIO
    ret = gpio_request(ENCODER_GPIO_PIN, "encoder_pin");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO %d\n", ENCODER_GPIO_PIN);
        device_destroy(encoder_class, dev_number_speed);
        device_destroy(encoder_class, dev_number);
        class_destroy(encoder_class);
        cdev_del(&encoder_cdev);
        unregister_chrdev_region(dev_number, 1);
        return ret;
    }

    // Set GPIO direction to input
    ret = gpio_direction_input(ENCODER_GPIO_PIN);
    if (ret) {
        printk(KERN_ERR "Failed to set GPIO direction for GPIO %d\n", ENCODER_GPIO_PIN);
        gpio_free(ENCODER_GPIO_PIN);
        device_destroy(encoder_class, dev_number_speed);
        device_destroy(encoder_class, dev_number);
        class_destroy(encoder_class);
        cdev_del(&encoder_cdev);
        unregister_chrdev_region(dev_number, 1);
        return ret;
    }

    // Get IRQ number for GPIO pin
    irq_number = gpio_to_irq(ENCODER_GPIO_PIN);
    if (irq_number < 0) {
        printk(KERN_ERR "Failed to get IRQ number for GPIO %d\n", ENCODER_GPIO_PIN);
        gpio_free(ENCODER_GPIO_PIN);
        device_destroy(encoder_class, dev_number_speed);
        device_destroy(encoder_class, dev_number);
        class_destroy(encoder_class);
        cdev_del(&encoder_cdev);
        unregister_chrdev_region(dev_number, 1);
        return irq_number;
    }

    // Request IRQ
    ret = request_irq(irq_number, encoder_isr, IRQF_TRIGGER_RISING, "encoder_irq", NULL);
    if (ret) {
        printk(KERN_ERR "Failed to request IRQ for GPIO %d\n", ENCODER_GPIO_PIN);
        gpio_free(ENCODER_GPIO_PIN);
        device_destroy(encoder_class, dev_number_speed);
        device_destroy(encoder_class, dev_number);
        class_destroy(encoder_class);
        cdev_del(&encoder_cdev);
        unregister_chrdev_region(dev_number, 1);
        return ret;
    }

    // Initialize and start the timer
    timer_setup(&encoder_timer, timer_callback, 0);
    mod_timer(&encoder_timer, jiffies + msecs_to_jiffies(100));

    printk(KERN_INFO "Encoder module loaded successfully\n");
    return 0;
}

// Module exit function
static void __exit encoder_exit(void) {
    free_irq(irq_number, NULL);
    del_timer(&encoder_timer);  // Remove the timer
    gpio_free(ENCODER_GPIO_PIN);
    device_destroy(encoder_class, dev_number_speed);
    device_destroy(encoder_class, dev_number);
    class_destroy(encoder_class);
    cdev_del(&encoder_cdev_speed);
    cdev_del(&encoder_cdev);
    unregister_chrdev_region(dev_number_speed, 1);
    unregister_chrdev_region(dev_number, 1);
    printk(KERN_INFO "Encoder module unloaded\n");
}

module_init(encoder_init);
module_exit(encoder_exit);
