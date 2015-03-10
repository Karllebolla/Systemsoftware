#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benjamin Frank Dominik Riemelmoser");
MODULE_DESCRIPTION("When you read from this driver you read only zeros.");
MODULE_SUPPORTED_DEVICE("none");

#define DRIVERNAME "ZERO"
#define BASE_MINOR_NUMBER 0
#define MINOR_COUNT 2

static int ownInit(void);
static void ownExit(void);
static int open(struct inode *devicefile, struct file *instanz);
static int close(struct inode *devicefile, struct file *instanz);
static ssize_t driver_read(struct file *devicefile, char *user, size_t count, loff_t *offset);

static dev_t device_number; /* device number */
static struct cdev *driver_object; /* login object */
static struct class *forSysfsClass; /* for entry in sys filesystem*/

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = driver_read,
	.open = open,
	.release = close
};

static int __init ModInit(void)
{
        printk(DRIVERNAME ": init module.\n");
        return ownInit();
}

static int ownInit(void) {

	/* get a device number */
	if( alloc_chrdev_region( &device_number, BASE_MINOR_NUMBER, MINOR_COUNT, DRIVERNAME ) ) {
        return -EIO;
    }
	
    driver_object = cdev_alloc();	/* reserve login object */
	
    if( driver_object==NULL ) {
        printk(KERN_ERR DRIVERNAME ": cdev_alloc failed ...\n");
        goto free_device_number;
    }
    
    driver_object->owner = THIS_MODULE;
	driver_object->ops = &fops;
	
	/* sign in the driver to the kernel */
    if( cdev_add( driver_object, device_number, MINOR_COUNT ) ) {
        printk(KERN_ERR DRIVERNAME  ": cdev_add failed ...\n");
        goto free_cdev;
    }
	
	/* entry in sys filesystem, that udev can create device file */
	forSysfsClass = class_create(THIS_MODULE, DRIVERNAME);
	if(IS_ERR(forSysfsClass)) {
		printk(KERN_ERR DRIVERNAME ": no udev support.\n");
		goto free_cdev;
	}
	device_create(forSysfsClass, NULL, device_number, NULL, "%s0", DRIVERNAME);
	device_create(forSysfsClass, NULL, MKDEV(MAJOR(device_number), MINOR(device_number)+1), NULL, "%s1", DRIVERNAME);
	return 0;
	
free_cdev:
    kobject_put(&driver_object->kobj);
    driver_object = NULL;
free_device_number:
    unregister_chrdev_region( device_number, MINOR_COUNT );
    return -EIO;
}

static void __exit ModExit(void) {
        printk(DRIVERNAME ": mod_exit called.\n");
		ownExit();
}

static void ownExit(void) {
	printk(DRIVERNAME ": sys mod_exit called.\n");
	/* delete sys filesystem entry and the devicefile*/
	device_destroy(forSysfsClass,device_number);
	class_destroy(forSysfsClass);
	/* sign off driver */
	cdev_del(driver_object);
	unregister_chrdev_region(device_number,MINOR_COUNT);
}

static int open(struct inode *devicefile, struct file *instanz) {
	printk(DRIVERNAME ": open driver.\n");
	return 0;
}

static int close(struct inode *devicefile, struct file *instanz) {
	printk(DRIVERNAME": close driver.\n");
	return 0;
}

static ssize_t driver_read(struct file *devicefile, char *user, size_t count, loff_t *offset) {
	char *string ="Hello World\n";
	size_t to_copy, not_copied;
	int minor = iminor(devicefile->f_dentry->d_inode);
	/* with minornumber 0 every read call gives back a 0
	   with minornumber 1  every read call gives back the string "Hello World" */
	if( minor == 0) {
		string = "0";
	}

	
	to_copy = strlen(string)+1;
	to_copy = min(to_copy, count);
	not_copied = copy_to_user(user,string,to_copy);
	
	return to_copy - not_copied;
}

module_init(ModInit);
module_exit(ModExit);