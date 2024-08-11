#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

MODULE_AUTHOR("Yaakov");
MODULE_DESCRIPTION("Scream Device");
MODULE_LICENSE("GPL");

#define DEVICE_NAME "scream_device"
#define CLASS_NAME "scream_class"

static int major_number;
static struct class* scream_class = NULL;
static struct device* scream_device = NULL;
static struct cdev scream_cdev;

static int scream_device_open(struct inode*, struct file*);
static int scream_device_release(struct inode*, struct file*);
static ssize_t scream_device_read(struct file*, char*, size_t, loff_t*);
static ssize_t scream_device_write(struct file*, const char*, size_t, loff_t*);

static struct file_operations fops = {
    .open = scream_device_open,
    .read = scream_device_read,
    .write = scream_device_write,
    .release = scream_device_release,
};

static int __init scream_module_init(void) {
    printk(KERN_INFO "[scream] loading...\n");

    if (alloc_chrdev_region(&major_number, 0, 1, DEVICE_NAME) < 0) {
        printk(KERN_ALERT "[scream] Failed to register a major number.\n");
        return major_number;
    }

    scream_class = class_create(CLASS_NAME);
    if (IS_ERR(scream_class)) {
        unregister_chrdev_region(major_number, 1);
        printk(KERN_ALERT "[scream] Failed to register device class\n");
        return PTR_ERR(scream_class);
    }
    
    scream_device = device_create(scream_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(scream_device)) {
        class_destroy(scream_class);
        unregister_chrdev_region(major_number, 1);
        printk(KERN_ALERT "[scream] Failed to create the device\n");
        return PTR_ERR(scream_device);
    }

    printk(KERN_INFO "[scream] %s/%s device registered.", CLASS_NAME, DEVICE_NAME);

    cdev_init(&scream_cdev, &fops);
    int add_result = cdev_add(&scream_cdev, MKDEV(major_number, 0), 1);
    if (add_result < 0) {
        device_destroy(scream_class, MKDEV(major_number, 0));
        class_destroy(scream_class);
        unregister_chrdev_region(major_number, 1);
        printk(KERN_ALERT "[scream] Failed to add cdev: %d\n", add_result);
        return -1;
    }

    printk(KERN_INFO "[scream] loaded.\n");
    return 0;
}

static void __exit scream_module_exit(void) {
    printk(KERN_INFO "[scream] exiting...\n");

    cdev_del(&scream_cdev);
    device_destroy(scream_class, MKDEV(major_number, 0));
    class_destroy(scream_class);
    unregister_chrdev_region(major_number, 1);

    printk(KERN_INFO "[scream] done.\n");
}

static int scream_device_open(struct inode* node, struct file* file)
{
    printk(KERN_INFO "[scream] opening device.\n");
    return 0;
}

static int scream_device_release(struct inode* node, struct file* file)
{
    printk(KERN_INFO "[scream] releasing device.\n");
    return 0;
}

static ssize_t scream_device_read(struct file* file, char* __user user_buffer, size_t size, loff_t* offset)
{
    printk(KERN_INFO "[scream] reading from device.\n");
    return 0;
}

static ssize_t scream_device_write(struct file* file, const char* __user user_buffer, size_t size, loff_t* offset)
{
    printk(KERN_INFO "[scream] writing to device.\n");
    return 0;
}

module_init(scream_module_init);
module_exit(scream_module_exit);