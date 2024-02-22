#include <linux/module.h>
#include <linux/init.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valdecir A. A. de Olivera");
MODULE_DESCRIPTION("First level dev Linux Kernel Model LKM");

/**
* @brief  This function  is called, when the module is loaded  into the kernel
*/
static int __init ModuleInit(void){
	printk("Hi Level 01, kernel !\n");
	return 0;
}

/**
* @brief This function is called, when the module is removed from the kernel
*/
static void __exit ModuleExit(void){
	printk("See you next Level, Kernel \n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
