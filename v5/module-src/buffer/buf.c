#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/wait.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/types.h>
#include "util/thread_save_fifo_buffer.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benjamin Frank Dominik Riemelmoser");
MODULE_DESCRIPTION("A FIFO Buffer module.");
MODULE_SUPPORTED_DEVICE("none");

#define DRIVERNAME "FIFO_BUFFER"
#define BUFFER_SIZE 10
static dev_t device_number; /* device number */
static struct cdev *driver_object; /* login object */
static struct class *forSysfsClass; /* for entry in sys filesystem*/


/* waitqueue */
static wait_queue_head_t wait_queue_read;
static wait_queue_head_t wait_queue_write;


static int ownInit(void);
static void ownExit(void);
static int open(struct inode *devicefile, struct file *instanz);
static int close(struct inode *devicefile, struct file *instanz);
static ssize_t read(struct file *instanz, char *buff, size_t count, loff_t *offset);
static ssize_t write(struct file *instanz,const char *buff, size_t count, loff_t *offset); 

fifoBuffer buffer;

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = open,
	.release = close,
	.write = write,
	.read = read
};


static int open(struct inode *devicefile, struct file *instanz) {
	return 0;
}
static int close(struct inode *devicefile, struct file *instanz) {
	return 0;
}

static ssize_t write(struct file *instanz, const char *buff, size_t count, loff_t *offset) {	
	int notCopied, copied;
	char kernelBuffer[count];
	if(instanz->f_flags & O_NONBLOCK && (is_fifo_buffer_full(&buffer) == 1)) {
		printk(DRIVERNAME " non-blocking write.\n");
		return -EAGAIN;
	}
	if(!(instanz->f_flags & O_NONBLOCK)) {
		printk(DRIVERNAME " blocking mode write.\n");
		wait_event_interruptible(wait_queue_write,(is_fifo_buffer_full(&buffer) == 0));
		printk(DRIVERNAME " buffer is not full.\n");
	}

	notCopied = copy_from_user(kernelBuffer, buff, count);
	copied = count - notCopied;
	
	wake_up_interruptible(&wait_queue_read);
	return write_fifo_buffer(&buffer, kernelBuffer, copied);
}

static ssize_t read(struct file *instanz,char *buff, size_t count, loff_t *offset) {

	int copied,notCopied;
	char tmp[count];
	if(instanz->f_flags & O_NONBLOCK && (is_fifo_buffer_empty(&buffer) == 1)) {
		printk(DRIVERNAME " non-blocking read.\n");
		return -EAGAIN;
	}
	if(!(instanz->f_flags & O_NONBLOCK)){
		printk(DRIVERNAME " blocking mode read.\n");
		wait_event_interruptible(wait_queue_read, is_fifo_buffer_empty(&buffer) != 1);
		printk(DRIVERNAME " buffer is not empty.\n");
	}
	copied = read_fifo_buffer(&buffer, tmp,count);
	notCopied = copy_to_user(buff,tmp,copied);

	wake_up_interruptible(&wait_queue_write);	
	return (copied-notCopied);
}


static int __init ModInit(void) {
        printk(DRIVERNAME " init module\n");
        return ownInit();
}

static int ownInit(void) {
	if(init_fifo_buffer(BUFFER_SIZE, &buffer) != 0) {
		printk(KERN_ERR DRIVERNAME " could not initialize fifo buffer.\n");
		return -ENOSPC;
	}
	/* waitqueue initialisation */
	init_waitqueue_head(&wait_queue_read);
	init_waitqueue_head(&wait_queue_write);
	
	/* get a device number */
	if( alloc_chrdev_region( &device_number, 0, 1, DRIVERNAME ) ) {
        return -EIO;
    }
	
    driver_object = cdev_alloc();	/* reserve login object */
	
    if( driver_object==NULL ) {
        printk(KERN_ERR DRIVERNAME " cdev_alloc failed ...\n");
        goto free_device_number;
    }
    driver_object->owner = THIS_MODULE;
	driver_object->ops = &fops;
	
	/* sign in the driver to the kernel */
    if( cdev_add( driver_object, device_number, 1 ) ) {
        printk(DRIVERNAME "  cdev_add failed ...\n");
        goto free_cdev;
    }
	
	/* entry in sys filesystem, that udev can create device file */
	forSysfsClass = class_create(THIS_MODULE, DRIVERNAME);
	if(IS_ERR(forSysfsClass)) {
		pr_err(DRIVERNAME "  no udev support\n");
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
	if(destroy_fifo_buffer(&buffer) != 0) {
		printk(DRIVERNAME " fifo buffer is already destroyed.\n");
	}
	printk(DRIVERNAME ": mod_exit called\n");
	/* delete sys filesystem entry and the devicefile*/
	device_destroy(forSysfsClass,device_number);
	class_destroy(forSysfsClass);
	/* sign off driver */
	cdev_del(driver_object);
	unregister_chrdev_region(device_number,1);
}

module_init(ModInit);
module_exit(ModExit);