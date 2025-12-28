#include <linux/module.h>
#include <linux/fs.h>     // character device driver
#include <linux/device.h> // for dev creation at ./dev
#include <linux/cdev.h>   // for cdev structure
#include <linux/uaccess.h> // for copy_to/from_user functions
#include <linux/string.h> // for strlen

#define DEVICE_NAME "razione_char"
#define CLASS_NAME  "razione_class"
#define BUFFER_SIZE 2048 // 2KB

static int majorNumber;
static char razione_kernel_buffer[BUFFER_SIZE]; // Kernel Mmm storage represnts the hardware for razione char
static short size_of_message;

static struct class*  razioneClass = NULL;
static struct device* razioneDevice = NULL;


static int razione_open(struct inode *inodep, struct file *filep)
{
	pr_info("RazioneChar: Device has been opened\n");
	return 0;
}

static int razione_release(struct inode *inodep, struct file *filep)
{
	pr_info("RazioneChar: Device file Successfully closed\n");
	return 0;
}

static ssize_t razione_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int error_count = 0;
	if (*offset > 0) return 0;
	error_count = copy_to_user(buffer, razione_kernel_buffer, size_of_message);

	if (error_count == 0)
	{
		pr_info("RazioneChar: Sent %d characters to the user\n", size_of_message);
		*offset += size_of_message;
		return size_of_message;
	} else 
	{
		pr_err("RazioneChar: Failed to send characters to user\n");
		return -EFAULT;
	}
}

static ssize_t razione_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	size_t copy_len = (len < BUFFER_SIZE) ? len : BUFFER_SIZE - 1;

	if (copy_from_user(razione_kernel_buffer, buffer, copy_len))
	{
		return -EFAULT;
	}
	razione_kernel_buffer[copy_len] = '\0';
	size_of_message = copy_len;
	pr_info("RazioneChar: Received %zu characters from the user\n", len);
	return len;
}

static struct file_operations fops_razione = {
        .open = razione_open,
        .release = razione_release,
	.read = razione_read,
	.write = razione_write,
};


static int __init razione_char_init(void)
{
	// dynamically allocate a major number and file operations in one go, older method but simpler
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops_razione);
	if (majorNumber < 0)
	{	
		pr_err("RazioneChar: Failed to register a major number\n");
		return majorNumber;
	}
	pr_info("RazioneChar: Registered with major number %d\n", majorNumber);
	
	// register device class
	razioneClass = class_create(CLASS_NAME);
	if (IS_ERR(razioneClass))
	{
		unregister_chrdev(majorNumber, DEVICE_NAME);
		return PTR_ERR(razioneClass);
	}

	razioneDevice = device_create(razioneClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(razioneDevice))
	{
		class_destroy(razioneClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		return PTR_ERR(razioneDevice);
	}
	pr_info("RazioneChar: Device created correctly in /dev/%s\n", DEVICE_NAME);
	return 0;
}

static void __exit razione_char_exit(void)
{
	device_destroy(razioneClass, MKDEV(majorNumber, 0));
	class_unregister(razioneClass);
	class_destroy(razioneClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
	pr_info("RazioneChar Device: successfully unloaded!\n");
}



module_init(razione_char_init);
module_exit(razione_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RAZIONE");
MODULE_DESCRIPTION("Test Char Device with older methods!\n");
