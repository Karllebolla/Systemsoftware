#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/completion.h>
#include <linux/sched.h>
#include <linux/kthread.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benjamin Frank Dominik Riemelmoser");
MODULE_DESCRIPTION("A Kernel Thread Module");
MODULE_SUPPORTED_DEVICE("none");

#define DRIVERNAME "KERNELTHREADS"
static dev_t device_number; /* device number */
static struct cdev *driver_object; /* login object */
static struct class *forSysfsClass; /* for entry in sys filesystem*/

static struct file_operations fops;
static struct task_struct *thread_id;
static wait_queue_head_t wq;
static DECLARE_COMPLETION( on_exit );

static int ownInit(void);
static void ownExit(void);


static int kernel_thread_function( void *data ) {
    unsigned long timeout;
    int i;

    allow_signal( SIGTERM ); 
    for( i=0; i<10; i++ ) {
        timeout=2*HZ; /* wait 2 seconds */
        timeout=wait_event_interruptible_timeout(wq, (timeout==0), timeout);
        printk("kernel_thread_function: woke up ...\n");
        if( timeout==-ERESTARTSYS ) {
            printk("got signal, break\n");
            break;
        }
    }
    thread_id = 0;
    complete_and_exit( &on_exit, 0 );
}

static int __init ModInit(void) {
        printk(DRIVERNAME": init module.\n");
        return ownInit();
}

static int ownInit(void) {

	/* waitqueue initialisation and create a kernel thread */
	init_waitqueue_head(&wq);
    thread_id = kthread_create(kernel_thread_function, NULL, "mykthread" );
    if( thread_id == 0 )
        return -EIO;
	wake_up_process( thread_id );
	
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
		pr_err(DRIVERNAME "driver: no udev support\n");
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

	printk(DRIVERNAME": mod_exit called\n");
	/* kill kernel thread */
	if( thread_id ) kill_pid( task_pid(thread_id), SIGTERM, 1 );
    wait_for_completion( &on_exit );

	/* delete sys filesystem entry and the devicefile*/
	device_destroy(forSysfsClass,device_number);
	class_destroy(forSysfsClass);
	/* sign off driver */
	cdev_del(driver_object);
	unregister_chrdev_region(device_number,1);
}

module_init(ModInit);
module_exit(ModExit);