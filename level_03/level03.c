#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valdecir A. A. de Olivera");
MODULE_DESCRIPTION("Second level dev Linux, reg device number and implement callback function");
/*
The /proc/devices file in Linux provides a list of character and block devices
currently configured in the running kernel.
It displays the major and minor numbers associated with each device type.
*/
#define MYMAJOR 90

/* Buffer for data*/
static char buffer[255];
static int buffer_pointer;
/* Var for device and device class */
static dev_t my_device_nb;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "dummydriver"
#define DRIVER_CLASS "MyModuleClass"

/**
* @brief   Read data out of the  buffer
*/
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){
	int to_copy, not_copied, delta;

	/* Get amount of data to copy */
	to_copy = min(count, buffer_pointer);

	/* copy data to user */
	not_copied = copy_to_user(user_buffer, buffer, to_copy);
	buffer_pointer = to_copy;
	/* Calculate data */
	delta = to_copy - not_copied;

	return delta;

}

/**
* @brief  Write  data to  buffer
*/
static ssize_t driver_write(struct file *File,const char *user_buffer, size_t count, loff_t *offs){
	int to_copy, not_copied, delta;

	/* Get amount of data to copy */
	to_copy = min( count, sizeof(buffer));

	/* copy data to user */
	not_copied = copy_from_user(buffer, user_buffer, to_copy);
	buffer_pointer = to_copy;
	/* Calculate data */
	delta = to_copy - not_copied;

	return delta;
}


/**
* @brief  This function  is called, when the device file is open
*/
static int driver_open(struct inode *device_file, struct file *instance){
	printk("device number - open was called! \n");
	return 0;
}

/**
* @brief  This function  is called, when the device file is close
*/
static int driver_close(struct inode *device_file, struct file *instance){ 
        printk("device number - close was called! \n");
        return 0;
}


static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open  = driver_open,
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
};


/**
* @brief  This function  is called, when the module is loaded  into the kernel
*/
static int __init ModuleInit(void){
	
	printk("Hi Level 02, kernel !\n");

	/* Allocate a device number */
	if ( alloc_chrdev_region(&my_device_nb, 0, 1, DRIVER_NAME) < 0){
		printk("Device Number could not be allocated! \n");
		return -1;
	}
	printk("read_write - Device Number. Major: %d, Minor: %d was registered! \n", my_device_nb >> 20, my_device_nb && 0xfffff);

	/* Create device class*/
	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL){
		printk("Device class can not be created! \n");
		goto ClassError; 
	}

	/* create device file	*/
	if (device_create(my_class, NULL, my_device_nb, NULL, DRIVER_NAME) == NULL){
		printk("Device class can not be created! \n");
		goto FileError; 

	}

	/* Initialize device file */
	cdev_init(&my_device,&fops);
	
	/* Registering device to Kernel */
	if (cdev_add(&my_device, my_device_nb, 1) == -1 ){
		printk("Registering of device to Kernel failed ! \n");
		goto AddError;
	}
	return 0;
AddError:
	device_destroy(my_class, my_device_nb);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev(my_device_nb, DRIVER_NAME);
	return -1;
}

/**
* @brief This function is called, when the module is removed from the kernel
*/
static void __exit ModuleExit(void){
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nb);
	class_destroy(my_class);
	unregister_chrdev(my_device_nb, "my_dev_number");
	printk("See you next Level, Kernel \n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

