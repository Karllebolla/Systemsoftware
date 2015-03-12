#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/timer.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/jiffies.h>
#include <asm/timex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benjamin Frank Dominik Riemelmoser");
MODULE_DESCRIPTION("A timer Module");
MODULE_SUPPORTED_DEVICE("none");

#define DRIVERNAME "TASKLET"
static dev_t device_number; /* device number */
static struct cdev *driver_object; /* login object */
static struct class *forSysfsClass; /* for entry in sys filesystem*/

static struct file_operations fops;
static struct timer_list mytimer;
static unsigned int start = 0;
static unsigned long last_call = 0;

static int ownInit(void);
static void ownExit(void);

static void timer_funktion(unsigned long data) {
	unsigned int end = 0;
	end = get_cycles();
	printk("get cycles: %ld.\n", get_cycles());
	printk("timer called at %ld (%ld); cycles since last call: %u.\n", mytimer.expires, mytimer.expires-last_call, end-start);
    
	last_call = mytimer.expires;
	start = get_cycles();
	/* gleiche mytimer.expires = jiffies + (2*HZ); add_timer( &mytimer ); */
	mod_timer(&mytimer, jiffies + (2*HZ)); 
}

static int __init ModInit(void) {
	printk(DRIVERNAME ": init module.\n");
    return ownInit();
}

static int ownInit(void) {

	init_timer( &mytimer );
	mytimer.function = timer_funktion;
    mytimer.data = 0;
    mytimer.expires = jiffies + (2*HZ); /* 2 second */
	start = get_cycles();
    add_timer( &mytimer );

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
        printk(KERN_ERR DRIVERNAME " driver: cdev_add failed ...\n");
        goto free_cdev;
    }
	
	/* entry in sys filesystem, that udev can create device file */
	forSysfsClass = class_create(THIS_MODULE, DRIVERNAME);
	if(IS_ERR(forSysfsClass)) {
		pr_err(DRIVERNAME " driver: no udev support.\n");
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
	printk(KERN_ALERT ": mod_exit called.\n");
	ownExit();
}

static void ownExit(void) {
	printk(DRIVERNAME ": mod_exit called.\n");
	
	if( timer_pending( &mytimer ) )
        printk("Timer is enabled ...\n");
    if( del_timer_sync( &mytimer ) )
        printk("Timer disabled.\n");
    else
        printk("No Timer enabled.\n");
	
	device_destroy(forSysfsClass,device_number);
	class_destroy(forSysfsClass);
	/* sign off driver */
	cdev_del(driver_object);
	unregister_chrdev_region(device_number,MINOR_COUNT);
}

module_init(ModInit);
module_exit(ModExit);