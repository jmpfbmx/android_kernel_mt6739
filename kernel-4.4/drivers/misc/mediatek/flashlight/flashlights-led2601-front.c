
#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

#include <linux/types.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <linux/pinctrl/consumer.h>

#include "flashlight-core.h"
#include "flashlight-dt.h"

/* define device tree */
#ifndef LED2601_FRONT_DTNAME
#define LED2601_FRONT_DTNAME "mediatek,flashlights_led2601_front"
#endif
//Begin modified by zhanyong.yin for XR5424731 on 2017/10/19
#ifndef LED2601_DTNAME_I2C
#define LED2601_DTNAME_I2C "mediatek,flashlights_led2601_i2c"
#endif
/* TODO: define driver name */
#define LED2601_FRONT_NAME "flashlights-led2601-front"

/* define registers */

/* define mutex and work queue */
static DEFINE_MUTEX(led2601_mutex);
static struct work_struct led2601_work;

/* define pinctrl */


#define LED2601_PINCTRL_STATE_FRONT_FLASHLIGHT_HIGH 		"front_flashlight_high" 
#define LED2601_PINCTRL_STATE_FRONT_FLASHLIGHT_LOW  		"front_flashlight_low"

static struct pinctrl *led2601_pinctrl;
static struct pinctrl_state *led2601_flashlight_high;
static struct pinctrl_state *led2601_flashlight_low;

//End modified by zhanyong.yin for XR5424731 on 2017/10/19
/* define usage count */
static int use_count;

static int g_flash_duty = -1;

/* platform data */
struct led2601_platform_data {
	int channel_num;
	struct flashlight_device_id *dev_id;
};


static int led2601_pinctrl_init(struct platform_device *pdev)
{
	int ret = 0;

	/* get pinctrl */
	led2601_pinctrl = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(led2601_pinctrl)) {
		pr_err("Failed to get flashlight pinctrl.\n");
		ret = PTR_ERR(led2601_pinctrl);
	}

	/*  rear flashlight pin initialization */
	led2601_flashlight_high = pinctrl_lookup_state(led2601_pinctrl, LED2601_PINCTRL_STATE_FRONT_FLASHLIGHT_HIGH);
	if (IS_ERR(led2601_flashlight_high)) {
		pr_err("Failed to init (%s)\n", LED2601_PINCTRL_STATE_FRONT_FLASHLIGHT_HIGH);
		ret = PTR_ERR(led2601_flashlight_high);
	}
	led2601_flashlight_low = pinctrl_lookup_state(led2601_pinctrl, LED2601_PINCTRL_STATE_FRONT_FLASHLIGHT_LOW);
	if (IS_ERR(led2601_flashlight_low)) {
		pr_err("Failed to init (%s)\n", LED2601_PINCTRL_STATE_FRONT_FLASHLIGHT_LOW);
		ret = PTR_ERR(led2601_flashlight_low);
	}

	
	return ret;
}

static int led2601_pinctrl_set(int state)
{
	int ret = 0;

	if (IS_ERR(led2601_pinctrl)) {
		pr_err("pinctrl is not available\n");
		return -1;
	}

	if (!state  && !IS_ERR(led2601_flashlight_low))
		ret = pinctrl_select_state(led2601_pinctrl, led2601_flashlight_low);
	else if (state  && !IS_ERR(led2601_flashlight_high))
		ret = pinctrl_select_state(led2601_pinctrl, led2601_flashlight_high);
	else
		pr_err("set err, state(%d)\n", state);

	return ret;
}


/* flashlight enable function */
static int led2601_enable(void)
{

	if (g_flash_duty < 0) 
		led2601_pinctrl_set(0);
	else
		led2601_pinctrl_set(1);
	//End added by zhanyong.yin for XR5424731 on 2017/10/19
	return 0;
}

/* flashlight disable function */
static int led2601_disable(void)
{
	return led2601_pinctrl_set(0);
}

/* set flashlight level */
static int led2601_set_level(int level)
{
	g_flash_duty = level;
	return 0;
}

/* flashlight init */
static int led2601_init(void)
{

	return led2601_pinctrl_set(0);
}

/* flashlight uninit */
static int led2601_uninit(void)
{

	return led2601_pinctrl_set( 0 );
}

static struct hrtimer led2601_timer;
static unsigned int led2601_timeout_ms;

static void led2601_work_disable(struct work_struct *data)
{
	pr_debug("work queue callback\n");
	led2601_disable();
}

static enum hrtimer_restart led2601_timer_func(struct hrtimer *timer)
{
	schedule_work(&led2601_work);
	return HRTIMER_NORESTART;
}


static int led2601_ioctl(unsigned int cmd, unsigned long arg)
{
	struct flashlight_dev_arg *fl_arg;
	int channel;
	ktime_t ktime;

	fl_arg = (struct flashlight_dev_arg *)arg;
	channel = fl_arg->channel;
	
	switch (cmd) {
	case FLASH_IOC_SET_TIME_OUT_TIME_MS:
		pr_info("FLASH_IOC_SET_TIME_OUT_TIME_MS(%d): %d\n",
				channel, (int)fl_arg->arg);
		led2601_timeout_ms = fl_arg->arg;
		break;

	case FLASH_IOC_SET_DUTY:
		pr_info("FLASH_IOC_SET_DUTY(%d): %d\n",
				channel, (int)fl_arg->arg);
		led2601_set_level(fl_arg->arg);
		break;

	case FLASH_IOC_SET_ONOFF:
		pr_info("FLASH_IOC_SET_ONOFF(%d): %d\n",
				channel, (int)fl_arg->arg);
		if (fl_arg->arg == 1) {
			if (led2601_timeout_ms) {
				ktime = ktime_set(led2601_timeout_ms / 1000,
						(led2601_timeout_ms % 1000) * 1000000);
				hrtimer_start(&led2601_timer, ktime, HRTIMER_MODE_REL);
			}
			led2601_enable();
		} else {
			led2601_disable();
			hrtimer_cancel(&led2601_timer);
		}
		break;
	default:
		pr_info("No such command and arg(%d): (%d, %d)\n",
				channel, _IOC_NR(cmd), (int)fl_arg->arg);
		return -ENOTTY;
	}

	return 0;
}

static int led2601_open(void)
{
	/* Move to set driver for saving power */
	return 0;
}

static int led2601_release(void)
{
	/* Move to set driver for saving power */
	return 0;
}

static int led2601_set_driver(int set)
{
	int ret = 0;

	/* set chip and usage count */
	mutex_lock(&led2601_mutex);
	if (set) {
		if (!use_count)
			ret = led2601_init();
		use_count++;
		pr_debug("Set driver: %d\n", use_count);
	} else {
		use_count--;
		if (!use_count)
			ret = led2601_uninit();
		if (use_count < 0)
			use_count = 0;
		pr_debug("Unset driver: %d\n", use_count);
	}
	mutex_unlock(&led2601_mutex);

	return ret;
}

static ssize_t led2601_strobe_store(struct flashlight_arg arg)
{
	led2601_set_driver(1);
	led2601_set_level(arg.level);
	led2601_timeout_ms = 0;
	led2601_enable();
	msleep(arg.dur);
	led2601_disable();
	led2601_set_driver(0);

	return 0;
}

static struct flashlight_operations led2601_ops = {
	led2601_open,
	led2601_release,
	led2601_ioctl,
	led2601_strobe_store,
	led2601_set_driver
};


static int led2601_chip_init(void)
{
	/* NOTE: Chip initialication move to "set driver" operation for power saving issue.
	 * led2601_init();
	 */

	return 0;
}

static int led2601_parse_dt(struct device *dev,
		struct led2601_platform_data *pdata)
{
	struct device_node *np, *cnp;
	u32 decouple = 0;
	int i = 0;

	if (!dev || !dev->of_node || !pdata)
		return -ENODEV;

	np = dev->of_node;

	pdata->channel_num = of_get_child_count(np);
	if (!pdata->channel_num) {
		pr_info("Parse no dt, node.\n");
		return 0;
	}
	pr_info("Channel number(%d).\n", pdata->channel_num);

	if (of_property_read_u32(np, "decouple", &decouple))
		pr_info("Parse no dt, decouple.\n");

	pdata->dev_id = devm_kzalloc(dev,
			pdata->channel_num * sizeof(struct flashlight_device_id),
			GFP_KERNEL);
	if (!pdata->dev_id)
		return -ENOMEM;

	for_each_child_of_node(np, cnp) {
		if (of_property_read_u32(cnp, "type", &pdata->dev_id[i].type))
			goto err_node_put;
		if (of_property_read_u32(cnp, "ct", &pdata->dev_id[i].ct))
			goto err_node_put;
		if (of_property_read_u32(cnp, "part", &pdata->dev_id[i].part))
			goto err_node_put;
		snprintf(pdata->dev_id[i].name, FLASHLIGHT_NAME_SIZE, LED2601_FRONT_NAME);
		pdata->dev_id[i].channel = i;
		pdata->dev_id[i].decouple = decouple;

		pr_info("Parse dt (type,ct,part,name,channel,decouple)=(%d,%d,%d,%s,%d,%d).\n",
				pdata->dev_id[i].type, pdata->dev_id[i].ct,
				pdata->dev_id[i].part, pdata->dev_id[i].name,
				pdata->dev_id[i].channel, pdata->dev_id[i].decouple);
		i++;
	}

	return 0;

err_node_put:
	of_node_put(cnp);
	return -EINVAL;
}

static int led2601_probe(struct platform_device *pdev)
{
	struct led2601_platform_data *pdata = dev_get_platdata(&pdev->dev);
	int err;
	int i;

	pr_debug("Probe start.\n");

	/* init pinctrl */
	if (led2601_pinctrl_init(pdev)) {
		pr_debug("Failed to init pinctrl.\n");
		err = -EFAULT;
		goto err;
	}

	/* init platform data */
	if (!pdata) {
		pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata) {
			err = -ENOMEM;
			goto err;
		}
		pdev->dev.platform_data = pdata;
		err = led2601_parse_dt(&pdev->dev, pdata);
		if (err)
			goto err;
	}

	/* init work queue */
	INIT_WORK(&led2601_work, led2601_work_disable);

	/* init timer */
	hrtimer_init(&led2601_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	led2601_timer.function = led2601_timer_func;
	led2601_timeout_ms = 100;

	/* init chip hw */
	led2601_chip_init();

	/* clear usage count */
	use_count = 0;

	/* register flashlight device */
	if (pdata->channel_num) {
		for (i = 0; i < pdata->channel_num; i++)
			if (flashlight_dev_register_by_device_id(&pdata->dev_id[i], &led2601_ops)) {
				err = -EFAULT;
				goto err;
			}
	} else {
		if (flashlight_dev_register(LED2601_FRONT_NAME, &led2601_ops)) {
			err = -EFAULT;
			goto err;
		}
	}

	pr_debug("Probe done.\n");

	return 0;
err:
	return err;
}

static int led2601_remove(struct platform_device *pdev)
{
	struct led2601_platform_data *pdata = dev_get_platdata(&pdev->dev);
	int i;

	pr_debug("Remove start.\n");

	pdev->dev.platform_data = NULL;

	/* unregister flashlight device */
	if (pdata && pdata->channel_num)
		for (i = 0; i < pdata->channel_num; i++)
			flashlight_dev_unregister_by_device_id(&pdata->dev_id[i]);
	else
		flashlight_dev_unregister(LED2601_FRONT_NAME);

	/* flush work queue */
	flush_work(&led2601_work);

	pr_debug("Remove done.\n");

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id led2601_gpio_of_match[] = {
	{.compatible = LED2601_FRONT_DTNAME},
	{},
};
MODULE_DEVICE_TABLE(of, led2601_gpio_of_match);
#else
static struct platform_device led2601_gpio_platform_device[] = {
	{
		.name = LED2601_FRONT_NAME,
		.id = 0,
		.dev = {}
	},
	{}
};
MODULE_DEVICE_TABLE(platform, led2601_gpio_platform_device);
#endif

static struct platform_driver led2601_platform_driver = {
	.probe = led2601_probe,
	.remove = led2601_remove,
	.driver = {
		.name = LED2601_FRONT_NAME,
		.owner = THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = led2601_gpio_of_match,
#endif
	},
};

static int __init flashlight_led2601_init(void)
{
	int ret;

	pr_debug("Init start.\n");

#ifndef CONFIG_OF
	ret = platform_device_register(&led2601_gpio_platform_device);
	if (ret) {
		pr_err("Failed to register platform device\n");
		return ret;
	}
#endif

	ret = platform_driver_register(&led2601_platform_driver);
	if (ret) {
		pr_err("Failed to register platform driver\n");
		return ret;
	}

	pr_debug("Init done.\n");

	return 0;
}

static void __exit flashlight_led2601_exit(void)
{
	pr_debug("Exit start.\n");

	platform_driver_unregister(&led2601_platform_driver);

	pr_debug("Exit done.\n");
}

module_init(flashlight_led2601_init);
module_exit(flashlight_led2601_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Simon Wang <Simon-TCH.Wang@mediatek.com>");
MODULE_DESCRIPTION("MTK Flashlight LED2601 GPIO Driver");

