#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/atomic.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benjamin Frank Dominik Riemelmoser");
MODULE_DESCRIPTION("A openclose Module");
MODULE_SUPPORTED_DEVICE("none");

#define DRIVERNAME "OPENCLOSE"
#define BASE_MINOR_NUMBER 0
#define MINOR_COUNT 2

static int ownInit(void);
static void ownExit(void);
static int open(struct inode *devicefile, struct file *instanz);
static int close(struct inode *devicefile, struct file *instanz);

static atomic_t count;
static dev_t device_number; /* device number */
static struct cdev *driver_object; /* login object */
static struct class *forSysfsClass; /* for entry in sys filesystem*/

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = open,
	.release = close 
};

static int __init ModInit(void) {
        printk(DRIVERNAME ": init module\n");
		atomic_set(&count,-1);
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
        printk(DRIVERNAME ": cdev_add failed ...\n");
        goto free_cdev;
    }
	
	/* entry in sys filesystem, that udev can create device file */
	forSysfsClass = class_create(THIS_MODULE, DRIVERNAME);
	if(IS_ERR(forSysfsClass)) {
		pr_err(DRIVERNAME ": no udev support.\n");
		goto free_cdev;
	}
	device_create(forSysfsClass, NULL, device_number, NULL, "%s0", DRIVERNAME);
	device_create(forSysfsClass, NULL, MKDEV(MAJOR(device_number), MINOR(device_number)+1), NULL, "%s1",DRIVERNAME);
	return 0;
	
free_cdev:
    kobject_put(&driver_object->kobj);
    driver_object = NULL;
free_device_number:
    unregister_chrdev_region( device_number, MINOR_COUNT );
    return -EIO;
}

static int open(struct inode *devicefile, struct file *instanz) {
	printk(DRIVERNAME ": open driver\n");
	/* with minornumber 0 every access is allowed
	   with minornumber 1 only one access is allowed */
	if (MINOR(devicefile->i_rdev) > 0) {
		if (!atomic_inc_and_test(&count)) {
			printk(DRIVERNAME "File is already opened by another process.\n");
			return -EAGAIN;
		}			
	}
	return 0;
}

static int close(struct inode *devicefile, struct file *instanz) {
	printk(DRIVERNAME": close driver.\n");
	if(MINOR(devicefile->i_rdev)> 0) {
		atomic_set(&count,-1);
	}
	return 0;
}

static void __exit ModExit(void) {
        printk(DRIVERNAME ": mod_exit called.\n");
		ownExit();
	
}

static void ownExit(void) {
	printk(DRIVERNAME ": mod_exit called.\n");
	/* delete sys filesystem entry and the devicefile*/
	device_destroy(forSysfsClass,device_number);
	class_destroy(forSysfsClass);
	/* sign off driver */
	cdev_del(driver_object);
	unregister_chrdev_region(device_number,MINOR_COUNT);
}

module_init(ModInit);
module_exit(ModExit);