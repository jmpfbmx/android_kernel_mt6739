#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <mach/gpio_const.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>

/* ioctl command, use to open or close device */
#define AW87318_OPEN                     _IOC(_IOC_NONE, 0x07, 0x03, 0)
#define AW87318_CLOSE                    _IOC(_IOC_NONE, 0x07, 0x04, 0)
/* awinic AW87318 pa control pin               */
#define AW87318_CONTROL_PIN              (24) //GPIO24
/* log tag of AW87318 pa                       */
#define AW87318_LOG_TAG                  "aw87318"
/* enable debug information                   */
#define AW87318_DEBUG
#ifdef AW87318_DEBUG
#define AW87318_LOGD(format, args...)    do                              \
                {                                                        \
                    printk(KERN_DEBUG AW87318_LOG_TAG format, ##args);   \
                }while(0)
#define AW87318_LOGE(format, args...)    do                              \
                {                                                        \
                    printk(KERN_ERR AW87318_LOG_TAG format, ##args);     \
                }while(0)
#else
#define AW87318_LOGD(format, args...)    do{}while(0)
#define AW87318_LOGE(format, args...)    do{}while(0)
#endif

static dev_t aw87318_devno;
static struct cdev aw87318_cdev;
static struct class *aw87318_class = NULL;
static struct device *aw87318_device = NULL;
struct pinctrl *aw87318_pinctrl;
struct pinctrl_state *pa_ctrl_pin_high;
struct pinctrl_state *pa_ctrl_pin_low;
unsigned int g_gpio_en_pin;

/************************************************************************************************************************************
 * Mode  |  Magnification(V/V)  |                      AGC Power(W)                       |  Multi-level ACG function  |  Receiver
 *       |                      |  RL=8omh+33uH  R=6omh+33uH  RL=4omh+15uH  RL=3omh+15uH  |                            |
 * 1     |  16.6                |  1.2           1.6          -             -             |  support                   |
 * 2     |  16.6                |  1.1           1.5          -             -             |  support                   |
 * 3     |  16.6                |  1.0           1.3          2.0           -             |  support                   |
 * 4     |  16.6                |  0.9           1.2          1.8           -             |  support                   |
 * 5     |  16.6                |  0.8           1.0          1.6           2.0           |  support                   |
 * 6     |  16.6                |  0.7           0.9          1.4           1.8           |  support                   |
 * 7     |  16.6                |  0.6           0.8          1.2           1.6           |  support                   |
 * 8     |  1                   |                                                         |                            |  support
 * 9     |  3                   |                                                         |                            |  support
 * 10    |  16.6                |  1.75W@THD=1%  2.05W@THD=1%  2.4W@THD=1%  2.35W@THD=1%  |                            |
 ************************************************************************************************************************************/
typedef enum {
    AW87318_MODE_MIX = 0,
    AW87318_MODE_1 = 1,
    AW87318_MODE_2,
    AW87318_MODE_3,
    AW87318_MODE_4,
    AW87318_MODE_5,
    AW87318_MODE_6,
    AW87318_MODE_7,
    AW87318_MODE_8,
    AW87318_MODE_9,
    AW87318_MODE_10,
    AW87318_MODE_CLOSE,
    AW87318_MODE_MAX,
} aw87318_mode;

struct of_device_id aw87318_of_match[] = {
    { .compatible = "mediatek,aw87318-pa", },
    {},
};

static int aw87318_io_config(struct platform_device *dev)
{
    struct pinctrl_state *pins_default = NULL;
    struct device_node *node = NULL;
    int err = 0;

    AW87318_LOGD("+%s\n", __func__);

    /* get pin config from device tree */
    aw87318_pinctrl = devm_pinctrl_get(&dev->dev);
    if (IS_ERR(aw87318_pinctrl)) {
        AW87318_LOGE("devm_pinctrl_get error\n");
        return -1;
    }
    /* default pin */
    pins_default = pinctrl_lookup_state(aw87318_pinctrl, "pin_default");
    if (IS_ERR(pins_default)) {
        AW87318_LOGE("pinctrl_lookup_state default error\n");
    }
    /* pa en pin high status */
    pa_ctrl_pin_high = pinctrl_lookup_state(aw87318_pinctrl, "en_pin_high");
    if (IS_ERR(pa_ctrl_pin_high)) {
        AW87318_LOGE("pinctrl_lookup_state en_pin_high error\n");
        return -1;
    }
    /* pa en pin low status */
    pa_ctrl_pin_low = pinctrl_lookup_state(aw87318_pinctrl, "en_pin_low");
    if (IS_ERR(pa_ctrl_pin_low)) {
        AW87318_LOGE("pinctrl_lookup_state en_pin_low error\n");
        return -1;
    }
    pinctrl_select_state(aw87318_pinctrl, pa_ctrl_pin_low);

    node = of_find_matching_node(node, aw87318_of_match);

    /* init the aw87318 enable pin */
    if (node) {
        g_gpio_en_pin = of_get_named_gpio(node, "deb-gpios", 0);
        err = gpio_request(g_gpio_en_pin, "aw87318_en");
        if (err) {
            AW87318_LOGE("%s request gpio error !\n", __func__);
            return err;
        }
    } else {
        AW87318_LOGE("%s get aw87318 dts node error !\n", __func__);
    }

    AW87318_LOGE("-%s, requested gpio = %d\n", __func__, (int)g_gpio_en_pin);

    gpio_direction_output(g_gpio_en_pin, 0);

    return 0;
}

static int aw87318_set_mode(aw87318_mode mode)
{
    int i;
    
    AW87318_LOGD("+%s, mode = %d\n", __func__, mode);
    
    if ((mode >= AW87318_MODE_MAX)
        || (mode <= AW87318_MODE_MIX)) {
        AW87318_LOGE("mode argument is error !\n");
        return -1;
    }

    if (mode == AW87318_MODE_CLOSE) {
        // pinctrl_select_state(aw87318_pinctrl, pa_ctrl_pin_low);
        gpio_set_value(g_gpio_en_pin, 0);
        msleep(1);                      // sleep 1ms, make sure the aw87318 is closed
    } else {
        msleep(1);                      // 1ms use to close the mode before set it
        for (i = 0; i < mode; ++i)
        {
            ndelay(2000);               // 2us
            // pinctrl_select_state(aw87318_pinctrl, pa_ctrl_pin_low);
            gpio_set_value(g_gpio_en_pin, 0);
            ndelay(2000);               // 2us
            // pinctrl_select_state(aw87318_pinctrl, pa_ctrl_pin_high);
            gpio_set_value(g_gpio_en_pin, 1);
        }
         //change by longquan.yuan for CTS audio issue on 18-2-7 begin
        msleep(25);                     // 42ms to 50ms, make sure the mode is built
        //change by longquan.yuan for CTS audio issue on 18-2-7 end
    }

    return 0;
}

static int aw87318_ops_open(struct inode *inode, struct file *file)
{
    AW87318_LOGD("+%s\n", __func__);
    return 0;
}

static int aw87318_ops_release(struct inode *inode, struct file *file)
{
   AW87318_LOGD("+%s\n", __func__);
    return 0;
}

static long aw87318_ops_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    long ret = 0;

    AW87318_LOGD("+%s, cmd = %d, arg = %ld\n", __func__, cmd, arg);

    switch (cmd)
    {
    case AW87318_OPEN:
        AW87318_LOGD("+%s, open\n", __func__);
        ret = aw87318_set_mode((aw87318_mode)arg);
        break;
    case AW87318_CLOSE:
        AW87318_LOGD("+%s, close\n", __func__);
        ret = aw87318_set_mode((aw87318_mode)arg);
        break;

    default:
        ret = -1;
        break;
    }

    return ret;
}

static struct file_operations aw87318_fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = aw87318_ops_ioctl,
    .open           = aw87318_ops_open,
    .release        = aw87318_ops_release,
};

static int aw87318_drv_probe(struct platform_device *dev)
{
    int ret = 0, err = 0;

    AW87318_LOGD("+%s\n", __func__);

    if (aw87318_io_config(dev) != 0) {
        AW87318_LOGE("pa control pin init fail\n");
        return -1;
    }

    ret = alloc_chrdev_region(&aw87318_devno, 0, 1, "aw87318");
    if (ret) {
        AW87318_LOGE("alloc_chrdev_region fail: %d\n", ret);
        goto aw87318_probe_error;
    } else {
        AW87318_LOGD("major: %d, minor: %d\n", MAJOR(aw87318_devno), MINOR(aw87318_devno));
    }
    cdev_init(&aw87318_cdev, &aw87318_fops);
    aw87318_cdev.owner = THIS_MODULE;
    err = cdev_add(&aw87318_cdev, aw87318_devno, 1);
    if (err) {
        AW87318_LOGE("cdev_add fail: %d\n", err);
        goto aw87318_probe_error;
    }

    aw87318_class = class_create(THIS_MODULE, "aw87318");
    if (IS_ERR(aw87318_class)) {
        AW87318_LOGE("Unable to create class\n");
        goto aw87318_probe_error;
    }

    aw87318_device = device_create(aw87318_class, NULL, aw87318_devno, NULL, "aw87318");
    if(aw87318_device == NULL) {
        AW87318_LOGE("device_create fail\n");
        goto aw87318_probe_error;
    }

    return 0;

aw87318_probe_error:
    if (err == 0)
        cdev_del(&aw87318_cdev);
    if (ret == 0)
        unregister_chrdev_region(aw87318_devno, 1);

    return -1;
}

static int aw87318_drv_remove(struct platform_device *dev)
{
    AW87318_LOGD("+%s\n", __func__);
    cdev_del(&aw87318_cdev);
    unregister_chrdev_region(aw87318_devno, 1);
    device_destroy(aw87318_class, aw87318_devno);
    class_destroy(aw87318_class);

    return 0;
}

static struct platform_driver aw87318_platform_driver =
{
    .probe      = aw87318_drv_probe,
    .remove     = aw87318_drv_remove,
    .driver     = {
        .name   = "aw87318",
        .of_match_table = aw87318_of_match,
    },
};

static int __init aw87318_init(void)
{
    int ret = 0;
    AW87318_LOGD("+%s\n", __func__);

    ret = platform_driver_register(&aw87318_platform_driver);
    if(ret) {
        AW87318_LOGE("platform_driver_register fail\n");
        return ret;
    }
    return ret;
}

static void __exit aw87318_exit(void)
{
    AW87318_LOGD("+%s\n", __func__);
    platform_driver_unregister(&aw87318_platform_driver);
}

module_init(aw87318_init);
module_exit(aw87318_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wingtech Multimedia Group");
MODULE_DESCRIPTION("aw87318 control Driver");
