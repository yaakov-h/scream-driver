#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

MODULE_AUTHOR("Yaakov");
MODULE_DESCRIPTION("Scream Device");
MODULE_LICENSE("GPL");

#define DEVICE_NAME "scream"
#define CLASS_NAME "scream_class"

static int major_number;
static struct class* scream_class = NULL;
static struct device* scream_device = NULL;
static struct cdev scream_cdev;

static int scream_device_open(struct inode*, struct file*);
static int scream_device_release(struct inode*, struct file*);
static ssize_t scream_device_read(struct file*, char*, size_t, loff_t*);

static const struct file_operations scream_fops = {
    .owner      = THIS_MODULE,
    .open = scream_device_open,
    .read = scream_device_read,
    .release = scream_device_release,
};

static int __init scream_module_init(void)
{
    int err;
    dev_t dev;

    err = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);

    major_number = MAJOR(dev);

    scream_class = class_create(CLASS_NAME);

    cdev_init(&scream_cdev, &scream_fops);
    scream_cdev.owner = THIS_MODULE;

    cdev_add(&scream_cdev, MKDEV(major_number, 0), 1);

    device_create(scream_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);

    return 0;
}

static void __exit scream_module_exit(void)
{
    device_destroy(scream_class, MKDEV(major_number, 0));

    class_unregister(scream_class);
    class_destroy(scream_class);

    unregister_chrdev_region(MKDEV(major_number, 0), MINORMASK);
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