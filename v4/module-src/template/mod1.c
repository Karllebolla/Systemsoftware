#include <linux/init.h>
#include <linux/module.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benjamin Frank Dominik Riemelmoser");
MOUDLE_DESCRITPION("Template Module ");
MODULE_SUPPORTED_DEVICE("none");

static int __init ModInit(void) {
        printk(KERN_ALERT "Hello, world.\n");
        return 0;
}

static void __exit ModExit(void) {
        printk(KERN_ALERT "Goodbye, cruel world.\n");
}

module_init(ModInit);
module_exit(ModExit);