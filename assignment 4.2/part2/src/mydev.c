#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eggert Kristjan");
MODULE_DESCRIPTION("A simple Linux LKM that accepts a single parameter.");
MODULE_VERSION("0.1");
static char *name = "world";
module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

#define DEVICE_NAME "mydev"
#define BUFFER_SIZE 256

static int majorNumber;
static char message[BUFFER_SIZE] = {0};

static int mydev_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mydev: Device opened\n");
    return 0;
}

static int mydev_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mydev: Device successfully closed\n");
    return 0;
}

ssize_t mydev_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    if (count > BUFFER_SIZE - 1) {
        count = BUFFER_SIZE - 1;
    }

    if (copy_from_user(message, buf, count)) {
        return -EFAULT;
    }

    message[count] = '\0';
    printk(KERN_INFO "mydev: Received %zu bytes: %s\n", count, message);
    return count;
}

struct file_operations fops = {
    .open = mydev_open,
    .write = mydev_write,
    .release = mydev_release,
};

static int __init mydev_init(void) {
    printk(KERN_INFO "mydev: Initializing the mydev LKM\n");

    // Register the character device
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ALERT "mydev: Failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "mydev: Registered with major number %d\n", majorNumber);

    return 0;
}

static void __exit mydev_exit(void) {
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "mydev: Goodbye from the mydev LKM\n");
}

module_init(mydev_init);
module_exit(mydev_exit);