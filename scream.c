#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/stat.h>

MODULE_AUTHOR("Yaakov");
MODULE_DESCRIPTION("Scream Device");
MODULE_LICENSE("GPL");

#define DEVICE_NAME "scream"
#define CLASS_NAME "scream_class"

static int scream_device_open(struct inode*, struct file*);
static int scream_device_release(struct inode*, struct file*);
static ssize_t scream_device_read(struct file*, char*, size_t, loff_t*);

static const struct file_operations scream_fops = {
    .owner = THIS_MODULE,
    .open = scream_device_open,
    .read = scream_device_read,
    .release = scream_device_release,
};

static struct miscdevice scream_misc_device  = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "scream",
    .fops = &scream_fops,
    .mode = S_IRUGO,
};

static int __init scream_module_init(void) {
    printk(KERN_INFO "[scream] loading...\n");
    int ret = misc_register(&scream_misc_device);
    return ret;
}

static void __exit scream_module_exit(void) {
    printk(KERN_INFO "[scream] exiting...\n");

    misc_deregister(&scream_misc_device);

    printk(KERN_INFO "[scream] done.\n");
}

static int scream_device_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[scream] Device open\n");
    return 0;
}

static int scream_device_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[scream] Device close\n");
    return 0;
}

static ssize_t scream_device_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    uint8_t rand;

    for (size_t i = 0; i < count; i++)
    {
        get_random_bytes(&rand, sizeof(rand));

        char value;

        if (rand > 200) {
            value = 'A';
        } else {
            value = 'a';
        }

        if (copy_to_user(buf + i, &value, 1)) {
            return -EFAULT;
        }
    }

    return count;
}

module_init(scream_module_init);
module_exit(scream_module_exit);