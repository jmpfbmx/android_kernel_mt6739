
#include "tpd.h"
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fb.h>
#include <linux/pinctrl/consumer.h>
#ifdef CONFIG_MTK_MT6306_GPIO_SUPPORT
#include <mach/mtk_6306_gpio.h>
#endif

#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif
/*** begin: added by liuhe.zhong for board id hardware information on 2018.01.03 ***/
#include <linux/of_gpio.h>
#include <linux/gpio.h>
/*** end:   added by liuhe.zhong for board id hardware information on 2018.01.03 ***/

#if defined(CONFIG_MTK_S3320) || defined(CONFIG_MTK_S3320_50) \
	|| defined(CONFIG_MTK_S3320_47) || defined(CONFIG_MTK_MIT200) \
	|| defined(CONFIG_TOUCHSCREEN_SYNAPTICS_S3528) || defined(CONFIG_MTK_S7020) \
	|| defined(CONFIG_TOUCHSCREEN_MTK_SYNAPTICS_3320_50)
#include <linux/input/mt.h>
#endif /* CONFIG_MTK_S3320 */
/* for magnify velocity******************************************** */
#define TOUCH_IOC_MAGIC 'A'

#define TPD_GET_VELOCITY_CUSTOM_X _IO(TOUCH_IOC_MAGIC, 0)
#define TPD_GET_VELOCITY_CUSTOM_Y _IO(TOUCH_IOC_MAGIC, 1)
#define TPD_GET_FILTER_PARA _IOWR(TOUCH_IOC_MAGIC, 2, struct tpd_filter_t)
#ifdef CONFIG_COMPAT
#define COMPAT_TPD_GET_FILTER_PARA _IOWR(TOUCH_IOC_MAGIC, 2, struct tpd_filter_t)
#endif
struct tpd_filter_t tpd_filter;
struct tpd_dts_info tpd_dts_data;
struct pinctrl *pinctrl1;
struct pinctrl_state *pins_default;
struct pinctrl_state *eint_as_int, *eint_output0, *eint_output1, *rst_output0, *rst_output1;
const struct of_device_id touch_of_match[] = {
	{ .compatible = "mediatek,mt8173-touch", },
	{ .compatible = "mediatek,mt6757-touch", },
	{ .compatible = "mediatek,mt6763-touch", },
	{ .compatible = "mediatek,mt6797-touch", },
	{ .compatible = "mediatek,mt8163-touch", },
	{ .compatible = "mediatek,mt8127-touch", },
	{ .compatible = "mediatek,mt2701-touch", },
	{ .compatible = "mediatek,mt7623-touch", },
	{ .compatible = "mediatek,mt6799-touch", },
	{ .compatible = "mediatek,mt6739-touch", },
	{ .compatible = "mediatek,mt6771-touch", },
	{ .compatible = "mediatek,touch", },
	{},
};

void tpd_get_dts_info(void)
{
	struct device_node *node1 = NULL;
	int key_dim_local[16], i, ret;

	node1 = of_find_matching_node(node1, touch_of_match);
	if (node1) {
		ret = of_property_read_u32(node1, "tpd-max-touch-num", &tpd_dts_data.touch_max_num);
		if (ret != 0)
			TPD_DEBUG("tpd-max-touch-num not found\n");
		ret = of_property_read_u32(node1, "use-tpd-button", &tpd_dts_data.use_tpd_button);
		if (ret != 0)
			TPD_DEBUG("use-tpd-button not found\n");
		else
			TPD_DEBUG("[tpd]use-tpd-button = %d\n", tpd_dts_data.use_tpd_button);
		ret = of_property_read_u32_array(node1, "tpd-resolution",
			tpd_dts_data.tpd_resolution, ARRAY_SIZE(tpd_dts_data.tpd_resolution));
		if (ret != 0)
			TPD_DEBUG("tpd-resolution not found\n");
		if (tpd_dts_data.use_tpd_button) {
			ret = of_property_read_u32(node1, "tpd-key-num", &tpd_dts_data.tpd_key_num);
			if (ret != 0)
				TPD_DEBUG("tpd-key-num not found\n");
			ret = of_property_read_u32_array(node1, "tpd-key-local",
				tpd_dts_data.tpd_key_local, ARRAY_SIZE(tpd_dts_data.tpd_key_local));
			if (ret != 0)
				TPD_DEBUG("tpd-key-local not found\n");
/*** begin: modified by liuhe.zhong for cts test on 2018.02.11 ***/
				tpd_dts_data.tpd_key_local[0] = 158;
				tpd_dts_data.tpd_key_local[1] = 172;
				tpd_dts_data.tpd_key_local[2] = 139;
/*** end:   modified by liuhe.zhong for cts test on 2018.02.11 ***/
			ret = of_property_read_u32_array(node1, "tpd-key-dim-local",
				key_dim_local, ARRAY_SIZE(key_dim_local));
			if (ret != 0)
				TPD_DEBUG("tpd-key-dim-local not found\n");

			memcpy(tpd_dts_data.tpd_key_dim_local, key_dim_local, sizeof(key_dim_local));
/*** begin: modified by liuhe.zhong for cts test on 2018.02.11 ***/
				tpd_dts_data.tpd_key_dim_local[0].key_x = 81;
				tpd_dts_data.tpd_key_dim_local[0].key_y = 905;
				tpd_dts_data.tpd_key_dim_local[0].key_width = 106;
				tpd_dts_data.tpd_key_dim_local[0].key_height = 55;

				tpd_dts_data.tpd_key_dim_local[1].key_x = 187;
				tpd_dts_data.tpd_key_dim_local[1].key_y = 905;
				tpd_dts_data.tpd_key_dim_local[1].key_width = 106;
				tpd_dts_data.tpd_key_dim_local[1].key_height = 55;

				tpd_dts_data.tpd_key_dim_local[2].key_x = 293;
				tpd_dts_data.tpd_key_dim_local[2].key_y = 905;
				tpd_dts_data.tpd_key_dim_local[2].key_width = 106;
				tpd_dts_data.tpd_key_dim_local[2].key_height = 55;
/*** end:   modified by liuhe.zhong for cts test on 2018.02.11 ***/
			for (i = 0; i < 4; i++) {
				pr_debug("[tpd]key[%d].key_x = %d\n", i, tpd_dts_data.tpd_key_dim_local[i].key_x);
				pr_debug("[tpd]key[%d].key_y = %d\n", i, tpd_dts_data.tpd_key_dim_local[i].key_y);
				pr_debug("[tpd]key[%d].key_W = %d\n", i, tpd_dts_data.tpd_key_dim_local[i].key_width);
				pr_debug("[tpd]key[%d].key_H = %d\n", i, tpd_dts_data.tpd_key_dim_local[i].key_height);
			}
		}
		ret = of_property_read_u32(node1, "tpd-filter-enable", &tpd_dts_data.touch_filter.enable);
		if (ret != 0)
			TPD_DEBUG("tpd-filter-enable not found\n");
		if (tpd_dts_data.touch_filter.enable) {
			ret = of_property_read_u32(node1, "tpd-filter-pixel-density",
						&tpd_dts_data.touch_filter.pixel_density);
			if (ret != 0)
				TPD_DEBUG("tpd-filter-pixel-density not found\n");
			ret = of_property_read_u32_array(node1, "tpd-filter-custom-prameters",
				(u32 *)tpd_dts_data.touch_filter.W_W, ARRAY_SIZE(tpd_dts_data.touch_filter.W_W));
			if (ret != 0)
				TPD_DEBUG("tpd-filter-custom-prameters not found\n");
			ret = of_property_read_u32_array(node1, "tpd-filter-custom-speed",
				tpd_dts_data.touch_filter.VECLOCITY_THRESHOLD,
				ARRAY_SIZE(tpd_dts_data.touch_filter.VECLOCITY_THRESHOLD));
			if (ret != 0)
				TPD_DEBUG("tpd-filter-custom-speed not found\n");
		}
		memcpy(&tpd_filter, &tpd_dts_data.touch_filter, sizeof(tpd_filter));
		TPD_DEBUG("[tpd]tpd-filter-enable = %d, pixel_density = %d\n",
					tpd_filter.enable, tpd_filter.pixel_density);
	} else {
		pr_err("[tpd]%s can't find touch compatible custom node\n", __func__);
	}
}

static DEFINE_MUTEX(tpd_set_gpio_mutex);
void tpd_gpio_as_int(int pin)
{
	mutex_lock(&tpd_set_gpio_mutex);
	TPD_DEBUG("[tpd]tpd_gpio_as_int\n");
	if (pin == 1)
		pinctrl_select_state(pinctrl1, eint_as_int);
	mutex_unlock(&tpd_set_gpio_mutex);
}

void tpd_gpio_output(int pin, int level)
{
	mutex_lock(&tpd_set_gpio_mutex);
	TPD_DEBUG("[tpd]tpd_gpio_output pin = %d, level = %d\n", pin, level);
	if (pin == 1) {
		if (level)
			pinctrl_select_state(pinctrl1, eint_output1);
		else
			pinctrl_select_state(pinctrl1, eint_output0);
	} else {
		if (tpd_dts_data.tpd_use_ext_gpio) {
#ifdef CONFIG_MTK_MT6306_GPIO_SUPPORT
			mt6306_set_gpio_dir(tpd_dts_data.rst_ext_gpio_num, 1);
			mt6306_set_gpio_out(tpd_dts_data.rst_ext_gpio_num, level);
#endif
		} else {
			if (level)
				pinctrl_select_state(pinctrl1, rst_output1);
			else
				pinctrl_select_state(pinctrl1, rst_output0);
		}
	}
	mutex_unlock(&tpd_set_gpio_mutex);
}
int tpd_get_gpio_info(struct platform_device *pdev)
{
	int ret;

	TPD_DEBUG("[tpd %d] mt_tpd_pinctrl+++++++++++++++++\n", pdev->id);
pr_err("Lomen 0.1\n");
	pinctrl1 = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(pinctrl1)) {
		ret = PTR_ERR(pinctrl1);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl1!\n");
		return ret;
	}
pr_err("Lomen 0.2\n");
	pins_default = pinctrl_lookup_state(pinctrl1, "default");
	if (IS_ERR(pins_default)) {
		ret = PTR_ERR(pins_default);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl default %d!\n", ret);
	}
	eint_as_int = pinctrl_lookup_state(pinctrl1, "state_eint_as_int");
	if (IS_ERR(eint_as_int)) {
		ret = PTR_ERR(eint_as_int);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_eint_as_int!\n");
		return ret;
	}
	eint_output0 = pinctrl_lookup_state(pinctrl1, "state_eint_output0");
	if (IS_ERR(eint_output0)) {
		ret = PTR_ERR(eint_output0);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_eint_output0!\n");
		return ret;
	}
	eint_output1 = pinctrl_lookup_state(pinctrl1, "state_eint_output1");
	if (IS_ERR(eint_output1)) {
		ret = PTR_ERR(eint_output1);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_eint_output1!\n");
		return ret;
	}
	if (tpd_dts_data.tpd_use_ext_gpio == false) {
		rst_output0 = pinctrl_lookup_state(pinctrl1, "state_rst_output0");
		if (IS_ERR(rst_output0)) {
			ret = PTR_ERR(rst_output0);
			dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_rst_output0!\n");
			return ret;
		}
		rst_output1 = pinctrl_lookup_state(pinctrl1, "state_rst_output1");
		if (IS_ERR(rst_output1)) {
			ret = PTR_ERR(rst_output1);
			dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_rst_output1!\n");
			return ret;
		}
	}
	TPD_DEBUG("[tpd%d] mt_tpd_pinctrl----------\n", pdev->id);
	return 0;
}

static int tpd_misc_open(struct inode *inode, struct file *file)
{
	return nonseekable_open(inode, file);
}

static int tpd_misc_release(struct inode *inode, struct file *file)
{
	return 0;
}

#ifdef CONFIG_COMPAT
static long tpd_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ret;
	void __user *arg32 = compat_ptr(arg);

	if (!file->f_op || !file->f_op->unlocked_ioctl)
		return -ENOTTY;
	switch (cmd) {
	case COMPAT_TPD_GET_FILTER_PARA:
		if (arg32 == NULL) {
			pr_err("invalid argument.");
			return -EINVAL;
		}
		ret = file->f_op->unlocked_ioctl(file, TPD_GET_FILTER_PARA,
					   (unsigned long)arg32);
		if (ret) {
			pr_err("TPD_GET_FILTER_PARA unlocked_ioctl failed.");
			return ret;
		}
		break;
	default:
		pr_err("tpd: unknown IOCTL: 0x%08x\n", cmd);
		ret = -ENOIOCTLCMD;
		break;
	}
	return ret;
}
#endif
static long tpd_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	/* char strbuf[256]; */
	void __user *data;

	long err = 0;

	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) {
		pr_err("tpd: access error: %08X, (%2d, %2d)\n", cmd, _IOC_DIR(cmd), _IOC_SIZE(cmd));
		return -EFAULT;
	}

	switch (cmd) {
	case TPD_GET_VELOCITY_CUSTOM_X:
		data = (void __user *)arg;

		if (data == NULL) {
			err = -EINVAL;
			break;
		}

		if (copy_to_user(data, &tpd_v_magnify_x, sizeof(tpd_v_magnify_x))) {
			err = -EFAULT;
			break;
		}

		break;

	case TPD_GET_VELOCITY_CUSTOM_Y:
		data = (void __user *)arg;

		if (data == NULL) {
			err = -EINVAL;
			break;
		}

		if (copy_to_user(data, &tpd_v_magnify_y, sizeof(tpd_v_magnify_y))) {
			err = -EFAULT;
			break;
		}

		break;
	case TPD_GET_FILTER_PARA:
			data = (void __user *) arg;

			if (data == NULL) {
				err = -EINVAL;
				pr_err("tpd: TPD_GET_FILTER_PARA IOCTL CMD: data is null\n");
				break;
			}

			if (copy_to_user(data, &tpd_filter, sizeof(struct tpd_filter_t))) {
				pr_err("tpd: TPD_GET_FILTER_PARA IOCTL CMD: copy data error\n");
				err = -EFAULT;
				break;
			}
			break;
	default:
		pr_err("tpd: unknown IOCTL: 0x%08x\n", cmd);
		err = -ENOIOCTLCMD;
		break;

	}

	return err;
}
static struct work_struct touch_resume_work;
static struct workqueue_struct *touch_resume_workqueue;
static const struct file_operations tpd_fops = {
/* .owner = THIS_MODULE, */
	.open = tpd_misc_open,
	.release = tpd_misc_release,
	.unlocked_ioctl = tpd_unlocked_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = tpd_compat_ioctl,
#endif
};

/*----------------------------------------------------------------------------*/
static struct miscdevice tpd_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "touch",
	.fops = &tpd_fops,
};

/* ********************************************** */
/* #endif */


/* function definitions */
static int __init tpd_device_init(void);
static void __exit tpd_device_exit(void);
static int tpd_probe(struct platform_device *pdev);
static int tpd_remove(struct platform_device *pdev);
static struct work_struct tpd_init_work;
static struct workqueue_struct *tpd_init_workqueue;
static int tpd_suspend_flag;
int tpd_register_flag;
/* global variable definitions */
struct tpd_device *tpd;
static struct tpd_driver_t tpd_driver_list[TP_DRV_MAX_COUNT];	/* = {0}; */

struct platform_device tpd_device = {
	.name		= TPD_DEVICE,
	.id			= -1,
};
const struct dev_pm_ops tpd_pm_ops = {
	.suspend = NULL,
	.resume = NULL,
};
static struct platform_driver tpd_driver = {
	.remove = tpd_remove,
	.shutdown = NULL,
	.probe = tpd_probe,
	.driver = {
			.name = TPD_DEVICE,
			.pm = &tpd_pm_ops,
			.owner = THIS_MODULE,
			.of_match_table = touch_of_match,
	},
};
static struct tpd_driver_t *g_tpd_drv;
/* hh: use fb_notifier */
static struct notifier_block tpd_fb_notifier;
/* use fb_notifier */
static void touch_resume_workqueue_callback(struct work_struct *work)
{
	TPD_DEBUG("GTP touch_resume_workqueue_callback\n");
	g_tpd_drv->resume(NULL);
	tpd_suspend_flag = 0;
}
static int tpd_fb_notifier_callback(struct notifier_block *self, unsigned long event, void *data)
{
	struct fb_event *evdata = NULL;
	int blank;
	int err = 0;

	TPD_DEBUG("tpd_fb_notifier_callback\n");

	evdata = data;
	/* If we aren't interested in this event, skip it immediately ... */
	//Begin modified by xiaobin.zhai for sequential requirements of insell lcd
	if ((event != FB_EARLY_EVENT_BLANK)&&(event != FB_EVENT_BLANK))
		return 0;
	//End modified by xiaobin.zhai for sequential requirements of insell lcd

	blank = *(int *)evdata->data;
	TPD_DMESG("fb_notify(blank=%d)\n", blank);
	switch (blank) {
	case FB_BLANK_UNBLANK:
		//Begin modified by xiaobin.zhai for sequential requirements of insell lcd
		if (event == FB_EVENT_BLANK){
			TPD_DMESG("LCD ON Notify\n");
			if (g_tpd_drv && tpd_suspend_flag) {
				err = queue_work(touch_resume_workqueue, &touch_resume_work);
				if (!err) {
					TPD_DMESG("start touch_resume_workqueue failed\n");
					return err;
				}
			}
		}
		//End modified by xiaobin.zhai for sequential requirements of insell lcd 
		break;
	case FB_BLANK_POWERDOWN:
		//Begin modified by xiaobin.zhai for sequential requirements of insell lcd
		if (event == FB_EARLY_EVENT_BLANK){
			TPD_DMESG("LCD OFF Notify\n");
			if (g_tpd_drv && !tpd_suspend_flag) {
				err = cancel_work_sync(&touch_resume_work);
				if (!err)
					TPD_DMESG("cancel touch_resume_workqueue err = %d\n", err);
				g_tpd_drv->suspend(NULL);
			}
			tpd_suspend_flag = 1;
		}
		//End modified by xiaobin.zhai for sequential requirements of insell lcd 
		break;
	default:
		break;
	}
	return 0;
}
/* Add driver: if find TPD_TYPE_CAPACITIVE driver successfully, loading it */
int tpd_driver_add(struct tpd_driver_t *tpd_drv)
{
	int i;

	if (g_tpd_drv != NULL) {
		TPD_DMESG("touch driver exist\n");
		return -1;
	}
	/* check parameter */
	if (tpd_drv == NULL)
		return -1;
	tpd_drv->tpd_have_button = tpd_dts_data.use_tpd_button;
	/* R-touch */
	if (strcmp(tpd_drv->tpd_device_name, "generic") == 0) {
		tpd_driver_list[0].tpd_device_name = tpd_drv->tpd_device_name;
		tpd_driver_list[0].tpd_local_init = tpd_drv->tpd_local_init;
		tpd_driver_list[0].suspend = tpd_drv->suspend;
		tpd_driver_list[0].resume = tpd_drv->resume;
		tpd_driver_list[0].tpd_have_button = tpd_drv->tpd_have_button;
		return 0;
	}
	for (i = 1; i < TP_DRV_MAX_COUNT; i++) {
		/* add tpd driver into list */
		if (tpd_driver_list[i].tpd_device_name == NULL) {
			tpd_driver_list[i].tpd_device_name = tpd_drv->tpd_device_name;
			tpd_driver_list[i].tpd_local_init = tpd_drv->tpd_local_init;
			tpd_driver_list[i].suspend = tpd_drv->suspend;
			tpd_driver_list[i].resume = tpd_drv->resume;
			tpd_driver_list[i].tpd_have_button = tpd_drv->tpd_have_button;
			tpd_driver_list[i].attrs = tpd_drv->attrs;
#if 0
			if (tpd_drv->tpd_local_init() == 0) {
				TPD_DMESG("load %s successfully\n",
					  tpd_driver_list[i].tpd_device_name);
				g_tpd_drv = &tpd_driver_list[i];
			}
#endif
			break;
		}
		if (strcmp(tpd_driver_list[i].tpd_device_name, tpd_drv->tpd_device_name) == 0)
			return 1;	/* driver exist */
	}

	return 0;
}

int tpd_driver_remove(struct tpd_driver_t *tpd_drv)
{
	int i = 0;
	/* check parameter */
	if (tpd_drv == NULL)
		return -1;
	for (i = 0; i < TP_DRV_MAX_COUNT; i++) {
		/* find it */
		if (strcmp(tpd_driver_list[i].tpd_device_name, tpd_drv->tpd_device_name) == 0) {
			memset(&tpd_driver_list[i], 0, sizeof(struct tpd_driver_t));
			break;
		}
	}
	return 0;
}

static void tpd_create_attributes(struct device *dev, struct tpd_attrs *attrs)
{
	int num = attrs->num;

	for (; num > 0;)
		device_create_file(dev, attrs->attr[--num]);
}
/*** begin: modified by liuhe.zhong for ctp hardware information on 2017.12.12 ***/
extern unsigned char tp_info[128]; 
/*** end:   modified by liuhe.zhong for ctp hardware information on 2017.12.12 ***/
//Add by qicai.gu begin for CTP deviceinfo node
int open_gesture = 0;
char ctp_module_name[256]="NA:NA:0";
int tp_firmware_version=0;//Added by qicai.gu.hz for Task 5000517 on 2017/07/06 

static ssize_t  ctpinfo_show(struct device *dev, 
                                  struct device_attribute *attr, char *buf)
{
/*** begin: modified by liuhe.zhong for ctp hardware information on 2017.12.12 ***/      
    sprintf(ctp_module_name, "%s", tp_info); 
/*** end:   modified by liuhe.zhong for ctp hardware information on 2017.12.12 ***/
    printk("Dump  %s \n",ctp_module_name);

    if (!dev ) {
        return 0;
    }
/*** begin: modified by liuhe.zhong for optimizing ctp hardware information on 2018.01.20 ***/
    return snprintf(buf, PAGE_SIZE, "%s", ctp_module_name);
/*** end:   modified by liuhe.zhong for optimizing ctp hardware information on 2018.01.20 ***/
}

static ssize_t ctpinfo_store(struct device* dev,
                                   struct device_attribute *attr, const char *buf, size_t count)
{
        return count;
}
static ssize_t  gesture_show(struct device *dev,
                                  struct device_attribute *attr, char *buf)
{
    return snprintf(buf, PAGE_SIZE, "%d\n", open_gesture);
}

static ssize_t gesture_store(struct device* dev,
                                   struct device_attribute *attr, const char *buf, size_t count)
{
	int value = 0;
	if (!kstrtoint(buf, 10, &value))
	{
		 printk("invallid format\n");
	}
	open_gesture= !!(value);
        return count;
}

/*** begin: added by liuhe.zhong for board id hardware information on 2017.12.31 ***/
/*** begin: added by liuhe.zhong for close spi0 clk when MS has no fingerprint on 2018.02.27 ***/
unsigned char board_id;
EXPORT_SYMBOL(board_id);
/*** end:   added by liuhe.zhong for close spi0 clk when MS has no fingerprint on 2018.02.27 ***/
static ssize_t board_id_show(struct device *dev,
                                  struct device_attribute *attr, char *buf)
{
	struct device_node *node1 = NULL;
	unsigned int gpio103 = 0;
	unsigned int gpio104 = 0;
	unsigned int gpio105 = 0;

	unsigned int gpiostate103 = 1;
	unsigned int gpiostate104 = 1;
	unsigned int gpiostate105 = 1;
	
	int ret = 0;
	
	node1 = of_find_matching_node(node1, touch_of_match);
	if (node1) {
		ret = of_get_named_gpio(node1, "boardid-gpio103", 0);
		if (ret < 0) {
			printk("[BOARD_ID]Failed to get boardid-gpio103 number\n");
			goto out;
		}
		gpio103 = ret;
		printk("[BOARD_ID]boardid-gpio103 = %d\n", gpio103);

		ret = of_get_named_gpio(node1, "boardid-gpio104", 0);
		if (ret < 0) {
			printk("[BOARD_ID]Failed to get boardid-gpio104 number\n");
			goto out;
		}
		gpio104 = ret;
		printk("[BOARD_ID]boardid-gpio104 = %d\n", gpio104);

		ret = of_get_named_gpio(node1, "boardid-gpio105", 0);
		if (ret < 0) {
			printk("[BOARD_ID]Failed to get boardid-gpio105 number\n");
			goto out;
		}
		gpio105 = ret;
		printk("[BOARD_ID]boardid-gpio105 = %d\n", gpio105);

		gpiostate103 = gpio_get_value(gpio103);
		gpiostate104 = gpio_get_value(gpio104);
		gpiostate105 = gpio_get_value(gpio105);
		out:
			printk("[BOARD_ID]%s: %d,%d,%d\n", __func__, gpiostate103, gpiostate104, gpiostate105);
			board_id = gpiostate103*100 + gpiostate104*10 + gpiostate105;
	}
	else {
		printk("[BOARD_ID]%s: can't find touch compatible custom node\n", __func__);
	}
		

	return snprintf(buf, PAGE_SIZE, "%d%d%d\n", gpiostate103, gpiostate104, gpiostate105);
}

static ssize_t board_id_store(struct device* dev,
                                   struct device_attribute *attr, const char *buf, size_t count)
{
        return count;
}
/*** end:   added by liuhe.zhong for board id hardware information on 2017.12.31 ***/

/*** begin: added by liuhe.zhong for fingerprint hardware information on 2018.01.30 ***/
unsigned char fp_info[256]="NA:NA:NA";
EXPORT_SYMBOL(fp_info);
static ssize_t FP_show(struct device *dev,
                                  struct device_attribute *attr, char *buf)
{
	if((board_id != 100) && (board_id != 101)) {
		sprintf(fp_info, "NA:NA:NA");
	}
	return snprintf(buf, PAGE_SIZE, "%s\n", fp_info);
}

static ssize_t FP_store(struct device* dev,
                                   struct device_attribute *attr, const char *buf, size_t count)
{
        return count;
}
DEVICE_ATTR(FP, S_IWUSR | S_IRUGO, FP_show, FP_store);
/*** end:   added by liuhe.zhong for fingerprint hardware information on 2018.01.30 ***/

DEVICE_ATTR(ctp,     S_IWUSR | S_IRUGO, ctpinfo_show, ctpinfo_store);
DEVICE_ATTR(gesture,     S_IWUSR | S_IRUGO, gesture_show, gesture_store);

/*** begin: added by liuhe.zhong for board id hardware information on 2017.12.31 ***/
DEVICE_ATTR(board_id, S_IWUSR | S_IRUGO, board_id_show, board_id_store);
/*** end:   added by liuhe.zhong for board id hardware information on 2017.12.31 ***/

//Modified by qicai.gu.hz for Task 4734391 on 2017/05/24 begin
//Modified by qicai.gu.hz for Task 4653409 on 2017/05/09 begin
#if  0
int psensor_available = 1;
int global_psensor_switch = 1;
extern u8 tpd_proximity_detect;
extern u8 g_FaceClosingTp;
extern int tp_vendor_id_ps;// 0 for msg  ;1 for gt 

extern int ps_report_interrupt_data(int value);

static ssize_t psensor_available_show(struct device *dev,
                    struct device_attribute *attr,
                    char *buf)
{
        return snprintf(buf, PAGE_SIZE, "psensor_available:%d;global_psensor_switch:%d\n", psensor_available, global_psensor_switch);
}   

static ssize_t psensor_available_store(struct device *dev,
                    struct device_attribute *attr,
                    const char *buf, size_t count)
{   
     int value = 0;
	 int err=0;
     if (!kstrtoint(buf, 10, &value))
     {
          printk("invallid format\n");
     }
	 switch(value)
	 {
	 	case 0:
			 psensor_available = !!(value);
			 break;
		case 1:
			 psensor_available = !!(value);
			 if(psensor_available)
			 {
			 	if (tp_vendor_id_ps == 1 ) {
					if ((err = ps_report_interrupt_data(tpd_proximity_detect)))
					{
						printk(" call ps_report_interrupt_data failed= %d\n", err);
					}
				}else if(tp_vendor_id_ps == 0 ) {
					 if((err = ps_report_interrupt_data(g_FaceClosingTp)))
		                       	 {
		                                TPD_DMESG("call ps_report_interrupt_data fail = %d\n", err);
		                        	}
				} else {
					printk("tp_vendor_id_ps is :%d,but is not exited\n", tp_vendor_id_ps);
				}
			 }
			 break;
		case 2:
			 global_psensor_switch = 1;
			 printk("global_psensor_switch:%d\n", global_psensor_switch);
			 break;
		case 3:
			 global_psensor_switch = 0;
			 printk("global_psensor_switch:%d\n", global_psensor_switch);
			 break;
		default:
			 break;
			
	 }
     return count;
}

DEVICE_ATTR(psensor_available,     S_IWUSR | S_IRUGO, psensor_available_show, psensor_available_store);
#endif
//Modified by qicai.gu.hz for Task 4653409 on 2017/05/09 end
//Modified by qicai.gu.hz for Task 4734391 on 2017/05/24 end

extern struct device* get_deviceinfo_dev(void);
/* touch panel probe */
static void Create_ctpinfo_node_ForMMI(void)
{

       struct device * ctp;
       struct device * gesture;
/*** begin: added by liuhe.zhong for board id hardware information on 2017.12.31 ***/
       struct device * board_id;
/*** end:   added by liuhe.zhong for board id hardware information on 2017.12.31 ***/

/*** begin: added by liuhe.zhong for fingerprint hardware information on 2018.01.30 ***/
       struct device * FP;
//Modified by qicai.gu.hz for Task 4734391 on 2017/05/24 begin
//Modified by qicai.gu.hz for Task 4653409 on 2017/05/09 end	   
#if 0
      struct device *  psensor_available;
	   psensor_available=get_deviceinfo_dev();
       if (device_create_file(psensor_available, &dev_attr_psensor_available) < 0)
               printk("Failed to create device file(%s)!\n", dev_attr_psensor_available.attr.name);
#endif
//Modified by qicai.gu.hz for Task 4653409 on 2017/05/9 end
//Modified by qicai.gu.hz for Task 4734391 on 2017/05/24 end

//modify by llf
#if 1
       ctp=get_deviceinfo_dev();
       gesture=get_deviceinfo_dev();
/*** begin: added by liuhe.zhong for board id hardware information on 2017.12.31 ***/
       board_id = get_deviceinfo_dev();
/*** end:   added by liuhe.zhong for board id hardware information on 2017.12.31 ***/
       FP = get_deviceinfo_dev();
       if (device_create_file(ctp, &dev_attr_ctp) < 0)
               printk("Failed to create device file(%s)!\n", dev_attr_ctp.attr.name);
       //psensor_available=get_deviceinfo_dev();
       if (device_create_file(gesture, &dev_attr_gesture) < 0)
               printk("Failed to create device file(%s)!\n", dev_attr_gesture.attr.name);
/*** begin: added by liuhe.zhong for board id hardware information on 2017.12.31 ***/	   
       if (device_create_file(board_id, &dev_attr_board_id) < 0)
               printk("Failed to create device file(%s)!\n", dev_attr_board_id.attr.name);
/*** end:   added by liuhe.zhong for board id hardware information on 2017.12.31 ***/
       if (device_create_file(FP, &dev_attr_FP) < 0)
               printk("Failed to create device file(%s)!\n", dev_attr_FP.attr.name);
/*** end:   added by liuhe.zhong for fingerprint hardware information on 2018.01.30 ***/
#endif
	return;
}
/*add by rongxiao.deng end*/
//add by qicai.gu end 

static int tpd_probe(struct platform_device *pdev)
{
	int touch_type = 1;	/* 0:R-touch, 1: Cap-touch */
	int i = 0;
#ifndef CONFIG_CUSTOM_LCM_X
#ifdef CONFIG_LCM_WIDTH
	unsigned long tpd_res_x = 0, tpd_res_y = 0;
	int ret = 0;
#endif
#endif

	TPD_DMESG("enter %s, %d\n", __func__, __LINE__);

	if (misc_register(&tpd_misc_device))
		pr_err("mtk_tpd: tpd_misc_device register failed\n");
pr_err("Lomen 0\n");
	tpd_get_gpio_info(pdev);
pr_err("Lomen 1\n");
	tpd = kmalloc(sizeof(struct tpd_device), GFP_KERNEL);
	if (tpd == NULL)
		return -ENOMEM;
	memset(tpd, 0, sizeof(struct tpd_device));

	/* allocate input device */
	tpd->dev = input_allocate_device();
	if (tpd->dev == NULL) {
		kfree(tpd);
		return -ENOMEM;
	}
	/* TPD_RES_X = simple_strtoul(LCM_WIDTH, NULL, 0); */
	/* TPD_RES_Y = simple_strtoul(LCM_HEIGHT, NULL, 0); */

	#ifdef CONFIG_MTK_LCM_PHYSICAL_ROTATION
	if (strncmp(CONFIG_MTK_LCM_PHYSICAL_ROTATION, "90", 2) == 0
		|| strncmp(CONFIG_MTK_LCM_PHYSICAL_ROTATION, "270", 3) == 0) {
/*Fix build errors,as some projects  cannot support these apis while bring up*/
#if defined(CONFIG_MTK_FB) && defined(CONFIG_MTK_LCM)
		TPD_RES_Y = DISP_GetScreenWidth();
		TPD_RES_X = DISP_GetScreenHeight();
#endif
	} else
    #endif
	{
#ifdef CONFIG_CUSTOM_LCM_X
#ifndef CONFIG_FPGA_EARLY_PORTING
/*Fix build errors,as some projects  cannot support these apis while bring up*/
#if defined(CONFIG_MTK_FB) && defined(CONFIG_MTK_LCM)
		TPD_RES_X = DISP_GetScreenWidth();
		TPD_RES_Y = DISP_GetScreenHeight();
#else/*for some projects, we do not use mtk framebuffer*/
	TPD_RES_X = tpd_dts_data.tpd_resolution[0];
	TPD_RES_Y = tpd_dts_data.tpd_resolution[1];
#endif
#endif
#else
#ifdef CONFIG_LCM_WIDTH
		ret = kstrtoul(CONFIG_LCM_WIDTH, 0, &tpd_res_x);
		if (ret < 0) {
			pr_err("Touch down get lcm_x failed");
			return ret;
		}
		TPD_RES_X = tpd_res_x;
		ret = kstrtoul(CONFIG_LCM_HEIGHT, 0, &tpd_res_x);
		if (ret < 0) {
			pr_err("Touch down get lcm_y failed");
			return ret;
		}
		TPD_RES_Y = tpd_res_y;
#endif
#endif
	}

	if (2560 == TPD_RES_X)
		TPD_RES_X = 2048;
	if (1600 == TPD_RES_Y)
		TPD_RES_Y = 1536;
	pr_debug("mtk_tpd: TPD_RES_X = %lu, TPD_RES_Y = %lu\n", TPD_RES_X, TPD_RES_Y);

	tpd_mode = TPD_MODE_NORMAL;
	tpd_mode_axis = 0;
	tpd_mode_min = TPD_RES_Y / 2;
	tpd_mode_max = TPD_RES_Y;
	tpd_mode_keypad_tolerance = TPD_RES_X * TPD_RES_X / 1600;
	/* struct input_dev dev initialization and registration */
	tpd->dev->name = TPD_DEVICE;
	set_bit(EV_ABS, tpd->dev->evbit);
	set_bit(EV_KEY, tpd->dev->evbit);
	set_bit(ABS_X, tpd->dev->absbit);
	set_bit(ABS_Y, tpd->dev->absbit);
	set_bit(ABS_PRESSURE, tpd->dev->absbit);
#if !defined(CONFIG_MTK_S3320) && !defined(CONFIG_MTK_S3320_47)\
	&& !defined(CONFIG_MTK_S3320_50) && !defined(CONFIG_MTK_MIT200) \
	&& !defined(CONFIG_TOUCHSCREEN_SYNAPTICS_S3528) && !defined(CONFIG_MTK_S7020) \
	&& !defined(CONFIG_TOUCHSCREEN_MTK_SYNAPTICS_3320_50)
	set_bit(BTN_TOUCH, tpd->dev->keybit);
#endif /* CONFIG_MTK_S3320 */
	set_bit(INPUT_PROP_DIRECT, tpd->dev->propbit);

	/* save dev for regulator_get() before tpd_local_init() */
	tpd->tpd_dev = &pdev->dev;
	for (i = 1; i < TP_DRV_MAX_COUNT; i++) {
		/* add tpd driver into list */
		if (tpd_driver_list[i].tpd_device_name != NULL) {
			tpd_driver_list[i].tpd_local_init();
			/* msleep(1); */
			if (tpd_load_status == 1) {
				TPD_DMESG("[mtk-tpd]tpd_probe, tpd_driver_name=%s\n",
					  tpd_driver_list[i].tpd_device_name);
				g_tpd_drv = &tpd_driver_list[i];
				break;
			}
		}
	}
	if (g_tpd_drv == NULL) {
		if (tpd_driver_list[0].tpd_device_name != NULL) {
			g_tpd_drv = &tpd_driver_list[0];
			/* touch_type:0: r-touch, 1: C-touch */
			touch_type = 0;
			g_tpd_drv->tpd_local_init();
			TPD_DMESG("[mtk-tpd]Generic touch panel driver\n");
		} else {
			TPD_DMESG("[mtk-tpd]cap touch and Generic touch both are not loaded!!\n");
			return 0;
		}
	}
	touch_resume_workqueue = create_singlethread_workqueue("touch_resume");
	INIT_WORK(&touch_resume_work, touch_resume_workqueue_callback);
	/* use fb_notifier */
	tpd_fb_notifier.notifier_call = tpd_fb_notifier_callback;
	if (fb_register_client(&tpd_fb_notifier))
		TPD_DMESG("register fb_notifier fail!\n");
	/* TPD_TYPE_CAPACITIVE handle */
	if (touch_type == 1) {

		set_bit(ABS_MT_TRACKING_ID, tpd->dev->absbit);
		set_bit(ABS_MT_TOUCH_MAJOR, tpd->dev->absbit);
		set_bit(ABS_MT_TOUCH_MINOR, tpd->dev->absbit);
		set_bit(ABS_MT_POSITION_X, tpd->dev->absbit);
		set_bit(ABS_MT_POSITION_Y, tpd->dev->absbit);
		input_set_abs_params(tpd->dev, ABS_MT_POSITION_X, 0, TPD_RES_X, 0, 0);
		input_set_abs_params(tpd->dev, ABS_MT_POSITION_Y, 0, TPD_RES_Y, 0, 0);
#if defined(CONFIG_MTK_S3320) || defined(CONFIG_MTK_S3320_47) \
	|| defined(CONFIG_MTK_S3320_50) || defined(CONFIG_MTK_MIT200) \
	|| defined(CONFIG_TOUCHSCREEN_SYNAPTICS_S3528) || defined(CONFIG_MTK_S7020) \
	|| defined(CONFIG_TOUCHSCREEN_MTK_SYNAPTICS_3320_50)
		input_set_abs_params(tpd->dev, ABS_MT_PRESSURE, 0, 255, 0, 0);
		input_set_abs_params(tpd->dev, ABS_MT_WIDTH_MAJOR, 0, 15, 0, 0);
		input_set_abs_params(tpd->dev, ABS_MT_WIDTH_MINOR, 0, 15, 0, 0);
		input_mt_init_slots(tpd->dev, 10, 0);
#else
		input_set_abs_params(tpd->dev, ABS_MT_TOUCH_MAJOR, 0, 100, 0, 0);
		input_set_abs_params(tpd->dev, ABS_MT_TOUCH_MINOR, 0, 100, 0, 0);
#endif /* CONFIG_MTK_S3320 */
		TPD_DMESG("Cap touch panel driver\n");
	}
	input_set_abs_params(tpd->dev, ABS_X, 0, TPD_RES_X, 0, 0);
	input_set_abs_params(tpd->dev, ABS_Y, 0, TPD_RES_Y, 0, 0);
	input_abs_set_res(tpd->dev, ABS_X, TPD_RES_X);
	input_abs_set_res(tpd->dev, ABS_Y, TPD_RES_Y);
	input_set_abs_params(tpd->dev, ABS_PRESSURE, 0, 255, 0, 0);
	input_set_abs_params(tpd->dev, ABS_MT_TRACKING_ID, 0, 10, 0, 0);

	if (input_register_device(tpd->dev))
		TPD_DMESG("input_register_device failed.(tpd)\n");
	else
		tpd_register_flag = 1;
	if (g_tpd_drv->tpd_have_button)
		tpd_button_init();

	if (g_tpd_drv->attrs.num)
		tpd_create_attributes(&pdev->dev, &g_tpd_drv->attrs);

	//Add by longfang.liu for deviceinfo node
	Create_ctpinfo_node_ForMMI();

	return 0;
}
static int tpd_remove(struct platform_device *pdev)
{
	input_unregister_device(tpd->dev);
	return 0;
}

/* called when loaded into kernel */
static void tpd_init_work_callback(struct work_struct *work)
{
	TPD_DEBUG("MediaTek touch panel driver init\n");
	if (platform_driver_register(&tpd_driver) != 0)
		TPD_DMESG("unable to register touch panel driver.\n");
}
static int __init tpd_device_init(void)
{
	int res = 0;

	tpd_init_workqueue = create_singlethread_workqueue("mtk-tpd");
	INIT_WORK(&tpd_init_work, tpd_init_work_callback);

	res = queue_work(tpd_init_workqueue, &tpd_init_work);
	if (!res)
		pr_err("tpd : touch device init failed res:%d\n", res);
	return 0;
}
/* should never be called */
static void __exit tpd_device_exit(void)
{
	TPD_DMESG("MediaTek touch panel driver exit\n");
	/* input_unregister_device(tpd->dev); */
	platform_driver_unregister(&tpd_driver);
}

late_initcall(tpd_device_init);
module_exit(tpd_device_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MediaTek touch panel driver");
MODULE_AUTHOR("Kirby Wu<kirby.wu@mediatek.com>");
