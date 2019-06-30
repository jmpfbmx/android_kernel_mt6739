/*
 * Copyright (C) 2010 Trusted Logic S.A.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * history-1: Added by meng.zhang for task 5424939 on 2017/10/21 
 *			this driver is copied from longfang.liu, original check-in is: http://10.92.32.10:8081/#/c/168380/ 
 * history-2: Added by meng.zhang for task 5629044 on 2017/12/08 
 *			this change is to remove clk irq pin, read NFC IRQ level, add wakelock.
 * history-3: Added by meng.zhang for task 6141394 on 2018/03/26 
 *			this change is to reduce the log.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>
#include <linux/spinlock.h>
#include <linux/dma-mapping.h>
#include <linux/device.h>
#include <linux/wakelock.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>


#define PN553_DRVNAME		"pn544" 
#define MAX_BUFFER_SIZE		512
#define PN553_MAGIC			0xE9
#define PN553_SET_PWR		_IOW(PN553_MAGIC, 0x01, unsigned int)

#define NFC_DEBUG(fmt, args...)   pr_debug("[NFC-pn553] "fmt, ##args)
#define NFC_INFO(fmt, args...)      pr_info("[NFC-pn553] "fmt, ##args)
#define NFC_ERROR(fmt, args...)   pr_err("[NFC-pn553] "fmt, ##args)	

struct pn553_dev	{
	wait_queue_head_t	read_wq;
	struct mutex		read_mutex;
	struct i2c_client	*client;
	struct miscdevice	pn553_device;
	bool			irq_enabled;
	spinlock_t		irq_enabled_lock;
};

static struct wake_lock 		nfc_wackelock;

static struct pinctrl 			*nfc_pinctrl = NULL;
static struct pinctrl_state *pins_default = NULL;
static struct pinctrl_state *nfc_eint_high = NULL;
static struct pinctrl_state *nfc_eint_low = NULL;
static struct pinctrl_state *nfc_enb_high = NULL;
static struct pinctrl_state *nfc_enb_low = NULL;
static struct pinctrl_state *nfc_dl_high = NULL;
static struct pinctrl_state *nfc_dl_low = NULL;

static struct platform_device *nfc_platdev = NULL;
static struct pn553_dev *p_pn553_dev = NULL;

static int nfc_exist = 0;
static int nfc_irq_pin_num = -1;
static unsigned int pn553_irq;

/*----------------------------------------------------------------------------*/
extern struct device* get_deviceinfo_dev(void);
/*----------------------------------------------------------------------------*/
static ssize_t nfc_show(struct device *dev, struct device_attribute *attr, char *buf)
{	
    if (!dev )
	{
        return 0;
    }
    NFC_INFO("nfc_show() The nfc is pn553 \n");
    return snprintf(buf, PAGE_SIZE, "%s\n", "pn553:NXP:NA");
}
static ssize_t nfc_store(struct device* dev, struct device_attribute *attr, const char *buf, size_t count)
{
	return count;
}   
/*----------------------------------------------------------------------------*/
DEVICE_ATTR(NFC, S_IWUSR | S_IRUGO, nfc_show, nfc_store);
/*----------------------------------------------------------------------------*/

static int pn553_compatible_test(struct pn553_dev *pn553_dev)
{
	int ret;

/* Begin Modified by meng.zhang for task 5629044 on 2018/1/4 */
	//char send_reset[] = {0x20, 0x00, 0x01, 0x01};//pn553 RSET Frame
	//NFC_INFO("%s :don't send reset frame to avoid power leakage \n", __func__);
	//nfc_exist = 1;
	//return 0;
/* End Modified by meng.zhang for task 5629044 on 2018/1/4 */

/* Begin Modified by meng.zhang for task 5629044 on 2018/01/09 */
	unsigned char raw_fw_get_version_cmd[] =  {0x00, 0x04, 0xF1, 0x00, 0x00, 0x00, 0x6E, 0xEF};
	unsigned char fw_get_version_rsp[14] = {0};

	// DL high
	if (!IS_ERR(nfc_dl_high))
	{
		pinctrl_select_state(nfc_pinctrl, nfc_dl_high);
	}
	msleep(10);

	/* making sure that the NFCC starts in a clean state. */
	if (!IS_ERR(nfc_enb_low))
	{
		/* ULPM: Disable */
		pinctrl_select_state(nfc_pinctrl, nfc_enb_low);
	}
	/* hardware dependent delay */
	msleep(20);

	/* HPD : Enable*/
	if (!IS_ERR(nfc_enb_high))
	{
		pinctrl_select_state(nfc_pinctrl, nfc_enb_high);
	}
	 /* hardware dependent delay */
	msleep(20);
	 
	/* send get FW Version CMD */
	ret = i2c_master_send(pn553_dev->client, &raw_fw_get_version_cmd[0], sizeof(raw_fw_get_version_cmd));
	if (ret < 0) 
	{
		NFC_ERROR("%s : no pn553 or it is bad %d\n", __func__, ret);
		nfc_exist = 0;
		return -1;
	}
	nfc_exist = 1;
	NFC_INFO("%s :send raw_fw_get_version success %d\n", __func__, ret);

	/* hardware dependent delay */
	msleep(30); 

	/* Read Response of get fw version */
	ret = i2c_master_recv(pn553_dev->client, fw_get_version_rsp, sizeof(fw_get_version_rsp));
	NFC_INFO("%s: - nq - fw cmd answer : NfcNciRx %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n", __func__,
			fw_get_version_rsp[0], fw_get_version_rsp[1], fw_get_version_rsp[2], fw_get_version_rsp[3],
			fw_get_version_rsp[4], fw_get_version_rsp[5], fw_get_version_rsp[6], fw_get_version_rsp[7],
			fw_get_version_rsp[8], fw_get_version_rsp[9], fw_get_version_rsp[10], fw_get_version_rsp[11],
			fw_get_version_rsp[12], fw_get_version_rsp[13]);

	// power off
	if (!IS_ERR(nfc_dl_low))
	{
		pinctrl_select_state(nfc_pinctrl, nfc_dl_low);
	}
	if (!IS_ERR(nfc_enb_low))
	{
		pinctrl_select_state(nfc_pinctrl, nfc_enb_low);
	}

	if (ret < 0)
	{
		NFC_ERROR("%s : no pn553 or it is bad %d\n", __func__, ret);
		nfc_exist = 0;
		return -1;
	}
	nfc_exist = 1;
	NFC_INFO("%s :receive fw_get_version_rsp success %d\n", __func__, ret);

	return 0;
/* End Modified by meng.zhang for task 5629044 on 2018/01/09 */
}

static void pn553_disable_irq(struct pn553_dev *pn553_dev)
{
	unsigned long flags;

	spin_lock_irqsave(&pn553_dev->irq_enabled_lock, flags);
	if (pn553_dev->irq_enabled)
	{
		disable_irq_nosync(pn553_irq);
		pn553_dev->irq_enabled = false;
	}
	spin_unlock_irqrestore(&pn553_dev->irq_enabled_lock, flags);
}

static irqreturn_t  pn553_dev_irq_handler(int irq, void *dev_id)
{
	struct pn553_dev *pn553_dev = p_pn553_dev;

	if(!__gpio_get_value(nfc_irq_pin_num))
	{
		printk("%s:NFC_IRQ low \n", __func__);
		return IRQ_HANDLED;
	}

	pn553_disable_irq(pn553_dev);
	/* Wake up waiting readers */
	wake_up(&pn553_dev->read_wq);

	return IRQ_HANDLED;
}

static ssize_t pn553_dev_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	struct pn553_dev *pn553_dev = filp->private_data;
	int ret;
	char databuf[MAX_BUFFER_SIZE] = {0};

	//printk("pn553_dev_read read %zu bytes.\n", count);
	if (count > MAX_BUFFER_SIZE)
	{
		count = MAX_BUFFER_SIZE;
	}
	
	mutex_lock(&pn553_dev->read_mutex);
	if(!__gpio_get_value(nfc_irq_pin_num))
	{
		//NFC_INFO("%s wait for event......\n", __func__);
		if (filp->f_flags & O_NONBLOCK) 
		{
			ret = -EAGAIN;
			goto fail;
		}
		
		pn553_dev->irq_enabled = true;
		enable_irq(pn553_irq);
		ret = wait_event_interruptible(pn553_dev->read_wq, __gpio_get_value(nfc_irq_pin_num));
		pn553_disable_irq(pn553_dev);
		if (ret)
		{
			NFC_ERROR("%s: pn553 read wait event error %d \n", __func__, ret);
			goto fail;
		}
	}
	
	/* if nfc lock is inactive, wake lock  */
	if (wake_lock_active(&nfc_wackelock) == 0)
	{
		/* lock system to prevent from deep sleep */
		//NFC_INFO("nfc_wackelock wake_lock read\n");
		wake_lock(&nfc_wackelock);
	}
	
	ret = i2c_master_recv(pn553_dev->client, &databuf[0], count);
	
	if (wake_lock_active(&nfc_wackelock))	
	{		
		//NFC_INFO("nfc_wackelock wake_unlock read\n");	
		wake_unlock(&nfc_wackelock);
	}

	if (ret < 0)
	{
		NFC_ERROR("%s: i2c_master_recv returned %d\n", __func__, ret);
		goto fail;
	}
	if (ret > count)
	{
		NFC_ERROR("%s: received too many bytes from i2c (%d)\n", __func__, ret);
		goto fail;
	}
	
	if (copy_to_user(buf, &databuf[0], count)) 
	{
		NFC_ERROR("%s : failed to copy to user space\n", __func__);
		goto fail;
	}

	/* printk("pn553 read success IFD->PC: ");
	for(i = 0; i < count; i++) 
	{
		printk("%02X ", databuf[i]);
	}
	printk("\n"); */
	
	mutex_unlock(&pn553_dev->read_mutex);

	return ret;

fail:
	mutex_unlock(&pn553_dev->read_mutex);
	printk(KERN_DEBUG "%s : failed ret=%d \n", __func__, ret);
	return ret;
}

static ssize_t pn553_dev_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	struct pn553_dev *pn553_dev;
	int ret;
	char databuf[MAX_BUFFER_SIZE] = {0};

	pn553_dev = p_pn553_dev;
	//printk("pn553_dev_write write %zu bytes\n", count);

	if (count > MAX_BUFFER_SIZE)
	{
		count = MAX_BUFFER_SIZE;
	}

	/* if nfc lock is inactive, wake lock  */
	if (wake_lock_active(&nfc_wackelock) == 0)
	{
		/* lock system to prevent from deep sleep */
		//NFC_INFO("nfc_wackelock wake_lock write\n");
		wake_lock(&nfc_wackelock);
	}
	
	if (copy_from_user(databuf, buf, count)) 
	{
		printk(KERN_DEBUG "%s : failed to copy from user space\n", __func__);
		ret = -EFAULT;
		goto fail;
	}

	ret = i2c_master_send(pn553_dev->client, databuf, count);
	if (ret != count)
	{
		pr_err("%s : i2c_master_send returned %d\n", __func__, ret);
		ret = -EIO;
		goto fail;
	}

	/* printk("pn553 write success PC->IFD: ");
	for(i = 0; i < count; i++)
	{
		printk("%02X ", databuf[i]);
	}
	printk("\n"); */
	
	if (wake_lock_active(&nfc_wackelock))
	{
		//NFC_INFO("nfc_wackelock wake_unlock write\n");
		wake_unlock(&nfc_wackelock);
	}

	return ret;

fail:
	if (wake_lock_active(&nfc_wackelock))
	{
		NFC_INFO("fail: nfc_wackelock wake_unlock write\n");
		wake_unlock(&nfc_wackelock);
	}
	printk(KERN_DEBUG "%s : failed ret=%d \n", __func__, ret);
	return ret;
}

static int pn553_dev_open(struct inode *inode, struct file *filp)
{
	struct pn553_dev *pn553_dev = container_of(filp->private_data,
						struct pn553_dev,
						pn553_device);

	filp->private_data = pn553_dev;
	NFC_DEBUG("%s : %d,%d\n", __func__, imajor(inode), iminor(inode));

	return 0;
}

static long pn553_dev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch (cmd)
	{
		case PN553_SET_PWR:
			if (arg == 2)
			{
				/* power on with firmware download (requires hw reset) */
				NFC_INFO("%s power on with firmware\n", __func__);
				if (!IS_ERR(nfc_enb_high))
				{
					pinctrl_select_state(nfc_pinctrl, nfc_enb_high);
				}
				if (!IS_ERR(nfc_dl_high))
				{
					pinctrl_select_state(nfc_pinctrl, nfc_dl_high);
				}
				msleep(10);
				if (!IS_ERR(nfc_enb_low))
				{
					pinctrl_select_state(nfc_pinctrl, nfc_enb_low);
				}
				msleep(50);
				if (!IS_ERR(nfc_enb_high))
				{
					pinctrl_select_state(nfc_pinctrl, nfc_enb_high);
				}
				msleep(10);
			} 
			else if (arg == 1)
			{
				/* power on */
				NFC_INFO("%s power on\n", __func__);
				if (!IS_ERR(nfc_dl_low))
				{
					pinctrl_select_state(nfc_pinctrl, nfc_dl_low);
				}
				if (!IS_ERR(nfc_enb_high))
				{
					pinctrl_select_state(nfc_pinctrl, nfc_enb_high);
				}
				msleep(10);
			} 
			else  if (arg == 0)
			{
				/* power off */
				NFC_INFO("%s power off\n", __func__);
				if (!IS_ERR(nfc_dl_low))
				{
					pinctrl_select_state(nfc_pinctrl, nfc_dl_low);
				}
				if (!IS_ERR(nfc_enb_low))
				{
					pinctrl_select_state(nfc_pinctrl, nfc_enb_low);
				}
				msleep(10);
			}
			else
			{
				NFC_ERROR("%s bad arg %lu\n", __func__, arg);
				return -EINVAL;
			}
			break;
		default:
			NFC_ERROR("%s bad ioctl %u\n", __func__, cmd);
			return -EINVAL;
	}

	return 0;
}

static const struct file_operations pn553_dev_fops = {
	.owner	= THIS_MODULE,
	.llseek	= no_llseek,
	.read	= pn553_dev_read,
	.write	= pn553_dev_write,
	.open	= pn553_dev_open,
	.unlocked_ioctl  = pn553_dev_ioctl,
};

static int pn553_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	int err = 0;
	struct pn553_dev *pn553_dev;
	struct device_node *node;
	struct device *deviceinfo = NULL;

	NFC_INFO("nfc probe step01 is ok\n");

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
	{
		NFC_ERROR("%s : need I2C_FUNC_I2C\n", __func__);
		return  -ENODEV;
	}

	NFC_INFO("nfc probe step02 is ok\n");

	pn553_dev = kzalloc(sizeof(*pn553_dev), GFP_KERNEL);
	if (pn553_dev == NULL)
	{
		dev_err(&client->dev, "failed to allocate memory for module data\n");
		return -ENOMEM;
	}
	memset(pn553_dev, 0, sizeof(struct pn553_dev));
	p_pn553_dev = pn553_dev;

	NFC_INFO("nfc probe step03 is ok\n");
	
	pn553_dev->client = client;

	/* init mutex and queues */
	init_waitqueue_head(&pn553_dev->read_wq);
	mutex_init(&pn553_dev->read_mutex);
	spin_lock_init(&pn553_dev->irq_enabled_lock);
	//add wakelock for MT6739
	wake_lock_init(&nfc_wackelock, WAKE_LOCK_SUSPEND, "nfc suspend wakelock");
	
	pn553_dev->pn553_device.minor = MISC_DYNAMIC_MINOR;
	pn553_dev->pn553_device.name = PN553_DRVNAME;
	pn553_dev->pn553_device.fops = &pn553_dev_fops;

	ret = misc_register(&pn553_dev->pn553_device);
	if (ret)
	{
		NFC_ERROR("%s : misc_register failed\n", __FILE__);
		goto err_misc_register;
	}
	NFC_INFO("nfc probe step04 is ok\n");

	nfc_pinctrl = devm_pinctrl_get(&nfc_platdev->dev);
	if (IS_ERR(nfc_pinctrl))
	{
		err = PTR_ERR(nfc_pinctrl);
		NFC_ERROR("fcDebug devm_pinctrl_get error\n");
		return err;
	}
	
	/*Default*/
	pins_default = pinctrl_lookup_state(nfc_pinctrl, "default");
	if (IS_ERR(pins_default))
	{
		err = PTR_ERR(pins_default);
		NFC_INFO("fcDebug pinctrl_lookup_state default error\n");
	} 
	
	/* VEN */
	nfc_enb_low = pinctrl_lookup_state(nfc_pinctrl, "nfc_enb_low");
	if (IS_ERR(nfc_enb_low))
	{
		err = PTR_ERR(nfc_enb_low);
		NFC_ERROR("fcDebug pinctrl_lookup_state nfc_enb_low error\n");
		return err;
	}
	nfc_enb_high = pinctrl_lookup_state(nfc_pinctrl, "nfc_enb_high");
	if (IS_ERR(nfc_enb_high))
	{
		err = PTR_ERR(nfc_enb_high);
		NFC_ERROR("fcDebug pinctrl_lookup_state nfc_enb_high error\n");
		return err;
	}

	/* DL */
	nfc_dl_low = pinctrl_lookup_state(nfc_pinctrl, "nfc_dl_low");
	if (IS_ERR(nfc_dl_low))
	{
		err = PTR_ERR(nfc_dl_low);
		NFC_ERROR("fcDebug pinctrl_lookup_state nfc_dl_low error\n");
		return err;
	}
	nfc_dl_high = pinctrl_lookup_state(nfc_pinctrl, "nfc_dl_high");
	if (IS_ERR(nfc_dl_high))
	{
		err = PTR_ERR(nfc_dl_high);
		NFC_ERROR("fcDebug pinctrl_lookup_state nfc_dl_high error\n");
		return err;
	}

	/* NFC read IRQ */
	nfc_eint_high = pinctrl_lookup_state(nfc_pinctrl, "nfc_eint_high");
	if (IS_ERR(nfc_eint_high))
	{
		err = PTR_ERR(nfc_eint_high);
		NFC_ERROR("fcDebug pinctrl_lookup_state nfc_eint_high error\n");
		return err;
	}
	nfc_eint_low = pinctrl_lookup_state(nfc_pinctrl, "nfc_eint_low");
	if (IS_ERR(nfc_eint_low))
	{
		err = PTR_ERR(nfc_eint_low);
		NFC_ERROR("fcDebug pinctrl_lookup_state nfc_eint_low error\n");
		return err;
	}

	/* set input && pull down IRQ pin */
	if (!IS_ERR(nfc_enb_low)) 
	{
		pinctrl_select_state(nfc_pinctrl, nfc_enb_low);
	}
	if (!IS_ERR(nfc_eint_low)) 
	{
		pinctrl_select_state(nfc_pinctrl, nfc_eint_low);
	}
	if (!IS_ERR(nfc_dl_low)) 
	{
		pinctrl_select_state(nfc_pinctrl, nfc_dl_low);
	}
	NFC_INFO("nfc probe step05 is ok\n");
    
	pn553_dev->irq_enabled = true;
	node = of_find_compatible_node(NULL, NULL, "mediatek,pn553-eint-nfc");
	if (node)
	{
		pn553_irq = irq_of_parse_and_map(node, 0);
		client->irq = pn553_irq;
		ret = request_irq(pn553_irq,  pn553_dev_irq_handler, IRQF_TRIGGER_HIGH, "pn553-eint-nfc-eint", NULL);
		if (ret) 
		{
			ret = -1;
			NFC_ERROR("%s : requesting IRQ error\n", __func__);
		} 
		NFC_INFO("%s : requesting IRQ %d\n", __func__, pn553_irq);
	}
	else 
	{
		NFC_ERROR("%s : can not find NFC eint compatible node\n",  __func__);
	}	

/* Begin Modified by meng.zhang for task 5998696 on 2018/03/07 */
/* start from kernel-4.4, we need add this function, than the irq has the ability to resume the system */
	enable_irq_wake(pn553_irq);
/* End Modified by meng.zhang for task 5998696 on 2018/03/07 */

	pn553_disable_irq(pn553_dev);

	node = of_find_compatible_node(NULL, NULL, "mediatek,pn553-nfc");
	if (node)
	{
		nfc_irq_pin_num = of_get_named_gpio(node, "eint_intr_gpio_named", 0);
		if (ret)
		{
			ret = -1;
			NFC_ERROR("%s : eint_intr_gpio_named error\n", __func__);
		}
		NFC_INFO("nfc_irq_pin_num=%d \n", nfc_irq_pin_num);
	}
	else 
	{
		NFC_ERROR("%s : can not find mediatek,pn553-nfc node\n",  __func__);
	}
	
	i2c_set_clientdata(client, pn553_dev);
	NFC_INFO("nfc probe step06 is ok\n");

	// Check if pn553 chips exist
	pn553_compatible_test(p_pn553_dev);

	if (1 == nfc_exist) 
	{
		//add for device information
		deviceinfo = get_deviceinfo_dev();
		if(deviceinfo)
		{
			//NFC  /sys/class/deviceinfo/device_info/nfc
			if (device_create_file(deviceinfo, &dev_attr_NFC) < 0)
			{
				NFC_ERROR("Failed to create device file(%s)!\n", dev_attr_NFC.attr.name);
			}
		}
		else
		{
			NFC_ERROR("Failed to get deviceinfo device!\n");
		}
	}
	NFC_INFO("nfc probe step07 is ok\n");

	//if nfc_exist ==0 ,remove the dev/pn553 
	if (0 == nfc_exist) 
	{
		NFC_ERROR("%s : the pn553 is not exit, misc_register failed\n", __FILE__);
		disable_irq_wake(pn553_irq);   //zhihua  add to fix current high because without NFC IC,this irq will wakeup system,2018/04/20
		misc_deregister(&pn553_dev->pn553_device);
	}
	return 0;

err_misc_register:
	mutex_destroy(&pn553_dev->read_mutex);
	kfree(pn553_dev);
	p_pn553_dev = NULL;

	return ret;
}

static int pn553_remove(struct i2c_client *client)
{
	struct pn553_dev *pn553_dev;

	wake_lock_destroy(&nfc_wackelock);
	pn553_dev = i2c_get_clientdata(client);
	misc_deregister(&pn553_dev->pn553_device);
	mutex_destroy(&pn553_dev->read_mutex);
	kfree(pn553_dev);
	p_pn553_dev = NULL;
	
	return 0;
}

static const struct i2c_device_id pn553_id[] = { { PN553_DRVNAME, 0 }, {} };

#ifdef CONFIG_OF
static const struct of_device_id nfc_switch_of_match[] = {
	{.compatible = "mediatek,nfc"}, 
	{}, 
};
#endif

static struct i2c_driver pn553_driver = {
	.id_table	= pn553_id,
	.probe		= pn553_probe,
	.remove		= pn553_remove,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= PN553_DRVNAME,
#ifdef CONFIG_OF
		.of_match_table = nfc_switch_of_match,
#endif
	},
};

/*
 * module load/unload record keeping
 */
static int pn553_platform_probe(struct platform_device *pdev) 
{
 	 int ret;
 	 static unsigned char one_times = 0;
	 
	 if(0 == one_times)
	 {
	 	 NFC_INFO("add pn553_driver\n");
		 nfc_platdev = pdev;
		 ret = i2c_add_driver(&pn553_driver);
		 one_times++;
		 if(ret != 0)
		 {
		 	NFC_ERROR("add pn553_driver fail\n");
		 	return -1;
	 	}
	 }
	 else
	 {
		NFC_INFO("add pn553_driver another times!\n");
	 }	
	return 0;
}
static int pn553_platform_remove(struct platform_device *pdev) 
{
	i2c_del_driver(&pn553_driver);
	return 0;
}
#ifdef CONFIG_OF
static struct of_device_id nfc_of_match[] = {
	{.compatible = "mediatek,pn553-nfc",},
	{},
};
#endif

static struct platform_driver pn553_platform_driver = {
	.probe      = pn553_platform_probe,
	.remove     = pn553_platform_remove,    
	.driver     = 
	{
		.name  = PN553_DRVNAME,
		.owner = THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = nfc_of_match,
#endif
	},
};

static int __init pn553_dev_init(void)
{
	printk("pn553_dev_init\n");
	
	if(platform_driver_register(&pn553_platform_driver))
	{
		printk("platform_driver_register failed\n");
		return -ENODEV;
	}

	return 0;
}
module_init(pn553_dev_init);

static void __exit pn553_dev_exit(void)
{
	printk("pn553 Unloading pn553 driver\n");
	platform_driver_unregister(&pn553_platform_driver);
}
module_exit(pn553_dev_exit);

MODULE_AUTHOR("XXX");
MODULE_DESCRIPTION("NFC PN553 driver");
MODULE_LICENSE("GPL");
