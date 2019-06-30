/* BEGIN PN:DTS2013053103858 , Added by d00238048, 2013.05.31*/
#ifndef BUILD_LK
    #include <linux/string.h>
    #include <linux/kernel.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
    #include <platform/mt_gpio.h>	
#elif defined(BUILD_UBOOT)
    #include <asm/arch/mt_gpio.h>
    #include <platform/mt_pmic.h>
#else
#ifdef CONFIG_MTK_LEGACY
    #include <mach/mt_gpio.h>
#endif
#endif
#ifdef BUILD_LK
#define LCD_DEBUG(fmt, args...) printf(fmt, ##args)		
#else
#define LCD_DEBUG(fmt, args...) printk(fmt, ##args)
#endif

#ifndef BUILD_LK
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/irq.h>
/* #include <linux/jiffies.h> */
/* #include <linux/delay.h> */
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#ifndef CONFIG_FPGA_EARLY_PORTING
#define NT_I2C_BUSNUM  2	/* for I2C channel 2 */
#define I2C_ID_NAME "nt50358"
#define NT_ADDR 0x3E

#if defined(CONFIG_MTK_LEGACY)
static struct i2c_board_info nt50358_board_info __initdata = { I2C_BOARD_INFO(I2C_ID_NAME, NT_ADDR) };
#endif
#if !defined(CONFIG_MTK_LEGACY)
static const struct of_device_id lcm_of_match[] = {
		{.compatible = "mediatek,I2C_LCD_BIAS"},
		{},
};
#endif

/*static struct i2c_client *nt50358_i2c_client;*/
struct i2c_client *nt50358_i2c_client;

/*****************************************************************************
 * Function Prototype
 *****************************************************************************/
static int nt50358_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int nt50358_remove(struct i2c_client *client);
/*****************************************************************************
 * Data Structure
 *****************************************************************************/

struct nt50358_dev {
	struct i2c_client *client;

};

static const struct i2c_device_id nt50358_id[] = {
	{I2C_ID_NAME, 0},
	{}
};

/* #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)) */
/* static struct i2c_client_address_data addr_data = { .forces = forces,}; */
/* #endif */
static struct i2c_driver nt50358_iic_driver = {
	.id_table = nt50358_id,
	.probe = nt50358_probe,
	.remove = nt50358_remove,
	/* .detect               = mt6605_detect, */
	.driver = {
		   .owner = THIS_MODULE,
		   .name = "nt50358",
#if !defined(CONFIG_MTK_LEGACY)
			.of_match_table = lcm_of_match,
#endif
		   },
};

static int nt50358_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	LCD_DEBUG("nt50358_iic_probe\n");
	LCD_DEBUG("NT: info==>name=%s addr=0x%x\n", client->name, client->addr);
	nt50358_i2c_client = client;
	return 0;
}

static int nt50358_remove(struct i2c_client *client)
{
	LCD_DEBUG("nt50358_remove\n");
	nt50358_i2c_client = NULL;
	i2c_unregister_device(client);
	return 0;
}

/*static int nt50358_write_bytes(unsigned char addr, unsigned char value)*/
#if !defined(CONFIG_ARCH_MT6797)
int nt50358_write_bytes(unsigned char addr, unsigned char value)
{
	int ret = 0;
	struct i2c_client *client = nt50358_i2c_client;
	char write_data[2] = { 0 };

	write_data[0] = addr;
	write_data[1] = value;
	ret = i2c_master_send(client, write_data, 2);
	if (ret < 0)
		LCD_DEBUG("nt50358 write data fail !!\n");
	return ret;
}
#endif

static int __init nt50358_iic_init(void)
{
	LCD_DEBUG("nt50358_iic_init\n");
#if defined(CONFIG_MTK_LEGACY)
	i2c_register_board_info(NT_I2C_BUSNUM, &nt50358_board_info, 1);
#endif
	LCD_DEBUG("nt50358_iic_init2\n");
	i2c_add_driver(&nt50358_iic_driver);
	LCD_DEBUG("nt50358_iic_init success\n");
	return 0;
}

static void __exit nt50358_iic_exit(void)
{
	LCD_DEBUG("nt50358_iic_exit\n");
	i2c_del_driver(&nt50358_iic_driver);
}


module_init(nt50358_iic_init);
module_exit(nt50358_iic_exit);

MODULE_AUTHOR("Longfang Liu");
MODULE_DESCRIPTION("MTK nt50358 I2C Driver");
MODULE_LICENSE("GPL");
#endif
#endif
