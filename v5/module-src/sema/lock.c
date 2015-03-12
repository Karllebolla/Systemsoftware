#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/semaphore.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benjamin Frank Dominik Riemelmoser");
MODULE_DESCRIPTION("Just a spinlock and semaphore module");
MODULE_SUPPORTED_DEVICE("none");

#define DRIVERNAME "LOCK"
static dev_t device_number; /* device number */
static struct cdev *driver_object; /* login object */
static struct class *forSysfsClass; /* for entry in sys filesystem*/

static int ownInit(void);
static void ownExit(void);
static int open(struct inode *devicefile, struct file *instanz);
static int close(struct inode *devicefile, struct file *instanz);

/* semaphore */
static struct semaphore s;

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = open,
	.release = close
};

static int open(struct inode *devicefile, struct file *instanz) {
	int retSema;

	while(1) {
		retSema = down_trylock(&s);
		if(retSema != 0) {
			printk(DRIVERNAME ": semaphore lock not available. Wait.\n");
			msleep(200);
		} else {
			break;
		}
	}

	printk(DRIVERNAME ": semaphore lock available.\n");	
	ssleep(3);
	printk(DRIVERNAME ": open driver after 3 seconds.\n");
	up(&s);
	return 0;
}

static int close(struct inode *devicefile, struct file *instanze){
	return 0;
}

static int __init ModInit(void) {
        printk(DRIVERNAME ": init module.\n");
        return ownInit();
}

static int ownInit(void) {

	sema_init(&s,1);
	/* get a device number */
	if( alloc_chrdev_region( &device_number, 0, 1, DRIVERNAME ) ) {
        return -EIO;
    }
	
    driver_object = cdev_alloc();	/* reserve login object */
	
    if( driver_object==NULL ) {
        printk(DRIVERNAME ": cdev_alloc failed ...\n");
        goto free_device_number;
    }
    driver_object->owner = THIS_MODULE;
	driver_object->ops = &fops;
	
	/* sign in the driver to the kernel */
    if( cdev_add( driver_object, device_number, 1 ) ) {
        printk(DRIVERNAME ": cdev_add failed ...\n");
        goto free_cdev;
    }
	
	/* entry in sys filesystem, that udev can create device file */
	forSysfsClass = class_create(THIS_MODULE, DRIVERNAME);
	if(IS_ERR(forSysfsClass)) {
		pr_err(DRIVERNAME": no udev support.\n");
		goto free_cdev;
	}
	device_create(forSysfsClass, NULL, device_number, NULL, "%s", DRIVERNAME);
	return 0;
	
free_cdev:
    kobject_put(&driver_object->kobj);
    driver_object = NULL;
free_device_number:
    unregister_chrdev_region( device_number, 1 );
    return -EIO;
}

static void __exit ModExit(void) {
        printk(DRIVERNAME ": mod_exit called.\n");
		ownExit();
}

static void ownExit(void) {
	printk(DRIVERNAME": sys mod_exit called.\n");
	/* delete sys filesystem entry and the devicefile */
	device_destroy(forSysfsClass,device_number);
	class_destroy(forSysfsClass);
	/* sign off driver */
	cdev_del(driver_object);
	unregister_chrdev_region(device_number,1);
}

module_init(ModInit);
module_exit(ModExit);