#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benjamin Frank Dominik Riemelmoser");
MODULE_DESCRIPTION("Own GPIO module");
MODULE_SUPPORTED_DEVICE("none");

#define DRIVERNAME "mygpio"
#define GPIO_BASE_ADR 0x20200000
#define NEEDED_ADR 4096
#define GPIO_COUNT 10
#define GPIO_SIZE 3
#define GPIO_OUT 18			/* GPIO18 */
#define GPIO_IN 25			/* GPIO25 */
#define ADR_GPFSEL(pin) (u32*)(gpio_ptr+(pin/10)) /* 10, with a GPFSEL 10 register will be addressed */
#define ADR_GPSET(pin) (u32*)(gpio_ptr+7+(pin/32)) /* 32, with a  GPSET 32 register will be addressed */
#define ADR_GPCLR(pin) (u32*)(gpio_ptr+10+(pin/32))
#define ADR_GPLEV(pin) (u32*)(gpio_ptr+13+(pin/32))

static dev_t device_number;
static dev_t device_number; /* device number */
static struct cdev *driver_object; /* login object */
static struct class *forSysfsClass; /* for entry in sys filesystem*/

static unsigned *gpio_ptr;

static int ownInit(void);
static void ownExit(void);
static int driver_open(struct inode *devicefile, struct file *instanz);
static int driver_close(struct inode *devicefile, struct file *instanz);
static ssize_t driver_read(struct file *instanz, char *buff, size_t count, loff_t *offset);
static ssize_t driver_write(struct file *instanz,const char *buff, size_t count, loff_t *offset);
static int gpio_init(void);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.write = driver_write,
	.read = driver_read
};

static void set_input(int pin) {
	u32* ptr;
	u32 value, bitmask;
	
	ptr = ADR_GPFSEL(pin);
	value = readl(ptr);
	rmb();
	
	bitmask = 0x7 << ((pin % GPIO_COUNT) *  GPIO_SIZE);
	
	/* invert */
	bitmask ^= 0xFFFFFFFF;
	
	value &= bitmask;
	wmb();
	writel(value, ptr);
}

static void set_output(int pin) {
	u32* ptr;
	u32 value, bitmask;
	
	/* reset bits */
	set_input(pin);
	
	ptr = ADR_GPFSEL(pin);
	value = readl(ptr);
	rmb();
	bitmask = 0x1 << ((pin % GPIO_COUNT) *  GPIO_SIZE);
	value |= bitmask;
	wmb();
	writel(value, ptr);
}

static ssize_t driver_write(struct file *instanz, const char *buff, size_t count, loff_t *offset) {
	u32* ptr;
	u32 bitmask;
	int number;

	kstrtoint_from_user(buff,count,10, &number);

	
	if(number == 1) {
		ptr = ADR_GPSET(GPIO_OUT);
	} else {
		ptr = ADR_GPCLR(GPIO_OUT);
	}
	
	rmb();
	bitmask = 0x1 << GPIO_OUT;
	wmb();
	writel(bitmask, ptr);
	return count;
}

static ssize_t driver_read(struct file *instanz,char *buff, size_t count, loff_t *offset) {
	u32* ptr;
	u32 bitmask, value;
	int notCopied, tocopy;
	char buffer[2];
	
	ptr = ADR_GPLEV(GPIO_IN);
	rmb();
	value = readl(ptr);
	rmb();

	bitmask = 0x1 << GPIO_IN;
	value &= bitmask;

	if(bitmask == value)
		strcpy(buffer, "1");
	else
		strcpy(buffer, "0");

	tocopy = min(count, strlen(buffer)+1);
	notCopied = copy_to_user(buff, buffer, tocopy);

	return tocopy - notCopied;
}

static int driver_open(struct inode *devicefile, struct file *instanz) {

	set_input(GPIO_IN);
	set_output(GPIO_OUT);
	
	return 0;
}

static int driver_close(struct inode *devicefile, struct file *instanze){
	
	return 0;
}

static int gpio_init(void) {
	printk(DRIVERNAME ": gpio init\n");
	gpio_ptr = ioremap(GPIO_BASE_ADR, NEEDED_ADR);
	if (gpio_ptr == NULL) {
		printk(DRIVERNAME " gpio not found\n");
		return -EBUSY;
	}
	
	return 0;
}

static int __init ModInit(void) {
        printk(DRIVERNAME ": init module\n");
        return ownInit();
}

static int ownInit(void) {

	gpio_init();
	/* get a device number */
	if( alloc_chrdev_region( &device_number, 0, 1, DRIVERNAME ) ) {
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
    if( cdev_add( driver_object, device_number, 1 ) ) {
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
    unregister_chrdev_region( device_number, 1 );
    return -EIO;
}


static void __exit ModExit(void) {
        printk(DRIVERNAME ": mod_exit called.\n");
		ownExit();
}

static void ownExit(void) {	
	release_mem_region(GPIO_BASE_ADR, NEEDED_ADR);
	/* delete sys filesystem entry and the devicefile */
	device_destroy(forSysfsClass,device_number);
	class_destroy(forSysfsClass);
	/* sign off driver */
	cdev_del(driver_object);
	unregister_chrdev_region(device_number,1);
}

module_init(ModInit);
module_exit(ModExit);