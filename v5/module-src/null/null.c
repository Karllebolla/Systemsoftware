#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benjamin Frank Dominik Riemelmoser");
MODULE_DESCRIPTION("A null Module");
MODULE_SUPPORTED_DEVICE("none");

#define DRIVERNAME "mynull"
#define BASE_MINOR_NUMBER 0
#define MINOR_COUNT 2

static int ownInit(void);
static void ownExit(void);
static int open(struct inode *devicefile, struct file *instance);
static int close(struct inode *devicefile, struct file *instance);
static ssize_t write(struct file *instance, const char *user, size_t count, loff_t *offset);
static ssize_t read(struct file *instance, char *user, size_t count, loff_t *offset);

static dev_t device_number; /* device number */
static struct cdev *driver_object; /* login object */
static struct class *forSysfsClass; /* for entry in sys filesystem*/

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = write,
	.read = read,
	.open = open,
	.release = close
};
/* instance-specific data */
typedef struct {
	int counter;
}instance_data; 

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
        printk(DRIVERNAME ": cdev_alloc failed ...\n");
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
	device_create(forSysfsClass, NULL, device_number, NULL, "%s", DRIVERNAME);
	return 0;
	
free_cdev:
    kobject_put(&driver_object->kobj);
    driver_object = NULL;
free_device_number:
    unregister_chrdev_region( device_number, MINOR_COUNT );
    return -EIO;
}

static void __exit ModExit(void)
{
        printk(DRIVERNAME ": mod_exit called.\n");
		ownExit();
}

static void ownExit(void) {
	printk(DRIVERNAME ": mod_exit called.\n");
	/* delete sys filesystem entry and the devicefile */
	device_destroy(forSysfsClass,device_number);
	class_destroy(forSysfsClass);
	/* sign off driver */
	cdev_del(driver_object);
	unregister_chrdev_region(device_number,MINOR_COUNT);
}

/* driver returns pro driver instance the string "Hello World" */
static int open(struct inode *devicefile, struct file *instance) {
	
	/* create instance-specific counter */
	struct instance_data *idptr;
	idptr = kmalloc(sizeof(instance_data), GFP_KERNEL);
	if (idptr == 0) {
		return -ENOMEM;
	}
	
	printk(DRIVERNAME": open driver\n");
	instance->private_data = (void*)idptr;	
	((instance_data *)instance->private_data)->counter = 0;
	return 0;
}

static int close(struct inode *devicefile, struct file *instance) {
	printk(DRIVERNAME ": close driver.\n");
	/* release instance-specific data */
	if (instance->private_data) {
		kfree(instance->private_data);
	}
	return 0;
}

static ssize_t write(struct file *instance, const char *user, size_t count, loff_t *offset) {
	printk(DRIVERNAME ": get %d byte.\n", count);
	return count;
}

static ssize_t read(struct file *instance, char *user, size_t count, loff_t *offset) {
	char *string ="Hello World";
	size_t to_copy, not_copied, copied;
	int instance_counter = ((instance_data *)instance->private_data)->counter;

	to_copy = strlen(string)-instance_counter;

	if (to_copy > 0) {
		to_copy = min(to_copy, count);
		not_copied = copy_to_user(user,(string + instance_counter),to_copy);
	} else {
		return 0;
	}
	
	copied = to_copy - not_copied;

	((instance_data *)instance->private_data)->counter += copied;
	return copied;
}

module_init(ModInit);
module_exit(ModExit);