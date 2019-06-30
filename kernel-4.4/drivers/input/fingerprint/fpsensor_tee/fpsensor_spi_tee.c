#include <linux/device.h>
#include <linux/input.h>
#include <linux/mutex.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/fb.h>
#include <linux/ioctl.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/completion.h>
#include <linux/gpio.h>

#include <linux/timer.h>
#include <linux/notifier.h>
#include <linux/fb.h>
#include <linux/pm_qos.h>
#include <linux/cpufreq.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#else
#include <linux/notifier.h>
#endif

#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#endif

#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif

#ifdef CONFIG_MTK_CLKMGR
#include "mach/mt_clkmgr.h"
#else
#include <linux/clk.h>
#endif

#include <net/sock.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <linux/wakelock.h>

/* MTK header */
//#include "mt_spi.h"
//#include "mt_spi_hal.h"
//#include "mt_gpio.h"
//#include "mach/gpio_const.h"

/* MTK header */
#ifndef CONFIG_SPI_MT65XX
#include "mtk_spi.h"
#include "mtk_spi_hal.h"
#endif
#include "mtk_gpio.h"
#include "fpsensor_spi_tee.h"

/*device name*/
#define FPSENSOR_DEV_NAME       "fpsensor"
#define FPSENSOR_CLASS_NAME     "fpsensor"
#define FPSENSOR_MAJOR          0
#define N_SPI_MINORS            32    /* ... up to 256 */

#define FPSENSOR_SPI_VERSION    "fpsensor_spi_tee_v0.1"
#define FPSENSOR_INPUT_NAME     "fpsensor_keys"

/**************************feature control******************************/
#define FPSENSOR_IOCTL    1
#define FPSENSOR_SYSFS    0
#define FPSENSOR_INPUT    0
/***********************input *************************/
#ifndef FPSENSOR_INPUT_HOME_KEY
/* on MTK EVB board, home key has been redefine to KEY_HOMEPAGE! */
/* double check the define on customer board!!! */
#define FPSENSOR_INPUT_HOME_KEY     KEY_HOMEPAGE /* KEY_HOME */
#define FPSENSOR_INPUT_MENU_KEY     KEY_MENU
#define FPSENSOR_INPUT_BACK_KEY     KEY_BACK
#define FPSENSOR_INPUT_FF_KEY       KEY_POWER
#define FPSENSOR_INPUT_CAMERA_KEY   KEY_CAMERA
#define FPSENSOR_INPUT_OTHER_KEY    KEY_VOLUMEDOWN  /* temporary key value for capture use */
#endif

#define FPSENSOR_NAV_UP_KEY     19//KEY_UP
#define FPSENSOR_NAV_DOWN_KEY   20//KEY_DOWN
#define FPSENSOR_NAV_LEFT_KEY   21//KEY_LEFT
#define FPSENSOR_NAV_RIGHT_KEY  22//KEY_RIGHT
#define FPSENSOR_NAV_TAP_KEY    23
/***********************GPIO setting port layer*************************/
/* customer hardware port layer, please change according to customer's hardware */
#define GPIO_PIN_IRQ   51
extern void mt_spi_disable_master_clk(struct spi_device *spidev);
extern void mt_spi_enable_master_clk(struct spi_device *spidev);
/*** begin: added by liuhe.zhong for fingerprint hardware information on 2018.01.30 ***/
extern unsigned char fp_info[256];
/*** end:   added by liuhe.zhong for fingerprint hardware information on 2018.01.30 ***/
/*** begin: added by liuhe.zhong for close spi0 clk when MS has no fingerprint on 2018.02.27 ***/
extern unsigned char board_id;
/*** end:   added by liuhe.zhong for close spi0 clk when MS has no fingerprint on 2018.02.27 ***/
/*************************************************************/
static struct wake_lock fpsensor_timeout_wakelock;
fpsensor_data_t *g_fpsensor = NULL;
EXPORT_SYMBOL(g_fpsensor);
#define ROUND_UP(x, align)        ((x+(align-1))&~(align-1))

/**************************debug******************************/
#define ERR_LOG  (0)
#define INFO_LOG (1)
#define DEBUG_LOG (2)

/* debug log setting */
static u8 fpsensor_debug_level = ERR_LOG;

#define fpsensor_debug(level, fmt, args...) do { \
        if (fpsensor_debug_level >= level) {\
            printk( "[fpsensor] " fmt, ##args); \
        } \
    } while (0)

#define FUNC_ENTRY()  fpsensor_debug(DEBUG_LOG, "%s, %d, entry\n", __func__, __LINE__)
#define FUNC_EXIT()  fpsensor_debug(DEBUG_LOG, "%s, %d, exit\n", __func__, __LINE__)

/* -------------------------------------------------------------------- */
/* fingerprint chip hardware configuration                              */
/* -------------------------------------------------------------------- */
static DEFINE_MUTEX(spidev_set_gpio_mutex);
#if 0
static void spidev_gpio_as_int(fpsensor_data_t *fpsensor)
{
    fpsensor_trace( "[fpsensor]----%s---\n", __func__);
    mutex_lock(&spidev_set_gpio_mutex);
    printk("[fpsensor]spidev_gpio_as_int\n");
    pinctrl_select_state(fpsensor->pinctrl1, fpsensor->eint_as_int);
    mutex_unlock(&spidev_set_gpio_mutex);
}
#endif
void fpsensor_gpio_output_dts(int gpio, int level)
{
    mutex_lock(&spidev_set_gpio_mutex);
    printk("[fpsensor]fpsensor_gpio_output_dts: gpio= %d, level = %d\n", gpio, level);
    if (gpio == FPSENSOR_RST_PIN) {
        if (level) {
            pinctrl_select_state(g_fpsensor->pinctrl1, g_fpsensor->fp_rst_high);
        } else {
            pinctrl_select_state(g_fpsensor->pinctrl1, g_fpsensor->fp_rst_low);
        }
    }	
    mutex_unlock(&spidev_set_gpio_mutex);
}

int fpsensor_gpio_wirte(int gpio, int value)
{
    fpsensor_gpio_output_dts(gpio, value);
    return 0;
}
int fpsensor_gpio_read(int gpio)
{
    return gpio_get_value(gpio);
}

int fpsensor_spidev_dts_init(fpsensor_data_t *fpsensor)
{
    struct device_node *node;
    int ret = 0;
    fpsensor_printk( "%s\n", __func__);
    node = of_find_compatible_node(NULL, NULL, "mediatek,fingerprint");
    if (node) {
        fpsensor->fp_rst_low = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_finger_rst_low");
        if (IS_ERR(fpsensor->fp_rst_low)) {
            ret = PTR_ERR(fpsensor->fp_rst_low);
            fpsensor_error("fpensor Cannot find fp pinctrl fpsensor_finger_rst_low!\n");
            return ret;
        }
        fpsensor->fp_rst_high = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_finger_rst_high");
        if (IS_ERR(fpsensor->fp_rst_high)) {
            ret = PTR_ERR(fpsensor->fp_rst_high);
            fpsensor_error( "fpsensor Cannot find fp pinctrl fpsensor_finger_rst_high!\n");
            return ret;
        }
	fpsensor->pins_power_off = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_finger_power_low");
        if (IS_ERR(fpsensor->pins_power_off)) {
            ret = PTR_ERR(fpsensor->pins_power_off);
            fpsensor_error("fpensor Cannot find fp pinctrl fpsensor_finger_power_low!\n");
            return ret;
        }
        fpsensor->pins_power_on = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_finger_power_high");
        if (IS_ERR(fpsensor->pins_power_on)) {
            ret = PTR_ERR(fpsensor->pins_power_on);
            fpsensor_error( "fpsensor Cannot find fp pinctrl fpsensor_finger_power_high!\n");
            return ret;
        }

        fpsensor->eint_as_int = pinctrl_lookup_state(fpsensor->pinctrl1,"fpsensor_eint"); //eint_in_low; eint
        if (IS_ERR(fpsensor->eint_as_int)) {
            ret = PTR_ERR(fpsensor->eint_as_int);
            fpsensor_error( "fpsensor Cannot find fp pinctrl fpsensor_eint!\n");
            return ret;
        }
		
	fpsensor->eint_in_low = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_eint_in_low");
        if (IS_ERR(fpsensor->eint_as_int)) {
            ret = PTR_ERR(fpsensor->eint_as_int);
            fpsensor_error("fpsensor Cannot find fp pinctrl fpsensor_eint_in_low!\n");
            return ret;
        }
		
        fpsensor->eint_in_float = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_eint_in_float");
        if (IS_ERR(fpsensor->eint_in_float)) {
            ret = PTR_ERR(fpsensor->eint_in_float);
            fpsensor_error(" Cannot find fp pinctrl eint_in_float!\n");
            return ret;
		}
		
        fpsensor->fp_cs_set = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_spi_cs_set");
        if (IS_ERR(fpsensor->fp_cs_set)) {
            ret = PTR_ERR(fpsensor->fp_cs_set);
            fpsensor_error("fpensor Cannot find fp pinctrl fp_cs_set111!\n");
            return ret;
        }
        fpsensor->fp_cs_clr = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_spi_cs_clr");
        if (IS_ERR(fpsensor->fp_cs_clr)) {
            ret = PTR_ERR(fpsensor->fp_cs_clr);
            fpsensor_error( "fpsensor Cannot find fp pinctrl fp_cs_clr111!\n");
            return ret;
        }

        fpsensor->fp_spi_mosi_set = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_spi_mo_set");
        if (IS_ERR(fpsensor->fp_spi_mosi_set)) {
            ret = PTR_ERR(fpsensor->fp_spi_mosi_set);
            fpsensor_error( "fpsensor Cannot find fp pinctrl fp_spi_mosi_set!\n");
            return ret;
        }
        fpsensor->fp_spi_mosi_clr = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_spi_mo_clr");
        if (IS_ERR(fpsensor->fp_spi_mosi_clr)) {
            ret = PTR_ERR(fpsensor->fp_spi_mosi_clr);
            fpsensor_error( "fpsensor Cannot find fp pinctrl fp_spi_mosi_clr!\n");
            return ret;
        }
        fpsensor->fp_spi_miso_set = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_spi_mi_set");
        if (IS_ERR(fpsensor->fp_spi_miso_set)) {
            ret = PTR_ERR(fpsensor->fp_spi_miso_set);
            fpsensor_error("fpensor Cannot find fp pinctrl fp_spi_miso_set!\n");
            return ret;
        }
          fpsensor->fp_spi_miso_clr = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_spi_mi_clr");
        if (IS_ERR(fpsensor->fp_spi_miso_clr)) {
            ret = PTR_ERR(fpsensor->fp_spi_miso_clr);
            fpsensor_error("fpensor Cannot find fp pinctrl fp_spi_miso_clr!\n");
            return ret;
        }

        fpsensor->fp_spi_clk_set = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_spi_mclk_set");
        if (IS_ERR(fpsensor->fp_spi_clk_set)) {
            ret = PTR_ERR(fpsensor->fp_spi_clk_set);
            fpsensor_error("fpensor Cannot find fp pinctrl fp_spi_clk_set!\n");
            return ret;
        }
        fpsensor->fp_spi_clk_clr = pinctrl_lookup_state(fpsensor->pinctrl1, "fpsensor_spi_mclk_clr");
        if (IS_ERR(fpsensor->fp_spi_clk_clr)) {
            ret = PTR_ERR(fpsensor->fp_spi_clk_clr);
            fpsensor_error("fpensor Cannot find fp pinctrl fp_spi_set!\n");
            return ret;
        }
        
        pinctrl_select_state(fpsensor->pinctrl1,fpsensor->fp_cs_set);
        pinctrl_select_state(fpsensor->pinctrl1,fpsensor->fp_spi_mosi_set);
        pinctrl_select_state(fpsensor->pinctrl1,fpsensor->fp_spi_miso_set);
        pinctrl_select_state(fpsensor->pinctrl1,fpsensor->fp_spi_clk_set);
    } else {
        fpsensor_error("fpensor Cannot find node!\n");
    }
    return 0;
}
/* delay us after reset */
static void fpsensor_hw_reset(int delay)
{
    FUNC_ENTRY();

    fpsensor_gpio_wirte(FPSENSOR_RST_PIN,    1);
    udelay(100);
    fpsensor_gpio_wirte(FPSENSOR_RST_PIN,  0);
    udelay(1000);
    fpsensor_gpio_wirte(FPSENSOR_RST_PIN,  1);
    if (delay) {
        /* delay is configurable */
        udelay(delay);
    }

    FUNC_EXIT();
    return ;
}

static void fpsensor_spi_clk_enable(u8 bonoff)
{
#ifdef CONFIG_MTK_CLKMGR
	if (bonoff)
		enable_clock(MT_CG_PERI_SPI0, "spi");
	else
		disable_clock(MT_CG_PERI_SPI0, "spi");

#else

	static int count;
	if (bonoff && (count == 0)) {
		 //mt_spi_enable_master_clk(g_fpsensor->spi);
		clk_prepare_enable(g_fpsensor->spiclk);
		count = 1;
	} else if ((count > 0) && (bonoff == 0)) {
		//mt_spi_disable_master_clk(g_fpsensor->spi);
		clk_disable_unprepare(g_fpsensor->spiclk);
		count = 0;
	}
#endif
}
 static void fpsensor_hw_power_enable(u8 onoff)
 {
     static int enable = 1;
     if (onoff && enable)
     {
         pinctrl_select_state(g_fpsensor->pinctrl1, g_fpsensor->pins_power_on);
	 fpsensor_printk("%s,----power enable on!---\n", __func__);
         enable = 0;
     }
     else if (!onoff && !enable)
     {
         pinctrl_select_state(g_fpsensor->pinctrl1, g_fpsensor->pins_power_off);
        fpsensor_printk("%s,----power enable off!---\n", __func__);
         enable = 1;
     }
  
 }

static int fpsensor_irq_gpio_cfg(void)
{
    int error = 0;
    struct device_node *node;
    fpsensor_data_t *fpsensor;
    //u32 ints[2] = {0, 0};
    fpsensor_printk("%s\n", __func__);

    fpsensor = g_fpsensor;

   // spidev_gpio_as_int(fpsensor);

    node = of_find_compatible_node(NULL, NULL, "mediatek,fpsensor_fp_eint");
    if ( node) {
#if 0
        of_property_read_u32_array( node, "debounce", ints, ARRAY_SIZE(ints));
        // gpio_request(ints[0], "fpsensor-irq");
        // gpio_set_debounce(ints[0], ints[1]);
        fpsensor_printk("[fpsensor]ints[0] = %d,is irq_gpio , ints[1] = %d!!\n", ints[0], ints[1]);
        fpsensor->irq_gpio = ints[0];
#else
        fpsensor->irq_gpio = GPIO_PIN_IRQ;
#endif
        fpsensor->irq = irq_of_parse_and_map(node, 0);  // get irq number
        if (!fpsensor->irq) {
            printk("fpsensor irq_of_parse_and_map fail!!\n");
            return -EINVAL;
        }
        fpsensor_printk(" [fpsensor]fpsensor->irq= %d,fpsensor>irq_gpio = %d\n", fpsensor->irq,
                        fpsensor->irq_gpio);
    } else {
        printk("fpsensor null irq node!!\n");
        return -EINVAL;
    }

    return error;

}
static void fpsensor_enable_irq(fpsensor_data_t *fpsensor_dev)
{
    FUNC_ENTRY();
    setRcvIRQ(0);
    if (0 == fpsensor_dev->device_available) {
        fpsensor_debug(ERR_LOG, "%s, devices not available\n", __func__);
    } else {
        if (1 == fpsensor_dev->irq_count) {
            fpsensor_debug(ERR_LOG, "%s, irq already enabled\n", __func__);
        } else {
            enable_irq(fpsensor_dev->irq);
            fpsensor_dev->irq_count = 1;
            fpsensor_debug(INFO_LOG, "%s enable interrupt!\n", __func__);
        }
    }
    FUNC_EXIT();
    return;
}

static void fpsensor_disable_irq(fpsensor_data_t *fpsensor_dev)
{
    FUNC_ENTRY();

    if (0 == fpsensor_dev->device_available) {
        fpsensor_debug(ERR_LOG, "%s, devices not available\n", __func__);
    } else {
        if (0 == fpsensor_dev->irq_count) {
            fpsensor_debug(ERR_LOG, "%s, irq already disabled\n", __func__);
        } else {
            disable_irq(fpsensor_dev->irq);
            fpsensor_dev->irq_count = 0;
            fpsensor_debug(DEBUG_LOG, "%s disable interrupt!\n", __func__);
        }
    }
    setRcvIRQ(0);
    FUNC_EXIT();
    return;
}

/* -------------------------------------------------------------------- */
/* file operation function                                              */
/* -------------------------------------------------------------------- */

static ssize_t fpsensor_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    fpsensor_debug(ERR_LOG, "Not support read opertion in TEE version\n");
    return -EFAULT;
}

static ssize_t fpsensor_write(struct file *filp, const char __user *buf,
                              size_t count, loff_t *f_pos)
{
    fpsensor_debug(ERR_LOG, "Not support write opertion in TEE version\n");
    return -EFAULT;
}

static irqreturn_t fpsensor_irq(int irq, void *handle)
{
    fpsensor_data_t *fpsensor_dev = (fpsensor_data_t *)handle;

    wake_lock_timeout(&fpsensor_timeout_wakelock, msecs_to_jiffies(1000));
#if FPSENSOR_IOCTL
    setRcvIRQ(1);
#endif
    wake_up_interruptible(&fpsensor_dev->wq_irq_return);
    fpsensor_dev->sig_count++;

    return IRQ_HANDLED;
}

void setRcvIRQ(int val)
{
    fpsensor_data_t *fpsensor_dev = g_fpsensor;
    // fpsensor_debug(INFO_LOG, "[rickon]: %s befor val :  %d ; set val : %d   \n", __func__, fpsensor_dev-> RcvIRQ, val);
    fpsensor_dev-> RcvIRQ = val;
}

//#include <linux/dev_info.h>
//static struct devinfo_struct *s_DEVINFO_fpsensor;
static long fpsensor_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    fpsensor_data_t *fpsensor_dev = NULL;
    struct fpsensor_key fpsensor_key;
#if FPSENSOR_INPUT
    uint32_t key_event;
#endif
    int retval = 0;
    unsigned int val = 0;

    FUNC_ENTRY();
    fpsensor_debug(INFO_LOG, "[rickon]: fpsensor ioctl cmd : 0x%x \n", cmd );
    fpsensor_dev = (fpsensor_data_t *)filp->private_data;
    //clear cancel flag
    fpsensor_dev->cancel = 0 ;
    switch (cmd) {
    case FPSENSOR_IOC_INIT:
        fpsensor_debug(INFO_LOG, "%s: fpsensor init started======\n", __func__);
        wake_lock_init(&fpsensor_timeout_wakelock, WAKE_LOCK_SUSPEND, "fpsensor timeout wakelock");
        fpsensor_irq_gpio_cfg();
        retval = request_threaded_irq(fpsensor_dev->irq, NULL,fpsensor_irq, 
                                      IRQF_TRIGGER_RISING | IRQF_ONESHOT, dev_name(&(fpsensor_dev->pldev->dev)), fpsensor_dev);
        if (retval == 0) {
            fpsensor_debug(ERR_LOG, " irq thread reqquest success!\n");
        } else {
            fpsensor_debug(ERR_LOG, " irq thread request failed , retval =%d \n", retval);
        }
//Begin add by qiyang.yan for fpsensor wakeup on 2018/1/11
	enable_irq_wake(fpsensor_dev->irq);
//End add by qiyang.yan for fpsensor wakeup on 2018/1/11
        fpsensor_dev->device_available = 1;
        fpsensor_dev->irq_count = 1;
        fpsensor_disable_irq(fpsensor_dev);

        fpsensor_dev->sig_count = 0;
		/*
        s_DEVINFO_fpsensor = (struct devinfo_struct *) kmalloc(sizeof(struct devinfo_struct), GFP_KERNEL);
        s_DEVINFO_fpsensor->device_type = "FP";
        s_DEVINFO_fpsensor->device_module = "oflim";
        s_DEVINFO_fpsensor->device_vendor = "NULL";
        s_DEVINFO_fpsensor->device_ic = "LC1550";
        s_DEVINFO_fpsensor->device_version = "NULL";
        s_DEVINFO_fpsensor->device_info = "fingerprint";
        s_DEVINFO_fpsensor->device_used = DEVINFO_USED;
        devinfo_check_add_device(s_DEVINFO_fpsensor);
		*/
     //   wake_lock_init(&fpsensor_timeout_wakelock, WAKE_LOCK_SUSPEND, "fpsensor timeout wakelock");
        fpsensor_debug(INFO_LOG, "%s: fpsensor init finished======\n", __func__);
        break;

    case FPSENSOR_IOC_EXIT:
        fpsensor_disable_irq(fpsensor_dev);
/*
        if (fpsensor_dev->spi->irq) {
            free_irq(fpsensor_dev->spi->irq, fpsensor_dev);
            fpsensor_dev->irq_count = 0;
        }
*/
        fpsensor_dev->irq_count = 0;
        fpsensor_dev->device_available = 0;
        fpsensor_debug(INFO_LOG, "%s: fpsensor exit finished======\n", __func__);
        break;

    case FPSENSOR_IOC_RESET:
        fpsensor_debug(INFO_LOG, "%s: chip reset command\n", __func__);
        fpsensor_hw_reset(1250);
        break;

    case FPSENSOR_IOC_ENABLE_IRQ:
        fpsensor_debug(INFO_LOG, "%s: chip ENable IRQ command\n", __func__);
        fpsensor_enable_irq(fpsensor_dev);
        break;

    case FPSENSOR_IOC_DISABLE_IRQ:
        fpsensor_debug(INFO_LOG, "%s: chip disable IRQ command\n", __func__);
        fpsensor_disable_irq(fpsensor_dev);
        break;
    case FPSENSOR_IOC_GET_INT_VAL:
        val = gpio_get_value(GPIO_PIN_IRQ);
        if (copy_to_user((void __user *)arg, (void *)&val, sizeof(unsigned int))) {
            fpsensor_debug(ERR_LOG, "Failed to copy data to user\n");
            retval = -EFAULT;
            break;
        }
        retval = 0;
        break;
    case FPSENSOR_IOC_ENABLE_SPI_CLK:
        fpsensor_debug(INFO_LOG, "%s: ENABLE_SPI_CLK ======\n", __func__);
        fpsensor_spi_clk_enable(1);
        break;
    case FPSENSOR_IOC_DISABLE_SPI_CLK:
        fpsensor_debug(INFO_LOG, "%s: DISABLE_SPI_CLK ======\n", __func__);
        fpsensor_spi_clk_enable(0);
        break;

    case FPSENSOR_IOC_ENABLE_POWER:
        fpsensor_debug(INFO_LOG, "%s: FPSENSOR_IOC_ENABLE_POWER ======\n", __func__);
        fpsensor_hw_power_enable(1);
        break;

    case FPSENSOR_IOC_DISABLE_POWER:
        fpsensor_debug(INFO_LOG, "%s: FPSENSOR_IOC_DISABLE_POWER ======\n", __func__);
        fpsensor_hw_power_enable(0);
        break;


    case FPSENSOR_IOC_INPUT_KEY_EVENT:
        if (copy_from_user(&fpsensor_key, (struct fpsensor_key *)arg, sizeof(struct fpsensor_key))) {
            fpsensor_debug(ERR_LOG, "Failed to copy input key event from user to kernel\n");
            retval = -EFAULT;
            break;
        }
#if FPSENSOR_INPUT
        if (FPSENSOR_KEY_HOME == fpsensor_key.key) {
            key_event = FPSENSOR_INPUT_HOME_KEY;
        } else if (FPSENSOR_KEY_POWER == fpsensor_key.key) {
            key_event = FPSENSOR_INPUT_FF_KEY;
        } else if (FPSENSOR_KEY_CAPTURE == fpsensor_key.key) {
            key_event = FPSENSOR_INPUT_CAMERA_KEY;
        } else {
            /* add special key define */
            key_event = FPSENSOR_INPUT_OTHER_KEY;
        }
        fpsensor_debug(INFO_LOG, "%s: received key event[%d], key=%d, value=%d\n",
                       __func__, key_event, fpsensor_key.key, fpsensor_key.value);
        if ((FPSENSOR_KEY_POWER == fpsensor_key.key || FPSENSOR_KEY_CAPTURE == fpsensor_key.key)
            && (fpsensor_key.value == 1)) {
            input_report_key(fpsensor_dev->input, key_event, 1);
            input_sync(fpsensor_dev->input);
            input_report_key(fpsensor_dev->input, key_event, 0);
            input_sync(fpsensor_dev->input);
        } else if (FPSENSOR_KEY_UP == fpsensor_key.key) {
            input_report_key(fpsensor_dev->input, FPSENSOR_NAV_UP_KEY, 1);
            input_sync(fpsensor_dev->input);
            input_report_key(fpsensor_dev->input, FPSENSOR_NAV_UP_KEY, 0);
            input_sync(fpsensor_dev->input);
        } else if (FPSENSOR_KEY_DOWN == fpsensor_key.key) {
            input_report_key(fpsensor_dev->input, FPSENSOR_NAV_DOWN_KEY, 1);
            input_sync(fpsensor_dev->input);
            input_report_key(fpsensor_dev->input, FPSENSOR_NAV_DOWN_KEY, 0);
            input_sync(fpsensor_dev->input);
        } else if (FPSENSOR_KEY_RIGHT == fpsensor_key.key) {
            input_report_key(fpsensor_dev->input, FPSENSOR_NAV_RIGHT_KEY, 1);
            input_sync(fpsensor_dev->input);
            input_report_key(fpsensor_dev->input, FPSENSOR_NAV_RIGHT_KEY, 0);
            input_sync(fpsensor_dev->input);
        } else if (FPSENSOR_KEY_LEFT == fpsensor_key.key) {
            input_report_key(fpsensor_dev->input, FPSENSOR_NAV_LEFT_KEY, 1);
            input_sync(fpsensor_dev->input);
            input_report_key(fpsensor_dev->input, FPSENSOR_NAV_LEFT_KEY, 0);
            input_sync(fpsensor_dev->input);
        } else  if (FPSENSOR_KEY_TAP == fpsensor_key.key) {
            input_report_key(fpsensor_dev->input, FPSENSOR_NAV_TAP_KEY, 1);
            input_sync(fpsensor_dev->input);
            input_report_key(fpsensor_dev->input, FPSENSOR_NAV_TAP_KEY, 0);
            input_sync(fpsensor_dev->input);
        } else if ((FPSENSOR_KEY_POWER != fpsensor_key.key) && (FPSENSOR_KEY_CAPTURE != fpsensor_key.key)) {
            input_report_key(fpsensor_dev->input, key_event, fpsensor_key.value);
            input_sync(fpsensor_dev->input);
        }
#endif
        break;

    case FPSENSOR_IOC_ENTER_SLEEP_MODE:
        fpsensor_dev->is_sleep_mode = 1;
        break;
    case FPSENSOR_IOC_REMOVE:
	    if(fpsensor_dev->pinctrl1 != NULL){
		    devm_pinctrl_put(fpsensor_dev->pinctrl1);
		}
		
		
        if (fpsensor_dev->irq) {
            free_irq(fpsensor_dev->irq, fpsensor_dev);
            fpsensor_dev->irq_count = 0;
        }

		
#if FPSENSOR_INPUT
        if (fpsensor_dev->input != NULL) {
            input_unregister_device(fpsensor_dev->input);
        }
#endif
        if (fpsensor_dev->device != NULL) {
            device_destroy(fpsensor_dev->class, fpsensor_dev->devno);
        }
        if (fpsensor_dev->class != NULL ) {
            unregister_chrdev_region(fpsensor_dev->devno, 1);
            class_destroy(fpsensor_dev->class);
        }
        if (fpsensor_dev->users == 0) {
#if FPSENSOR_INPUT
            if (fpsensor_dev->input != NULL) {
                input_unregister_device(fpsensor_dev->input);
            }
#endif
            // if (fpsensor_dev->buffer != NULL)
            // kfree(fpsensor_dev->buffer);

            kfree(fpsensor_dev);
        }
        //mutex_unlock(&device_list_lock);
/*
        if (s_DEVINFO_fpsensor != NULL) {
            s_DEVINFO_fpsensor->device_type = "FP";
            s_DEVINFO_fpsensor->device_module = "oflim";
            s_DEVINFO_fpsensor->device_vendor = "NULL";
            s_DEVINFO_fpsensor->device_ic = "LC1550";
            s_DEVINFO_fpsensor->device_version = "NULL";
            s_DEVINFO_fpsensor->device_info = "fingerprint";
            s_DEVINFO_fpsensor->device_used = DEVINFO_UNUSED;
            devinfo_check_add_device(s_DEVINFO_fpsensor);
        }
*/
        fpsensor_hw_power_enable(0);
        fpsensor_spi_clk_enable(0);
#if FP_NOTIFY
        fb_unregister_client(&fpsensor_dev->notifier);
#endif
    //spi_unregister_driver(&fpsensor_spi_driver);
        fpsensor_debug(INFO_LOG, "yqy---%s remove finished\n", __func__);
		
        break;

    case FPSENSOR_IOC_CANCEL_WAIT:
        fpsensor_debug(INFO_LOG, "%s: FPSENSOR CANCEL WAIT\n", __func__);
        fpsensor_dev->cancel = 1;
        wake_up_interruptible(&fpsensor_dev->wq_irq_return);
        break;
#if FP_NOTIFY
    case FPSENSOR_IOC_GET_FP_STATUS :
        val = fpsensor_dev->fb_status;
        fpsensor_debug(INFO_LOG, "%s: FPSENSOR_IOC_GET_FP_STATUS  %d \n",__func__, fpsensor_dev->fb_status);
        if (copy_to_user((void __user *)arg, (void *)&val, sizeof(unsigned int))) {
            fpsensor_debug(ERR_LOG, "Failed to copy data to user\n");
            retval = -EFAULT;
            break;
        }
        retval = 0;
        break;
#endif
    default:
        fpsensor_debug(ERR_LOG, "fpsensor doesn't support this command(%d)\n", cmd);
        break;
    }

    FUNC_EXIT();
    return retval;
}
static long fpsensor_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    return fpsensor_ioctl(filp, cmd, (unsigned long)(arg));
}

static unsigned int fpsensor_poll(struct file *filp, struct poll_table_struct *wait)
{
    unsigned int ret = 0;
    fpsensor_debug(ERR_LOG, " support poll opertion  in   version\n");
    ret |= POLLIN;
    poll_wait(filp, &g_fpsensor->wq_irq_return, wait);
    if (g_fpsensor->cancel == 1 ) {
        fpsensor_debug(ERR_LOG, " cancle\n");
        ret =  POLLERR;
        g_fpsensor->cancel = 0;
        return ret;
    }
    if ( g_fpsensor->RcvIRQ) {
        fpsensor_debug(ERR_LOG, " get irq\n");
        ret |= POLLRDNORM;
    } else {
        ret = 0;
    }
    return ret;
}


/* -------------------------------------------------------------------- */
/* device function                                                      */
/* -------------------------------------------------------------------- */
static int fpsensor_open(struct inode *inode, struct file *filp)
{
    fpsensor_data_t *fpsensor_dev;

    FUNC_ENTRY();
    fpsensor_dev = container_of(inode->i_cdev, fpsensor_data_t, cdev);
    fpsensor_dev->users++;
    fpsensor_dev->device_available = 1;
    filp->private_data = fpsensor_dev;
    FUNC_EXIT();
    return 0;
}

static int fpsensor_release(struct inode *inode, struct file *filp)
{
    fpsensor_data_t *fpsensor_dev;
    int    status = 0;

    FUNC_ENTRY();
/*** begin: added by liuhe.zhong for close spi0 clk when MS has no fingerprint on 2018.02.27 ***/
    fpsensor_hw_power_enable(0);
    fpsensor_spi_clk_enable(0);
/*** end:   added by liuhe.zhong for close spi0 clk when MS has no fingerprint on 2018.02.27 ***/
    fpsensor_dev = filp->private_data;
    filp->private_data = NULL;

    /*last close??*/
    fpsensor_dev->users--;
    if (!fpsensor_dev->users) {
        //fpsensor_debug(INFO_LOG, "%s, disble_irq. irq = %d\n", __func__, fpsensor_dev->spi->irq);
        fpsensor_disable_irq(fpsensor_dev);
    }
    fpsensor_dev->device_available = 0;
/*** begin: added by liuhe.zhong for fingerprint hardware information on 2018.01.30 ***/
    sprintf(fp_info, "FPC1020:TOPTOUCH:NA");
/*** end:   added by liuhe.zhong for fingerprint hardware information on 2018.01.30 ***/
    FUNC_EXIT();
    return status;
}

static const struct file_operations fpsensor_fops = {
    .owner =    THIS_MODULE,

    .write =    fpsensor_write,
    .read =        fpsensor_read,
    .unlocked_ioctl = fpsensor_ioctl,
    .compat_ioctl   = fpsensor_compat_ioctl,
    .open =        fpsensor_open,
    .release =    fpsensor_release,
    .poll    = fpsensor_poll,

};

static int fpsensor_create_class(fpsensor_data_t *fpsensor)
{
    int error = 0;

    fpsensor->class = class_create(THIS_MODULE, FPSENSOR_CLASS_NAME);
    if (IS_ERR(fpsensor->class)) {
        fpsensor_debug(ERR_LOG, "%s, Failed to create class.\n", __func__);
        error = PTR_ERR(fpsensor->class);
    }

    return error;
}

static int fpsensor_create_device(fpsensor_data_t *fpsensor)
{
    int error = 0;


    if (FPSENSOR_MAJOR > 0) {
        //fpsensor->devno = MKDEV(FPSENSOR_MAJOR, fpsensor_device_count++);
        //error = register_chrdev_region(fpsensor->devno,
        //                 1,
        //                 FPSENSOR_DEV_NAME);
    } else {
        error = alloc_chrdev_region(&fpsensor->devno,
                                    fpsensor->device_count++,
                                    1,
                                    FPSENSOR_DEV_NAME);
    }

    if (error < 0) {
        fpsensor_debug(ERR_LOG,
                       "%s: FAILED %d.\n", __func__, error);
        goto out;

    } else {
        fpsensor_debug(INFO_LOG, "%s: major=%d, minor=%d\n",
                       __func__,
                       MAJOR(fpsensor->devno),
                       MINOR(fpsensor->devno));
    }

    fpsensor->device = device_create(fpsensor->class, &(fpsensor->pldev->dev), fpsensor->devno,
                                     fpsensor, FPSENSOR_DEV_NAME);

    if (IS_ERR(fpsensor->device)) {
        fpsensor_debug(ERR_LOG, "device_create failed.\n");
        error = PTR_ERR(fpsensor->device);
    }
out:
    return error;
}
#if FP_NOTIFY
static int fpsensor_fb_notifier_callback(struct notifier_block* self,
        unsigned long event, void* data)
{
    static char screen_status[64] = {'\0'};
    struct fb_event* evdata = data;
    unsigned int blank;
    int retval = 0;
    //fpsensor_debug(INFO_LOG,"%s enter.  event : %d\n", __func__,event);
    fpsensor_data_t *fpsensor_dev = g_fpsensor;
    if (event != FB_EVENT_BLANK /* FB_EARLY_EVENT_BLANK */) {
        return 0;
    }

    blank = *(int*)evdata->data;
    fpsensor_debug(INFO_LOG,"%s enter, blank=0x%x\n", __func__, blank);

    switch (blank) {
    case FB_BLANK_UNBLANK:
        fpsensor_debug(INFO_LOG,"%s: lcd on notify\n", __func__);
        sprintf(screen_status, "SCREEN_STATUS=%s", "ON");
        fpsensor_dev->fb_status = 1;
/*** begin: added by liuhe.zhong for close spi0 clk when MS has no fingerprint on 2018.02.27 ***/
	if((board_id != 100) && (board_id != 101)) {
		fpsensor_hw_power_enable(0);
		fpsensor_spi_clk_enable(0);
		fpsensor_debug(INFO_LOG,"%s: fpsensor_spi_clk_enable(0)\n", __func__);
	}
/*** end:   added by liuhe.zhong for close spi0 clk when MS has no fingerprint on 2018.02.27 ***/
        break;
    case FB_BLANK_POWERDOWN:
        fpsensor_debug(INFO_LOG,"%s: lcd off notify\n", __func__);
        sprintf(screen_status, "SCREEN_STATUS=%s", "OFF");
        fpsensor_dev->fb_status = 0;
/*** begin: added by liuhe.zhong for close spi0 clk when MS has no fingerprint on 2018.02.27 ***/
	if((board_id != 100) && (board_id != 101)) {
		fpsensor_hw_power_enable(0);
		fpsensor_spi_clk_enable(0);
		fpsensor_debug(INFO_LOG,"%s: fpsensor_spi_clk_enable(0)\n", __func__);
/*** end:   added by liuhe.zhong for close spi0 clk when MS has no fingerprint on 2018.02.27 ***/
	}
        break;

    default:
        fpsensor_debug(INFO_LOG,"%s: other notifier, ignore\n", __func__);
        break;
    }

    fpsensor_debug(INFO_LOG,"%s %s leave.\n", screen_status, __func__);
    return retval;
}
#endif

//static int fpsensor_probe(struct spi_device *spi)
static int fpsensor_probe(void)
{
    //struct device *dev = &spi->dev;
    fpsensor_data_t *fpsensor_dev = NULL;
    struct device_node *dev_node = NULL;
    struct device_node *dev_node_clk = NULL;
    struct platform_device *pdev = NULL;
    struct platform_device *pdev_clk = NULL;
    int error = 0;
    // u16 i = 0;
    // unsigned long minor;f
    int status = -EINVAL;

    FUNC_ENTRY();
    /* Allocate driver data */
    fpsensor_dev = kzalloc(sizeof(*fpsensor_dev), GFP_KERNEL);
    if (!fpsensor_dev) {
        fpsensor_debug(ERR_LOG, "%s, Failed to alloc memory for fpsensor device.\n", __func__);
        FUNC_EXIT();
        return -ENOMEM;
    }
    //fpsensor_dev->device = dev ;

    g_fpsensor = fpsensor_dev;
    /* Initialize the driver data */
    mutex_init(&fpsensor_dev->buf_lock);

    //spi_set_drvdata(spi, fpsensor_dev);
    //fpsensor_dev->spi = spi;
    // INIT_LIST_HEAD(&fpsensor_dev->device_entry);
    fpsensor_dev->device_available = 0;
    //fpsensor_dev->spi->irq = 0;
    fpsensor_dev->probe_finish = 0;
    fpsensor_dev->device_count     = 0;
    fpsensor_dev->users = 0;
    /*setup fpsensor configurations.*/
    fpsensor_debug(INFO_LOG, "%s, Setting fpsensor device configuration.\n", __func__);
    // fpsensor_irq_gpio_cfg();
    // fpsensor_reset_gpio_cfg();
    // dts read
    dev_node = of_find_compatible_node(NULL, NULL, "mediatek,fingerprint");
    //fpsensor_dev->pinctrl1 = devm_pinctrl_get(&spi->dev);
    /* Convert to platform device. */
    pdev = of_find_device_by_node(dev_node);
    if (!pdev) {
        fpsensor_error("of_find_device_by_node(..) failed.\n");
        return (-ENODEV);
    }
    fpsensor_dev->pldev = pdev;
    fpsensor_dev->device = &pdev->dev ;
    fpsensor_dev->pinctrl1 = devm_pinctrl_get(&pdev->dev);
    if (IS_ERR(fpsensor_dev->pinctrl1)) {
        error = PTR_ERR(fpsensor_dev->pinctrl1);
        fpsensor_error("fpsensor Cannot find fp pinctrl1.\n");
        goto err1;
    }
    fpsensor_spidev_dts_init(fpsensor_dev);
    error = fpsensor_create_class(fpsensor_dev);
    if (error) {
        goto err2;
    }
    error = fpsensor_create_device(fpsensor_dev);
    if (error) {
        goto err2;
    }
    cdev_init(&fpsensor_dev->cdev, &fpsensor_fops);
    fpsensor_dev->cdev.owner = THIS_MODULE;
    error = cdev_add(&fpsensor_dev->cdev, fpsensor_dev->devno, 1);
    if (error) {
        goto err2;
    }

    dev_node_clk = of_find_compatible_node(NULL, NULL, "mediatek,mt6739-spi");
    if (!dev_node) {
        fpsensor_error("of_find_compatible_node(.., '%s') failed.\n", "mediatek,mt6739-spi");
        return (-ENODEV);
    }

    /* 3-2: Convert to platform device. */
    pdev_clk = of_find_device_by_node(dev_node_clk);
    if (!pdev) {
        fpsensor_error("of_find_device_by_node(..) failed.\n");
        return (-ENODEV);
    }
    fpsensor_error("spi controller name: %s.\n", pdev_clk->name);

    /* 3-3: Retrieve the SPI clk handler. */
    fpsensor_dev->spiclk = devm_clk_get(&pdev_clk->dev, "spi-clk");
    if (!fpsensor_dev->spiclk) {
        fpsensor_error("devm_clk_get(..) failed.\n");
        return (-ENODEV);
    }

    //register input device
#if FPSENSOR_INPUT
    fpsensor_dev->input = input_allocate_device();
    if (fpsensor_dev->input == NULL) {
        fpsensor_debug(ERR_LOG, "%s, Failed to allocate input device.\n", __func__);
        error = -ENOMEM;
        goto err2;
    }
    __set_bit(EV_KEY, fpsensor_dev->input->evbit);
    __set_bit(FPSENSOR_INPUT_HOME_KEY, fpsensor_dev->input->keybit);

    __set_bit(FPSENSOR_INPUT_MENU_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_INPUT_BACK_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_INPUT_FF_KEY, fpsensor_dev->input->keybit);

    __set_bit(FPSENSOR_NAV_TAP_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_NAV_UP_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_NAV_DOWN_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_NAV_RIGHT_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_NAV_LEFT_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_INPUT_CAMERA_KEY, fpsensor_dev->input->keybit);
    fpsensor_dev->input->name = FPSENSOR_INPUT_NAME;
    if (input_register_device(fpsensor_dev->input)) {
        fpsensor_debug(ERR_LOG, "%s, Failed to register input device.\n", __func__);
        error = -ENODEV;
        goto err1;
    }
#endif
    fpsensor_dev->device_available = 1;
    fpsensor_dev->irq_count = 1;
    // mt_eint_unmask(fpsensor_dev->spi->irq);

    fpsensor_dev->sig_count = 0;

    fpsensor_debug(INFO_LOG, "%s: fpsensor init finished======\n", __func__);


    fpsensor_dev->probe_finish = 1;
    fpsensor_dev->is_sleep_mode = 0;
    fpsensor_hw_power_enable(1);
    fpsensor_spi_clk_enable(1);

    //init wait queue
    init_waitqueue_head(&fpsensor_dev->wq_irq_return);
#if FP_NOTIFY
    fpsensor_dev->notifier.notifier_call = fpsensor_fb_notifier_callback;
    fb_register_client(&fpsensor_dev->notifier);
#endif
    fpsensor_debug(INFO_LOG, "%s probe finished, normal driver version: %s\n", __func__,
                   FPSENSOR_SPI_VERSION);
/*** begin: added by liuhe.zhong for fingerprint hardware information on 2018.01.30 ***/
    sprintf(fp_info, "ICN7151:HOLITECH:NA");
/*** end:   added by liuhe.zhong for fingerprint hardware information on 2018.01.30 ***/
    FUNC_EXIT();
    return 0;
err1:
#if FPSENSOR_INPUT
    input_free_device(fpsensor_dev->input);
#endif

err2:
    device_destroy(fpsensor_dev->class, fpsensor_dev->devno);
    fpsensor_hw_power_enable(0);
    fpsensor_spi_clk_enable(0);
    kfree(fpsensor_dev);
    FUNC_EXIT();
    return status;
}

#if 0
static int fpsensor_probe(struct spi_device *spi)
{
    struct device *dev = &spi->dev;
    fpsensor_data_t *fpsensor_dev = NULL;
    int error = 0;
    // u16 i = 0;
    // unsigned long minor;f
    int status = -EINVAL;

    FUNC_ENTRY();
    /* Allocate driver data */
    fpsensor_dev = kzalloc(sizeof(*fpsensor_dev), GFP_KERNEL);
    if (!fpsensor_dev) {
        fpsensor_debug(ERR_LOG, "%s, Failed to alloc memory for fpsensor device.\n", __func__);
        FUNC_EXIT();
        return -ENOMEM;
    }
    fpsensor_dev->device = dev ;

    g_fpsensor = fpsensor_dev;
    /* Initialize the driver data */
    mutex_init(&fpsensor_dev->buf_lock);

    spi_set_drvdata(spi, fpsensor_dev);
    fpsensor_dev->spi = spi;
    // INIT_LIST_HEAD(&fpsensor_dev->device_entry);
    fpsensor_dev->device_available = 0;
    fpsensor_dev->spi->irq = 0;
    fpsensor_dev->probe_finish = 0;
    fpsensor_dev->device_count     = 0;
    fpsensor_dev->users = 0;
    /*setup fpsensor configurations.*/
    fpsensor_debug(INFO_LOG, "%s, Setting fpsensor device configuration.\n", __func__);
    // fpsensor_irq_gpio_cfg();
    // fpsensor_reset_gpio_cfg();
    // dts read
    spi->dev.of_node = of_find_compatible_node(NULL, NULL, "mediatek,fingerprint");
    fpsensor_dev->pinctrl1 = devm_pinctrl_get(&spi->dev);
    if (IS_ERR(fpsensor_dev->pinctrl1)) {
        error = PTR_ERR(fpsensor_dev->pinctrl1);
        fpsensor_error("fpsensor Cannot find fp pinctrl1.\n");
        goto err1;
    }
    fpsensor_spidev_dts_init(fpsensor_dev);
    error = fpsensor_create_class(fpsensor_dev);
    if (error) {
        goto err2;
    }
    error = fpsensor_create_device(fpsensor_dev);
    if (error) {
        goto err2;
    }
    cdev_init(&fpsensor_dev->cdev, &fpsensor_fops);
    fpsensor_dev->cdev.owner = THIS_MODULE;
    error = cdev_add(&fpsensor_dev->cdev, fpsensor_dev->devno, 1);
    if (error) {
        goto err2;
    }
    //register input device
#if FPSENSOR_INPUT
    fpsensor_dev->input = input_allocate_device();
    if (fpsensor_dev->input == NULL) {
        fpsensor_debug(ERR_LOG, "%s, Failed to allocate input device.\n", __func__);
        error = -ENOMEM;
        goto err2;
    }
    __set_bit(EV_KEY, fpsensor_dev->input->evbit);
    __set_bit(FPSENSOR_INPUT_HOME_KEY, fpsensor_dev->input->keybit);

    __set_bit(FPSENSOR_INPUT_MENU_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_INPUT_BACK_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_INPUT_FF_KEY, fpsensor_dev->input->keybit);

    __set_bit(FPSENSOR_NAV_TAP_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_NAV_UP_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_NAV_DOWN_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_NAV_RIGHT_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_NAV_LEFT_KEY, fpsensor_dev->input->keybit);
    __set_bit(FPSENSOR_INPUT_CAMERA_KEY, fpsensor_dev->input->keybit);
    fpsensor_dev->input->name = FPSENSOR_INPUT_NAME;
    if (input_register_device(fpsensor_dev->input)) {
        fpsensor_debug(ERR_LOG, "%s, Failed to register input device.\n", __func__);
        error = -ENODEV;
        goto err1;
    }
#endif
    fpsensor_dev->device_available = 1;
    fpsensor_dev->irq_count = 1;
    // mt_eint_unmask(fpsensor_dev->spi->irq);

    fpsensor_dev->sig_count = 0;

    fpsensor_debug(INFO_LOG, "%s: fpsensor init finished======\n", __func__);


    fpsensor_dev->probe_finish = 1;
    fpsensor_dev->is_sleep_mode = 0;
    fpsensor_hw_power_enable(1);
    fpsensor_spi_clk_enable(1);

    //init wait queue
    init_waitqueue_head(&fpsensor_dev->wq_irq_return);
#if FP_NOTIFY
    fpsensor_dev->notifier.notifier_call = fpsensor_fb_notifier_callback;
    fb_register_client(&fpsensor_dev->notifier);
#endif
    fpsensor_debug(INFO_LOG, "%s probe finished, normal driver version: %s\n", __func__,
                   FPSENSOR_SPI_VERSION);
    FUNC_EXIT();
    return 0;
err1:
#if FPSENSOR_INPUT
    input_free_device(fpsensor_dev->input);
#endif

err2:
    device_destroy(fpsensor_dev->class, fpsensor_dev->devno);
    fpsensor_hw_power_enable(0);
    fpsensor_spi_clk_enable(0);
    kfree(fpsensor_dev);
    FUNC_EXIT();
    return status;
}

static int fpsensor_remove(struct spi_device *spi)
{
    fpsensor_data_t *fpsensor_dev = spi_get_drvdata(spi);
    FUNC_ENTRY();

    /* make sure ops on existing fds can abort cleanly */
    if (fpsensor_dev->spi->irq) {
        free_irq(fpsensor_dev->spi->irq, fpsensor_dev);
    }

	devm_pinctrl_put(fpsensor_dev->pinctrl1);
	
    fpsensor_dev->spi = NULL;
    spi_set_drvdata(spi, NULL);
    device_destroy(fpsensor_dev->class, fpsensor_dev->devno);
    unregister_chrdev_region(fpsensor_dev->devno, 1);
    class_destroy(fpsensor_dev->class);
    if (fpsensor_dev->users == 0) {
#if FPSENSOR_INPUT
        if (fpsensor_dev->input != NULL) {
            input_unregister_device(fpsensor_dev->input);
        }
#endif
    }
    fpsensor_hw_power_enable(0);
#if FP_NOTIFY
    fb_unregister_client(&fpsensor_dev->notifier);
#endif
    fpsensor_debug(INFO_LOG, "%s remove finished\n", __func__);
    kfree(fpsensor_dev);
#if 0
    if (s_DEVINFO_fpsensor != NULL) {
        kfree(s_DEVINFO_fpsensor);
    }
#endif
    //spi_unregister_driver(&fpsensor_spi_driver);
    //spi_unregister_device(spi);
	printk("yqy-------remove spi device and driver!!\n");
    FUNC_EXIT();
    return 0;
}

#ifdef CONFIG_OF
static struct of_device_id fpsensor_of_match[] = {
    { .compatible = "fpsensor,fingerprint", },
    {}
};
MODULE_DEVICE_TABLE(of, fpsensor_of_match);
#endif
struct spi_device_id fpsensor_spi_id_table = {FPSENSOR_DEV_NAME, 0};

static struct spi_driver fpsensor_spi_driver = {
    .driver = {
        .name = FPSENSOR_DEV_NAME,
        .bus = &spi_bus_type,
        .owner = THIS_MODULE,
#ifdef CONFIG_OF
        .of_match_table = fpsensor_of_match,
#endif
    },
    .id_table = &fpsensor_spi_id_table,
    .probe = fpsensor_probe,
    .remove = fpsensor_remove,
};
#endif

static int __init fpsensor_init(void)
{
    int status;
    FUNC_ENTRY();
    //status = spi_register_driver(&fpsensor_spi_driver);
	status = fpsensor_probe();
    if (status < 0) {
        fpsensor_debug(ERR_LOG, "%s, Failed to register SPI driver.\n", __func__);
    }

    FUNC_EXIT();
    return status;
}
module_init(fpsensor_init);

static void __exit fpsensor_exit(void)
{
    FUNC_ENTRY();
    //spi_unregister_driver(&fpsensor_spi_driver);
    FUNC_EXIT();
}
module_exit(fpsensor_exit);

MODULE_AUTHOR("xhli");
MODULE_DESCRIPTION(" Fingerprint chip TEE driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:fpsensor_spi");
