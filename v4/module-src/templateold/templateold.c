#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define DRIVERNAME "TEMPLATE_OLD"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benjamin Frank Dominik Riemelmoser");
MODULE_DESCRIPTION("Just a Module Template - classic art");
MODULE_SUPPORTED_DEVICE("none");

static int majorNumber;
static struct file_operations fops;		/* driver object */


static int __init ModInit(void) {
        printk(DRIVERNAME ": init module.\n");
		/* get a device number */
		if((majorNumber = register_chrdev(0,"TemplateDriver", &fops)) == 0) { 
			printk(KERN_ALERT  DRIVERNAME": can't registers.\n");
			return -EIO;
		}
		printk(KERN_ALERT DRIVERNAME ": successful registered.\n");
        return 0;
}


static void __exit ModExit(void) {
        printk(DRIVERNAME ": mod_exit called.\n");
		unregister_chrdev(majorNumber,"TemplateDriver");	/* sign off driver */
}

module_init(ModInit);
module_exit(ModExit);
