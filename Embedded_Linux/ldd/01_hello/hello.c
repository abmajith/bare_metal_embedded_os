#include <linux/module.h>
#include <linux/init.h>


static int __init hello_init(void)
{
	printk("Linux: Hello from RPI3b Plus!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk("Linux: Bye from RPI3b Plus!\n");
	return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RAZIONE");
MODULE_DESCRIPTION("A Simple Hello RPI3b Plus Linux Kernel Module");