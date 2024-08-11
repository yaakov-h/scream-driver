#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

MODULE_AUTHOR("Yaakov");
MODULE_DESCRIPTION("Scream Device");
MODULE_LICENSE("GPL");

static int __init scream_module_init(void) {
 printk(KERN_INFO "Module init");
 return 0;
}

static void __exit scream_module_exit(void) {
 printk(KERN_INFO "Module exit");
}

module_init(scream_module_init);
module_exit(scream_module_exit);