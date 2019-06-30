

#include <robust.h>
#include <linux/cdev.h>
#define MAX_ROBUST_NODE_NUM 9
#define I2C_CHECK_DEVNAME "i2c_check"
extern struct class* get_i2cdevinfo_class(void);
 int acc_load_status = 0;
 int magnetometer_init_flag = 0;
 int led_initialise_status = 0;
extern  int tpd_load_status ;
 int gyro_init_flag = 0;
 int proximity_initialise_status = 0;

static ssize_t  gyroscope_status_show(struct device *dev, struct device_attribute *attr, char *buf){
	int status=0;

	if(gyro_init_flag == 1)
		status = 1;

	if (!dev ) {
		return 0;
	}
	printk("gyroscope_status_show: %d \n",status);	
	return snprintf(buf, PAGE_SIZE, "%d\n", status);
}

static ssize_t gyroscope_status_store(struct device* dev, struct device_attribute *attr, const char *buf, size_t count){
	return count;
}

static ssize_t  accelerometer_status_show(struct device *dev, struct device_attribute *attr, char *buf){
	if (!dev ) {
		return 0;
	}
	printk("accelerometer_status_show %d \n",acc_load_status);
	return snprintf(buf, PAGE_SIZE, "%d\n", acc_load_status);
}

static ssize_t accelerometer_status_store(struct device* dev, struct device_attribute *attr, const char *buf, size_t count){
	return count;
}

static ssize_t  magnetometer_status_show(struct device *dev, struct device_attribute *attr, char *buf){
	int status=0;

	if(magnetometer_init_flag  == 1)
		status = 1;

	if (!dev ) {
		return 0;
	}
	printk("magnetometer_status_show %d \n",status);	
	return snprintf(buf, PAGE_SIZE, "%d\n", status);
}

static ssize_t magnetometer_status_store(struct device* dev, struct device_attribute *attr, const char *buf, size_t count){
	return count;
}

static ssize_t  led_status_show(struct device *dev, struct device_attribute *attr, char *buf){
	int status=0;

	if(led_initialise_status == 1)
		status = 1;

	if (!dev ) {
		return 0;
	}
	printk("led_status_show: %d \n",status);	
	return snprintf(buf, PAGE_SIZE, "%d\n", status);
}

static ssize_t led_status_store(struct device* dev, struct device_attribute *attr, const char *buf, size_t count){
	return count;
}

static ssize_t  touchpanel_status_show(struct device *dev, struct device_attribute *attr, char *buf){
	if (!dev ) {
		return 0;
	}

	printk("touchpanel_status_show: %d \n",tpd_load_status);	
	return snprintf(buf, PAGE_SIZE, "%d\n", tpd_load_status);
}

static ssize_t touchpanel_status_store(struct device* dev, struct device_attribute *attr, const char *buf, size_t count){
	return count;
}

static ssize_t  proximity_status_show(struct device *dev, struct device_attribute *attr, char *buf){
	int status=0;

	if(proximity_initialise_status  == 1)
		status = 1;

	if (!dev ){
		return 0;
	}

	printk("proximity_status_show: %d \n",status);	
	return snprintf(buf, PAGE_SIZE, "%d\n", status);
}

static ssize_t proximity_status_store(struct device* dev, struct device_attribute *attr, const char *buf, size_t count){
	return count;
}

static ssize_t  light_status_show(struct device *dev, struct device_attribute *attr, char *buf){
	int status=0;

	if(proximity_initialise_status  == 1)
		status = 1;

	if (!dev ) {
		return 0;
	}

	printk("light_status_show: %d \n",status);	
	return snprintf(buf, PAGE_SIZE, "%d\n", status);
}

static ssize_t light_status_store(struct device* dev, struct device_attribute *attr, const char *buf, size_t count){
	return count;
}

static ssize_t  smartpa_status_show(struct device *dev, struct device_attribute *attr, char *buf){
	int status=0;

	if(1  == 1)
		status = 1;

	if (!dev ) {
		return 0;
	}
	printk("smartpa_status_show: %d \n",status);	
	return snprintf(buf, PAGE_SIZE, "%d\n", status);
}

static ssize_t smartpa_status_store(struct device* dev, struct device_attribute *attr, const char *buf, size_t count){
	return count;
}

static ssize_t  hifi_status_show(struct device *dev, struct device_attribute *attr, char *buf){
	int status=0;

	if(1  == 1)
		status = 1;

	if (!dev ) {
		return 0;
	}
	printk("hifi_status_show: %d \n",status);	
	return snprintf(buf, PAGE_SIZE, "%d\n", status);
}

static ssize_t hifi_status_store(struct device* dev, struct device_attribute *attr, const char *buf, size_t count){
	return count;
}

static const robust_node_info robust_node[MAX_ROBUST_NODE_NUM] = {
	{"GyroscopeSensor" , "status", gyroscope_status_show , gyroscope_status_store},
	{"AccelerometerSensor" , "status", accelerometer_status_show , accelerometer_status_store},
	{"MagnetometerSensor" , "status", magnetometer_status_show , magnetometer_status_store},
	{"indicator_led" , "status", led_status_show , led_status_store},
	{"touch_panel" , "status", touchpanel_status_show , touchpanel_status_store},
	{"ProximitySensor" , "status", proximity_status_show , proximity_status_store},	
	{"LightSensor" , "status", light_status_show , light_status_store},
	{"audio_smpart_pa" , "status", smartpa_status_show , smartpa_status_store},
	{"audio_hifi" , "status", hifi_status_show , hifi_status_store},	
};

struct device_attribute dev_attr[MAX_ROBUST_NODE_NUM];
static const struct file_operations cdevOper = {
	.owner = THIS_MODULE,
};
//static struct class *probustClass;
//static dev_t midia1_devno;
//static dev_t midia2_devno;
static int __init robust_init(void){
	int i , retval = 1;
	//int err = 0;	
	struct class *i2cdev_class = NULL;
	struct device *i2cdev_dev = NULL;
	//static struct cdev *cdev;

	ROBUST_FUN;	

	//i2c check 
	//i2cdev_class = get_i2cdevinfo_class();
	i2cdev_class = class_create(THIS_MODULE, I2C_CHECK_DEVNAME);
	if (IS_ERR(i2cdev_class)){
		printk("Failed to create class(sys/class/i2c_check/)!\n");
		return 0;
	}

	for(i = 0 ; i < sizeof(robust_node)/sizeof(robust_node_info) ; i++){
		i2cdev_dev = device_create(i2cdev_class,NULL, 0, NULL, robust_node[i].dir);
		if (IS_ERR(i2cdev_dev)){
			pr_err("Failed to create device(sys/class/i2c_check/%s)!\n" , robust_node[i].dir);
			retval = 0;
			break;
		}

		dev_attr[i].attr.name = robust_node[i].node ;
		dev_attr[i].attr.mode = S_IWUSR | S_IRUGO ;
		dev_attr[i].show = robust_node[i].show;
		dev_attr[i].store = robust_node[i].store;
		if(device_create_file(i2cdev_dev, &dev_attr[i]) < 0){
			pr_err("Failed to create device file(sys/class/i2c_check/%s/%s)!\n" , robust_node[i].dir , robust_node[i].node);
			retval = 0;
			break;
		}
	}	

#if 0 /*modify by yuda.chen */
	//camera check
	probustClass = class_create(THIS_MODULE, "robust");

	if(1 == 1){
		err = alloc_chrdev_region(&midia1_devno, 0, 1, "media1");
		if (err < 0) {
			pr_err("%s:alloc_chrdev_region failed  error code=%d\n", __func__, err);
			return err;
		}
		device_create(probustClass, NULL, midia1_devno, NULL, "media1");
	}

	if(1 == 1){
		err = alloc_chrdev_region(&midia2_devno, 0, 1, "media2");
		if (err < 0) {
			pr_err("%s:alloc_chrdev_region failed  error code=%d\n", __func__, err);
			return err;
		}
		device_create(probustClass, NULL, midia2_devno, NULL, "media2");	
	}
#endif
#if 0
	cdev = cdev_alloc();
	if (!cdev) {
		err = -ENOMEM;
		unregister_chrdev_region(devno, 1);
		pr_err("%s:media_drv_major failed\n", __func__);
		return err;
	}

	cdev_init(cdev, &cdevOper);

	cdev->owner = THIS_MODULE;
	err = cdev_add(cdev, devno, 1);
	if (err)
		pr_err("media_drv_major %d adding LED%d", err, err);
#endif

	return retval;
}
/*----------------------------------------------------------------------------*/

static void __exit robust_exit(void){
	ROBUST_FUN;
}
/*----------------------------------------------------------------------------*/
module_init(robust_init);
module_exit(robust_exit);
/*----------------------------------------------------------------------------*/
MODULE_AUTHOR("ersen.shang");
MODULE_DESCRIPTION("robust driver");
MODULE_LICENSE("GPL");
