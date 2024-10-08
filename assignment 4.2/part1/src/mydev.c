#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eggert Kristjan");
MODULE_DESCRIPTION("A simple Linux LKM that accepts a single parameter.");
MODULE_VERSION("0.1");
static char *name = "world";
module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

static int __init mydev_init(void) 
{
    printk(KERN_INFO "hello: Hello %s from the RPi LKM!\n", name);
    return 0;
}

static void __exit mydev_exit(void) 
{
    printk(KERN_INFO "hello: Goodbye %s from the RPi LKM!\n", name);
}

module_init(mydev_init);
module_exit(mydev_exit);