#include <linux/init.h>  
#include <linux/module.h>  
#include <linux/types.h>  
#include <linux/fs.h>  
#include <linux/device.h>  
#include <asm/uaccess.h>
#include <linux/slab.h>  
#include <linux/device.h>  

#define checkefuse_DEVICE_FILE_NAME        "checkefuse"  
#define checkefuse_DEVICE_CLASS_NAME       "checkefuse"  

extern int sec_schip_enabled(void);

static ssize_t show_efuse_val_info(struct device *dev, struct device_attribute *attr, char *buf)
{
	int val = -1;
	val = sec_schip_enabled();
	printk("[checkefuse]:efuse value = %d\n",val);
	return snprintf(buf, PAGE_SIZE, "%d\n", val);  
}

DEVICE_ATTR(val_efuse, S_IRUGO | S_IWUSR, show_efuse_val_info, NULL);

static int __init checkefuse_dev_init(void)
{
	static struct class* checkefuse_class = NULL; 
	struct device* temp = NULL;
        dev_t dev = 0;  
	checkefuse_class = class_create(THIS_MODULE, checkefuse_DEVICE_CLASS_NAME); 
 
        if(IS_ERR(checkefuse_class)){  
            printk(KERN_ALERT"[checkefuse]: Failed to create checkefuse class.\n");  
            return -1;
        }  

        temp = device_create(checkefuse_class, NULL, dev, "%s", checkefuse_DEVICE_FILE_NAME);  
        if(IS_ERR(temp)){  
            printk(KERN_ALERT"[checkefuse]: Failed to create checkefuse device.\n");  
            return -1;
        } 
          
	if (device_create_file(temp, &dev_attr_val_efuse) < 0){
	    pr_err("[checkefuse]:Failed to create attribute file(%s)!\n", dev_attr_val_efuse.attr.name);
	    return -1;
	}
	
        printk("[checkefuse]:checkefuse Init success.\n");
	return 0;
}

module_init(checkefuse_dev_init);

static void __exit checkefuse_dev_exit(void)
{
    printk("[checkefuse]: Unloading checkefuse driver\n");
}
module_exit(checkefuse_dev_exit);

MODULE_AUTHOR("XXX");
MODULE_DESCRIPTION("EFUSE CHECK DRIVER");
MODULE_LICENSE("GPL");


