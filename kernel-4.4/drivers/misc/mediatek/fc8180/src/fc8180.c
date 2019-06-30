/*****************************************************************************
	Copyright(c) 2013 FCI Inc. All Rights Reserved

	File name : fc8180.c

	Description : Driver source file

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

	History :
	----------------------------------------------------------------------
*******************************************************************************/
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/poll.h>
#include <linux/vmalloc.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/of_irq.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#include "fc8180.h"
#include "bbm.h"
#include "fci_oal.h"
#include "fci_tun.h"
#include "fc8180_regs.h"
#include "fc8180_isr.h"
#include "fci_hal.h"

#ifdef FEATURE_MTK
#include <mach/mt_gpio.h>
#include <mach/eint.h>
#include <mach/mt_pm_ldo.h>
#include <mach/mt_boot.h>
#include <cust_gpio_usage.h>
#include <cust_eint.h>
#include <cust_eint_md1.h>
#endif

#include <mtk_clkbuf_ctl.h>
struct ISDBT_INIT_INFO_T *hInit;

#define RING_BUFFER_SIZE	(188 * 32 * 50)

/* GPIO(RESET & INTRRUPT) Setting */
#define FC8180_NAME		"isdbt"

#define GPIO_ISDBT_IRQ         13
#define GPIO_ISDBT_PWR_EN      (2) //(GPIO2)

// ---------------------------------------------------- start
static struct device_node *node = NULL;
static u32 ints[2] = { 0, 0 };
static u32 ints1[2] = { 0, 0 };
static unsigned int dtv_eint_type;
static unsigned int irq_num;
static struct of_device_id dtv_of_match[] = {
	{ .compatible = "mediatek,dtv-platform", },
	{},
};

static struct pinctrl *dtv_pinctrl;

static struct pinctrl_state *pins_default;

static struct pinctrl_state *sck_pins;
static struct pinctrl_state *mosi_pins;
static struct pinctrl_state *miso_pins;
static struct pinctrl_state *cs_pins;
static struct pinctrl_state *eint_pins;

static struct pinctrl_state *en_pins_set;
static struct pinctrl_state *en_pins_clr;
static struct pinctrl_state *sw_pins_set;
static struct pinctrl_state *sw_pins_clr;

int isdbt_hw_setting(void);

static int dtvplat_drv_probe(struct platform_device *dev)
{
    int err;

    print_log(0, "fc8180 dtvplat_drv_probe ----> start\n");

    dtv_pinctrl = devm_pinctrl_get(&dev->dev);
	if (IS_ERR(dtv_pinctrl)) {
		err = PTR_ERR(dtv_pinctrl);
		print_log(0, "fc8180 devm_pinctrl_get error\n");
	}
    
    /* default pin config */
	pins_default = pinctrl_lookup_state(dtv_pinctrl, "default");
	if (IS_ERR(pins_default)) {
		err = PTR_ERR(pins_default);
		print_log(0, "fc8180 pinctrl_lookup_state pins_default error\n");
	}
	
    /* spi pins config - SCK */
    sck_pins = pinctrl_lookup_state(dtv_pinctrl, "sck_cfg");
	if (IS_ERR(sck_pins)) {
		err = PTR_ERR(sck_pins);
		print_log(0, "fc8180 pinctrl_lookup_state sck_pins error\n");
	} else {
    	pinctrl_select_state(dtv_pinctrl, sck_pins);
    }

    /* spi pins config - MOSI */
	mosi_pins = pinctrl_lookup_state(dtv_pinctrl, "mosi_cfg");
	if (IS_ERR(mosi_pins)) {
		err = PTR_ERR(mosi_pins);
		print_log(0, "fc8180 pinctrl_lookup_state mosi_pins error\n");
	} else {
    	pinctrl_select_state(dtv_pinctrl, mosi_pins);
    }

    /* spi pins config - MISO */
	miso_pins = pinctrl_lookup_state(dtv_pinctrl, "miso_cfg");
	if (IS_ERR(miso_pins)) {
		err = PTR_ERR(miso_pins);
		print_log(0, "fc8180 pinctrl_lookup_state miso_pins error\n");
	} else {
    	pinctrl_select_state(dtv_pinctrl, miso_pins);
    }

    /* spi pins config - CS */
    cs_pins = pinctrl_lookup_state(dtv_pinctrl, "cs_cfg");
	if (IS_ERR(cs_pins)) {
		err = PTR_ERR(cs_pins);
		print_log(0, "fc8180 pinctrl_lookup_state cs_pins error\n");
	} else {
    	pinctrl_select_state(dtv_pinctrl, cs_pins);    
    }

    /* interrupt pin config */
	eint_pins = pinctrl_lookup_state(dtv_pinctrl, "eint_cfg");
	if (IS_ERR(eint_pins)) {
        err = PTR_ERR(eint_pins);
		print_log(0, "fc8180 pinctrl_lookup_state eint_pins error\n");
	} else {
    	pinctrl_select_state(dtv_pinctrl, eint_pins);
    }

    /* chip enable pin high state config */
    en_pins_set = pinctrl_lookup_state(dtv_pinctrl, "en_cfg_set");
	if (IS_ERR(en_pins_set)) {
		err = PTR_ERR(en_pins_set);
		print_log(0, "fc8180 pinctrl_lookup_state en_pins_set error\n");
	}

    /* chip enable pin low state config */
    en_pins_clr = pinctrl_lookup_state(dtv_pinctrl, "en_cfg_clr");
	if (IS_ERR(en_pins_clr)) {
		err = PTR_ERR(en_pins_clr);
		print_log(0, "fc8180 pinctrl_lookup_state en_pins_clr error\n");
	}

    /* FM or DTV select switch pin high state */
	sw_pins_set = pinctrl_lookup_state(dtv_pinctrl, "sw_cfg_set");
	if (IS_ERR(sw_pins_set)) {
        err = PTR_ERR(sw_pins_set);
		print_log(0, "fc8180 pinctrl_lookup_state sw_pins_set error\n");
	}

    /* FM or DTV select switch pin low state */
	sw_pins_clr = pinctrl_lookup_state(dtv_pinctrl, "sw_cfg_clr");
	if (IS_ERR(sw_pins_clr)) {
        err = PTR_ERR(sw_pins_clr);
		print_log(0, "fc8180 pinctrl_lookup_state sw_pins_clr error\n");
	}

    /* sw gpio pin request and set it to high */
#if 0
    err = gpio_request(GPIO_FC8XXX_SW_PIN, "sw_pinctrl");
    if (err) {
        print_log(0, "fc8180 +%s request gpio error !\n", __func__);
    } else {
        gpio_direction_output(GPIO_FC8XXX_SW_PIN, 1);
    }
#else
    pinctrl_select_state(dtv_pinctrl, sw_pins_clr);	// set to FM ANT default.
#endif

#if 0
    /* EN gpio pin request and set it to low(chip disable) */
    err = gpio_request(GPIO_FC8XXX_EN_PIN, "en_pinctrl");
    if (err) {
        print_log(0, "fc8180 +%s request gpio error !\n", __func__);
    } else {
        gpio_direction_output(GPIO_FC8XXX_EN_PIN, 0);
    }
#else
    pinctrl_select_state(dtv_pinctrl, en_pins_clr);
#endif

	isdbt_hw_setting();

    /* Close DTV clock */
    clk_buf_ctrl(CLK_BUF_NFC, 0);

    print_log(0, "fc8180 dtvplat_drv_probe ----> end\n");

    return 0;
}

static int dtvplat_drv_remove(struct platform_device *dev)
{
    print_log(0, "fc8180 +%s\n", __func__);
    return 0;
}

static struct platform_driver dtv_platform_driver =
{
    .probe      = dtvplat_drv_probe,
    .remove     = dtvplat_drv_remove,
    .driver     = {
        .name   = "dtv",
        .of_match_table = dtv_of_match,
    },
};
// -------------------------------------------------------- end

u8 static_ringbuffer[RING_BUFFER_SIZE];

enum ISDBT_MODE driver_mode = ISDBT_POWEROFF;
static DEFINE_MUTEX(ringbuffer_lock);

static DECLARE_WAIT_QUEUE_HEAD(isdbt_isr_wait);

#ifndef BBM_I2C_TSIF
static u8 isdbt_isr_sig;
static struct task_struct *isdbt_kthread;

#ifdef FEATURE_MTK
void isdbt_irq(void)
{
	isdbt_isr_sig = 1;
	wake_up_interruptible(&isdbt_isr_wait);
    /*mt_eint_unmask(CUST_EINT_ISDBT_NUM);*/
	return;
}
#else
static irqreturn_t isdbt_irq(int irq, void *dev_id)
{
	isdbt_isr_sig = 1;
	wake_up_interruptible(&isdbt_isr_wait);
	return IRQ_HANDLED;
}
#endif
#endif
#ifdef FEATURE_MTK
int isdbt_hw_setting(void)
{
	print_log(0, "[FC8300] isdbt_hw_setting mtk\n");
	mt_set_gpio_mode(GPIO_ISDBT_PWR_EN,  GPIO_ISDBT_EN_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_ISDBT_PWR_EN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_ISDBT_PWR_EN, GPIO_OUT_ZERO);

	mt_eint_registration(CUST_EINT_ISDBT_NUM
		, CUST_EINT_ISDBT_TYPE, isdbt_irq, 1);
	mt_eint_mask(CUST_EINT_ISDBT_NUM);
	return 0;
}
#else
int isdbt_hw_setting(void)
{
	int err = 0;
	print_log(0, "isdbt_hw_setting\n");

#if 0
	err = gpio_request(GPIO_ISDBT_PWR_EN, "isdbt_en");
	if (err) {
		print_log(0, "isdbt_hw_setting: Couldn't request isdbt_en\n");
		goto gpio_isdbt_en;
	}
	gpio_direction_output(GPIO_ISDBT_PWR_EN, 1);
#else
	pinctrl_select_state(dtv_pinctrl, en_pins_clr);
#endif

#ifndef BBM_I2C_TSIF
#if 0
	err = gpio_request(GPIO_ISDBT_IRQ, "isdbt_irq");
	if (err) {
		print_log(0, "isdbt_hw_setting: Couldn't request isdbt_irq\n");
		goto gpio_isdbt_rst;
	}

	gpio_direction_input(GPIO_ISDBT_IRQ);
#endif
// ---------------------------------------------------- start
    node = of_find_matching_node(node, dtv_of_match);
    if (node) {
        of_property_read_u32_array(node, "debounce", ints, ARRAY_SIZE(ints));
        of_property_read_u32_array(node, "interrupts", ints1, ARRAY_SIZE(ints1));
        gpio_set_debounce(ints[0], ints[1]);
        dtv_eint_type = ints1[1];
        irq_num = irq_of_parse_and_map(node, 0);
        if (!irq_num) {
            print_log(0, "fc8180: couldn't request gpio interrupt\n");
            goto request_isdbt_irq;            
        }
		print_log(0, "fc8180: interrupt num=%s\n", irq_num);
        err = request_irq(irq_num, isdbt_irq, IRQF_TRIGGER_NONE, FC8180_NAME, NULL);
        if (err) {
            print_log(0, "fc8180: couldn't request gpio interrupt %d reason(%d)\n"
                    , irq_num, err);
            goto request_isdbt_irq;
        }        
    } else {
        print_log(0, "fc8180: can't find compatible node\n"
                   , GPIO_ISDBT_IRQ, err);
        goto request_isdbt_irq;
    }
// ------------------------------------------------------ end
#if 0
	err = request_irq(GPIO_ISDBT_IRQ, isdbt_irq
		, IRQF_DISABLED | IRQF_TRIGGER_FALLING, FC8180_NAME, NULL);
	if (err < 0) {
		print_log(0,
			"isdbt_hw_setting: couldn't request gpio interrupt %d reason(%d)\n"
			, GPIO_ISDBT_IRQ, err);
	goto request_isdbt_irq;
	}
#endif

#endif

	return 0;
#ifndef BBM_I2C_TSIF
request_isdbt_irq:
	gpio_free(GPIO_ISDBT_IRQ);
#endif
#if 0
gpio_isdbt_rst:
	gpio_free(GPIO_ISDBT_PWR_EN);
 gpio_isdbt_en:
#endif
	return err;
}
#endif

/*POWER_ON & HW_RESET & INTERRUPT_CLEAR */
void isdbt_hw_init(void)
{
	int i = 0;
	
	clk_buf_ctrl(CLK_BUF_NFC, 1);               // Open DTV clock
	while (driver_mode == ISDBT_DATAREAD) {
		ms_wait(100);
		if (i++ > 5)
			break;
	}

	print_log(0, "isdbt_hw_init\n");

#ifdef FEATURE_MTK
	mt_set_gpio_out(GPIO_ISDBT_PWR_EN, GPIO_OUT_ONE);
#else
#if 0
    gpio_set_value(GPIO18, 1);                  // Set the switch to DTV ANT
	gpio_set_value(GPIO_ISDBT_PWR_EN, 1);
#else
	pinctrl_select_state(dtv_pinctrl, sw_pins_set);	// Set the switch to DTV ANT
	pinctrl_select_state(dtv_pinctrl, en_pins_clr);
	mdelay(100);
	pinctrl_select_state(dtv_pinctrl, en_pins_set);
#endif
#endif
	mdelay(30);
	mdelay(50); //add by shiqiang.chen.hz for defect 5876221 on 20180208, add 50ms delay time for the stabilization of spi.  

	driver_mode = ISDBT_POWERON;

}

/*POWER_OFF */
void isdbt_hw_deinit(void)
{
	print_log(0, "isdbt_hw_deinit\n");
	driver_mode = ISDBT_POWEROFF;
	clk_buf_ctrl(CLK_BUF_NFC, 0);               // Close DTV clock
#ifdef FEATURE_MTK
	mt_set_gpio_out(GPIO_ISDBT_PWR_EN, GPIO_OUT_ZERO);
#else
#if 0
    gpio_set_value(GPIO18, 0);                  // Set the switch to FM ANT
	gpio_set_value(GPIO_ISDBT_PWR_EN, 0);
#else
	pinctrl_select_state(dtv_pinctrl, sw_pins_clr); // Set the switch to FM ANT
	pinctrl_select_state(dtv_pinctrl, en_pins_clr);
#endif
#endif
	mdelay(5);
}

int data_callback(ulong hDevice, u8 *data, int len)
{
	struct ISDBT_INIT_INFO_T *hInit;
	struct list_head *temp;
	hInit = (struct ISDBT_INIT_INFO_T *)hDevice;

	list_for_each(temp, &(hInit->hHead))
	{
		struct ISDBT_OPEN_INFO_T *hOpen;

		hOpen = list_entry(temp, struct ISDBT_OPEN_INFO_T, hList);

		if (hOpen->isdbttype == TS_TYPE) {
			mutex_lock(&ringbuffer_lock);
			if (fci_ringbuffer_free(&hOpen->RingBuffer) < len) {
				/* return 0 */;
				FCI_RINGBUFFER_SKIP(&hOpen->RingBuffer, len);
			}
			fci_ringbuffer_write(&hOpen->RingBuffer, data, len);

			wake_up_interruptible(&(hOpen->RingBuffer.queue));

			mutex_unlock(&ringbuffer_lock);
		}
	}

	return 0;
}


#ifndef BBM_I2C_TSIF
static int isdbt_thread(void *hDevice)
{
	struct ISDBT_INIT_INFO_T *hInit = (struct ISDBT_INIT_INFO_T *)hDevice;

	set_user_nice(current, -20);

	print_log(hInit, "isdbt_kthread enter\n");

	bbm_com_ts_callback_register((ulong)hInit, data_callback);

	while (1) {
		wait_event_interruptible(isdbt_isr_wait,
			isdbt_isr_sig || kthread_should_stop());

		if (driver_mode == ISDBT_POWERON) {
			driver_mode = ISDBT_DATAREAD;
			bbm_com_isr(hInit);
			driver_mode = ISDBT_POWERON;
		}

		isdbt_isr_sig = 0;

		if (kthread_should_stop())
			break;
	}

	bbm_com_ts_callback_deregister();

	print_log(hInit, "isdbt_kthread exit\n");

	return 0;
}
#endif

const struct file_operations isdbt_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl		= isdbt_ioctl,
	.open		= isdbt_open,
	.read		= isdbt_read,
	.release	= isdbt_release,
};

static struct miscdevice fc8180_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = FC8180_NAME,
	.fops = &isdbt_fops,
};

int isdbt_open(struct inode *inode, struct file *filp)
{
	struct ISDBT_OPEN_INFO_T *hOpen;

	print_log(hInit, "isdbt open\n");

	hOpen = kmalloc(sizeof(struct ISDBT_OPEN_INFO_T), GFP_KERNEL);

	hOpen->buf = &static_ringbuffer[0];
	/*kmalloc(RING_BUFFER_SIZE, GFP_KERNEL);*/
	hOpen->isdbttype = 0;

	list_add(&(hOpen->hList), &(hInit->hHead));

	hOpen->hInit = (HANDLE *)hInit;

	if (hOpen->buf == NULL) {
		print_log(hInit, "ring buffer malloc error\n");
		return -ENOMEM;
	}

	fci_ringbuffer_init(&hOpen->RingBuffer, hOpen->buf, RING_BUFFER_SIZE);

	filp->private_data = hOpen;

	return 0;
}

ssize_t isdbt_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	s32 avail;
	s32 non_blocking = filp->f_flags & O_NONBLOCK;
	struct ISDBT_OPEN_INFO_T *hOpen
		= (struct ISDBT_OPEN_INFO_T *)filp->private_data;
	struct fci_ringbuffer *cibuf = &hOpen->RingBuffer;
	ssize_t len, read_len = 0;

	if (!cibuf->data || !count)	{
		/*print_log(hInit, " return 0\n"); */
		return 0;
	}

	if (non_blocking && (fci_ringbuffer_empty(cibuf)))	{
		/*print_log(hInit, "return EWOULDBLOCK\n"); */
		return -EWOULDBLOCK;
	}

	if (wait_event_interruptible(cibuf->queue,
		!fci_ringbuffer_empty(cibuf))) {
		print_log(hInit, "return ERESTARTSYS\n");
		return -ERESTARTSYS;
	}

	mutex_lock(&ringbuffer_lock);

	avail = fci_ringbuffer_avail(cibuf);

	if (count >= avail)
		len = avail;
	else
		len = count - (count % 188);

	read_len = fci_ringbuffer_read_user(cibuf, buf, len);

	mutex_unlock(&ringbuffer_lock);

	return read_len;
}

int isdbt_release(struct inode *inode, struct file *filp)
{
	struct ISDBT_OPEN_INFO_T *hOpen;

	print_log(hInit, "isdbt_release\n");

	hOpen = filp->private_data;

	hOpen->isdbttype = 0;

	list_del(&(hOpen->hList));
	/*kfree(hOpen->buf);*/

	if (hOpen != NULL)
		kfree(hOpen);

	return 0;
}


#ifndef BBM_I2C_TSIF
void isdbt_isr_check(HANDLE hDevice)
{
	u8 isr_time = 0;

	bbm_com_write(hDevice, BBM_BUF_ENABLE, 0x00);

	while (isr_time < 10) {
		if (!isdbt_isr_sig)
			break;

		ms_wait(10);
		isr_time++;
	}

}
#endif

long isdbt_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	s32 res = BBM_NOK;
	s32 err = 0;
	s32 size = 0;
	struct ISDBT_OPEN_INFO_T *hOpen;

	struct ioctl_info info;

	if (_IOC_TYPE(cmd) != IOCTL_MAGIC)
		return -EINVAL;
	if (_IOC_NR(cmd) >= IOCTL_MAXNR)
		return -EINVAL;

	hOpen = filp->private_data;

	size = _IOC_SIZE(cmd);

	switch (cmd) {
	case IOCTL_ISDBT_RESET:
		res = bbm_com_reset(hInit);
		print_log(hInit, "[FC8180] IOCTL_ISDBT_RESET\n");
		break;
	case IOCTL_ISDBT_INIT:
		res = bbm_com_i2c_init(hInit, FCI_HPI_TYPE);
		res |= bbm_com_probe(hInit);
		if (res) {
			print_log(hInit, "FC8180 Initialize Fail\n");
			break;
		}
		res = bbm_com_init(hInit);
		res |= bbm_com_tuner_select(hInit, FC8180_TUNER, 0);
		break;
	case IOCTL_ISDBT_BYTE_READ:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_byte_read(hInit, (u16)info.buff[0]
			, (u8 *)(&info.buff[1]));
		err |= copy_to_user((void *)arg, (void *)&info, size);
		break;
	case IOCTL_ISDBT_WORD_READ:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_word_read(hInit, (u16)info.buff[0]
			, (u16 *)(&info.buff[1]));
		err |= copy_to_user((void *)arg, (void *)&info, size);
		break;
	case IOCTL_ISDBT_LONG_READ:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_long_read(hInit, (u16)info.buff[0]
			, (u32 *)(&info.buff[1]));
		err |= copy_to_user((void *)arg, (void *)&info, size);
		break;
	case IOCTL_ISDBT_BULK_READ:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_bulk_read(hInit, (u16)info.buff[0]
			, (u8 *)(&info.buff[2]), info.buff[1]);
		err |= copy_to_user((void *)arg, (void *)&info, size);
		break;
	case IOCTL_ISDBT_BYTE_WRITE:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_byte_write(hInit, (u16)info.buff[0]
			, (u8)info.buff[1]);
		break;
	case IOCTL_ISDBT_WORD_WRITE:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_word_write(hInit, (u16)info.buff[0]
			, (u16)info.buff[1]);
		break;
	case IOCTL_ISDBT_LONG_WRITE:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_long_write(hInit, (u16)info.buff[0]
			, (u32)info.buff[1]);
		break;
	case IOCTL_ISDBT_BULK_WRITE:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_bulk_write(hInit, (u16)info.buff[0]
			, (u8 *)(&info.buff[2]), info.buff[1]);
		break;
	case IOCTL_ISDBT_TUNER_READ:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_tuner_read(hInit, (u8)info.buff[0]
			, (u8)info.buff[1],  (u8 *)(&info.buff[3])
			, (u8)info.buff[2]);
		err |= copy_to_user((void *)arg, (void *)&info, size);
		break;
	case IOCTL_ISDBT_TUNER_WRITE:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_tuner_write(hInit, (u8)info.buff[0]
			, (u8)info.buff[1], (u8 *)(&info.buff[3])
			, (u8)info.buff[2]);
		break;
	case IOCTL_ISDBT_TUNER_SET_FREQ:
		{
			u32 f_rf;
			err = copy_from_user((void *)&info, (void *)arg, size);
			f_rf = (u32)info.buff[0];
#ifndef BBM_I2C_TSIF
#ifdef FEATURE_MTK
			mt_eint_mask(CUST_EINT_ISDBT_NUM);
#endif
			isdbt_isr_check(hInit);
#endif
			res = bbm_com_tuner_set_freq(hInit, f_rf);
#ifndef BBM_I2C_TSIF
			mutex_lock(&ringbuffer_lock);
			fci_ringbuffer_flush(&hOpen->RingBuffer);
			mutex_unlock(&ringbuffer_lock);
#ifdef FEATURE_MTK
			mt_eint_unmask(CUST_EINT_ISDBT_NUM);
#endif
			bbm_com_write(hInit, BBM_BUF_ENABLE, 0x01);
#endif
		}
		break;
	case IOCTL_ISDBT_TUNER_SELECT:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_tuner_select(hInit
			, (u32)info.buff[0], (u32)info.buff[1]);
		print_log(hInit, "[FC8180] IOCTL_ISDBT_TUNER_SELECT\n");
		break;
	case IOCTL_ISDBT_TS_START:
		hOpen->isdbttype = TS_TYPE;
		print_log(hInit, "[FC8180] IOCTL_ISDBT_TS_START\n");
		break;
	case IOCTL_ISDBT_TS_STOP:
		hOpen->isdbttype = 0;
		print_log(hInit, "[FC8180] IOCTL_ISDBT_TS_STOP\n");
		break;
	case IOCTL_ISDBT_POWER_ON:
		isdbt_hw_init();
//Begin added by shiqiang.chen.hz for defect 5876221 on 20180208, optimize init seq.
#ifdef BBM_SPI_IF
		bbm_com_byte_write(hInit, BBM_DM_DATA, 0x00);
#endif
//End added by shiqiang.chen.hz for defect 5876221 on 20180208, optimize init seq.
		print_log(hInit, "[FC8180] IOCTL_ISDBT_POWER_ON\n");
		break;
	case IOCTL_ISDBT_POWER_OFF:
#ifdef FEATURE_MTK
		mt_eint_mask(CUST_EINT_ISDBT_NUM);
#endif
		isdbt_hw_deinit();
		print_log(hInit, "[FC8180] IOCTL_ISDBT_POWER_OFF\n");
		break;
	case IOCTL_ISDBT_SCAN_STATUS:
		res = bbm_com_scan_status(hInit);
		print_log(hInit
			, "[FC8180] IOCTL_ISDBT_SCAN_STATUS : %d\n", res);
		break;
	case IOCTL_ISDBT_TUNER_GET_RSSI:
		err = copy_from_user((void *)&info, (void *)arg, size);
		res = bbm_com_tuner_get_rssi(hInit, (s32 *)&info.buff[0]);
		err |= copy_to_user((void *)arg, (void *)&info, size);
		break;
	default:
		print_log(hInit, "isdbt ioctl error!\n");
		res = BBM_NOK;
		break;
	}

	if (err < 0) {
		print_log(hInit, "copy to/from user fail : %d", err);
		res = BBM_NOK;
	}
	return res;
}

int isdbt_init(void)
{
	s32 res;

	print_log(hInit, "isdbt_init 20150113\n");

	res = platform_driver_register(&dtv_platform_driver);
	if(res) {
		print_log(0, "fc8180 platform_driver_register fail\n");
		return res;
	}

	res = misc_register(&fc8180_misc_device);

	if (res < 0) {
		print_log(hInit, "isdbt init fail : %d\n", res);
		return res;
	}

	hInit = kmalloc(sizeof(struct ISDBT_INIT_INFO_T), GFP_KERNEL);


#if defined(BBM_I2C_TSIF) || defined(BBM_I2C_SPI)
	res = bbm_com_hostif_select(hInit, BBM_I2C);
#elif defined(BBM_SPI_IF)
	res = bbm_com_hostif_select(hInit, BBM_SPI);
#else
	res = bbm_com_hostif_select(hInit, BBM_PPI);
#endif

	if (res)
		print_log(hInit, "isdbt host interface select fail!\n");

#ifndef BBM_I2C_TSIF
	if (!isdbt_kthread)	{
		print_log(hInit, "kthread run\n");
		isdbt_kthread = kthread_run(isdbt_thread
			, (void *)hInit, "isdbt_thread");
	}
#endif

	INIT_LIST_HEAD(&(hInit->hHead));

	return 0;
}

void isdbt_exit(void)
{
	print_log(hInit, "isdbt isdbt_exit\n");

	platform_driver_unregister(&dtv_platform_driver);

	isdbt_hw_deinit();

#ifndef BBM_I2C_TSIF
	free_irq(GPIO_ISDBT_IRQ, NULL);
	gpio_free(GPIO_ISDBT_IRQ);
#endif
	gpio_free(GPIO_ISDBT_PWR_EN);

#ifndef BBM_I2C_TSIF
	if (isdbt_kthread)
		kthread_stop(isdbt_kthread);

	isdbt_kthread = NULL;
#endif

	bbm_com_hostif_deselect(hInit);

	if (hInit != NULL)
		kfree(hInit);

	misc_deregister(&fc8180_misc_device);

}

module_init(isdbt_init);
module_exit(isdbt_exit);

MODULE_LICENSE("Dual BSD/GPL");

