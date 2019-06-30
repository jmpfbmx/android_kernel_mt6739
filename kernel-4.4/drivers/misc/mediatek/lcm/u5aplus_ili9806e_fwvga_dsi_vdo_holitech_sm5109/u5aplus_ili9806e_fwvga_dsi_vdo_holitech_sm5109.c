#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
#include <platform/upmu_common.h>
#include <platform/mt_gpio.h>
#include <platform/mt_i2c.h>
#include <platform/mt_pmic.h>
#include <string.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#endif

#ifdef BUILD_LK
#define LCD_DEBUG(fmt)  dprintf(CRITICAL,fmt)
#else
#define LCD_DEBUG(fmt)  printk(fmt)
#endif

#define LOG_TAG "u5aplus_ili9806e_fwvga_dsi_vdo_holitech_sm5109_drv"

#ifdef BUILD_LK
#define LCM_LOGI(string, args...)  dprintf(0, "[LK/"LOG_TAG"]"string, ##args)
#define LCM_LOGD(string, args...)  dprintf(1, "[LK/"LOG_TAG"]"string, ##args)
#else
#define LCM_LOGI(fmt, args...)  pr_notice("[KERNEL/"LOG_TAG"]"fmt, ##args)
#define LCM_LOGD(fmt, args...)  pr_debug("[KERNEL/"LOG_TAG"]"fmt, ##args)
#endif

static LCM_UTIL_FUNCS lcm_util;

#define SET_RESET_PIN(v)                                    (lcm_util.set_reset_pin((v)))
#define MDELAY(n)                                           (lcm_util.mdelay(n))
#define UDELAY(n)                                           (lcm_util.udelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)    lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)                                       lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)               lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
#define set_gpio_lcd_enp(cmd)                                                           lcm_util.set_gpio_lcd_enp_bias(cmd)
#define set_gpio_lcd_enn(cmd)                                                               lcm_util.set_gpio_lcd_enn_bias(cmd)

#ifndef BUILD_LK
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/irq.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/string.h>
#include <linux/wait.h>

#ifdef CONFIG_MTK_LEGACY
#include <mach/mt_gpio.h>
#include <cust_gpio_usage.h>
#include <cust_i2c.h>
#endif

#include "lcm_define.h"
#include "lcm_drv.h"
#include "lcm_i2c.h"

/*** begin: add by liuhe.zhong for round corner support on 2018.02.02 ***/
#ifdef MTK_ROUND_CORNER_SUPPORT
#include "data_rgba4444_roundedpattern.h"
#endif
/*** end:   add by liuhe.zhong for round corner support on 2018.02.02 ***/

/*****************************************************************************
 * Define
 *****************************************************************************/
#define I2C_I2C_LCD_BIAS_CHANNEL 0
#define TPS_I2C_BUSNUM  I2C_I2C_LCD_BIAS_CHANNEL//for I2C channel 0
#define I2C_ID_NAME "tps65132"
#define TPS_ADDR 0x3E

/*****************************************************************************
 * GLobal Variable
 *****************************************************************************/
static struct i2c_board_info __initdata tps65132_board_info = {I2C_BOARD_INFO(I2C_ID_NAME, TPS_ADDR)};
static struct i2c_client *tps65132_i2c_client = NULL;


/*****************************************************************************
 * Function Prototype
 *****************************************************************************/
static int tps65132_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int tps65132_remove(struct i2c_client *client);
/*****************************************************************************
 * Data Structure
 *****************************************************************************/

struct tps65132_dev    {
	struct i2c_client   *client;

};

static const struct i2c_device_id tps65132_id[] = {
	{ I2C_ID_NAME, 0 },
	{ }
};

static struct i2c_driver tps65132_iic_driver = {
	.id_table   = tps65132_id,
	.probe      = tps65132_probe,
	.remove     = tps65132_remove,
	//.detect       = mt6605_detect,
	.driver     = {
		.owner  = THIS_MODULE,
		.name   = "tps65132",
	},

};
/*****************************************************************************
 * Extern Area
 *****************************************************************************/



/*****************************************************************************
 * Function
 *****************************************************************************/
static int tps65132_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk( "tps65132_iic_probe\n");
	printk("TPS: info==>name=%s addr=0x%x\n",client->name,client->addr);
	tps65132_i2c_client  = client;
	return 0;
}


static int tps65132_remove(struct i2c_client *client)
{
	printk( "tps65132_remove\n");
	tps65132_i2c_client = NULL;
	i2c_unregister_device(client);
	return 0;
}

#if 0
static int tps65132_write_bytes(unsigned char addr, unsigned char value)
{
	int ret = 0;
	struct i2c_client *client = tps65132_i2c_client;
	char write_data[2]= {0};
	write_data[0]= addr;
	write_data[1] = value;

	ret=i2c_master_send(client, write_data, 2);
	if (ret<0)
		printk("tps65132 write data fail !!\n");
	return ret ;
}
#endif


/*
 * module load/unload record keeping
 */

static int __init tps65132_iic_init(void)
{

	printk( "tps65132_iic_init\n");
	i2c_register_board_info(TPS_I2C_BUSNUM, &tps65132_board_info, 1);
	printk( "tps65132_iic_init2\n");
	i2c_add_driver(&tps65132_iic_driver);
	printk( "tps65132_iic_init success\n");
	return 0;
}

static void __exit tps65132_iic_exit(void)
{
	printk( "tps65132_iic_exit\n");
	i2c_del_driver(&tps65132_iic_driver);
}


module_init(tps65132_iic_init);
module_exit(tps65132_iic_exit);

MODULE_AUTHOR("Xiaokuan Shi");
MODULE_DESCRIPTION("MTK TPS65132 I2C Driver");

#endif


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
/*** begin: added by liuhe.zhong for compatible with jd lcm driver on 2017.12.7 ***/
#define LCM_ID_ILI9806E                                                                         (0x9806)
/*** end:   added by liuhe.zhong for compatible with jd lcm driver on 2017.12.7 ***/
#define FRAME_WIDTH                                         (480)
#define FRAME_HEIGHT                                        (960)
#define LCM_PHYSICAL_WIDTH                                                                (60650)   //um
#define LCM_PHYSICAL_HEIGHT                                                               (121310)  //um

#define GPIO_65132_EN1 GPIO_LCD_BIAS_ENP_PIN
#define GPIO_65132_EN2 GPIO_LCD_BIAS_ENN_PIN

#define REGFLAG_DELAY                                           0xFC
#define REGFLAG_UDELAY                                          0xFB

#define REGFLAG_END_OF_TABLE                                0xFD   // END OF REGISTERS MARKER
#define REGFLAG_RESET_LOW                                       0xFE
#define REGFLAG_RESET_HIGH                                      0xFF

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------


struct LCM_setting_table {
	unsigned char cmd;
	unsigned char count;
	unsigned char para_list[64];
};

static struct LCM_setting_table lcm_suspend_setting[] = {
	{0x28,0,{}},
	{REGFLAG_DELAY, 120, {}},
	{0x10,0,{}},
	{REGFLAG_DELAY, 120, {}},
};


static void init_lcm_registers(void)
{
	unsigned int data_array[16];

	data_array[0] = 0x00063902;
	data_array[1] = 0x0698FFFF;
	data_array[2] = 0x00000104;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x10081500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x01211500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x772E1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x07301500;
	dsi_set_cmdq(data_array, 1, 1);
/*** begin: modified by liuhe.zhong for lcd high power consumption on 2018.01.11 ***/
	data_array[0] = 0x00311500;
/*** end:   modified by liuhe.zhong for lcd high power consumption on 2018.01.11 ***/
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x10401500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x55411500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x03421500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x09431500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x05441500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x68501500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x68511500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00521500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x84531500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x07601500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00611500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x0A621500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00631500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00A01500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x02A11500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x0AA21500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x0DA31500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x08A41500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x18A51500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x0AA61500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x09A71500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x04A81500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x08A91500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x05AA1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x04AB1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x0EAC1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x34AD1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x30AE1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00AF1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00C01500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x02C11500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x06C21500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x0DC31500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x06C41500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x13C51500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x0AC61500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x08C71500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x04C81500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x09C91500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x08CA1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x05CB1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x09CC1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x29CD1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x23CE1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00CF1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x0698FFFF;
	data_array[2] = 0x00000604;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x21001500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x06011500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00021500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00031500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x01041500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x01051500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x98061500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x02071500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x01081500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x80091500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x000A1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x000B1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x080C1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x080D1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x000E1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x000F1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0xF0101500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0xF4111500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x04121500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x85131500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00141500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0xC0151500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00161500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00171500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00181500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00191500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x001A1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x001B1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x001C1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x001D1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x01201500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x23211500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x45221500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x67231500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x01241500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x23251500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x45261500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x67271500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x01301500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x22311500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x22321500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x22331500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x22341500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0xBB351500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0xCA361500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0xDD371500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0xAC381500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x76391500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x673A1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x223B1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x223C1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x223D1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x223E1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x223F1500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x22401500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x10521500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x10531500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x13541500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x0698FFFF;
	data_array[2] = 0x00000704;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x22171500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x77021500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x79E11500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0xB2261500;
	dsi_set_cmdq(data_array, 1, 1);

//	data_array[0] = 0x13061500;
//	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x10B31500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x0698FFFF;
	data_array[2] = 0x00000004;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00110500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(120);

	data_array[0] = 0x00290500;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(10);	//modified by shiqiang.chen.hz for FR5597245 on 20171211, for the blank of panel.
};
static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

	for (i = 0; i < count; i++) {
		unsigned cmd;
		cmd = table[i].cmd;

		switch (cmd) {

			case REGFLAG_DELAY :
				if (table[i].count <= 10)
					MDELAY(table[i].count);
				else
					MDELAY(table[i].count);
				break;

			case REGFLAG_UDELAY :
				UDELAY(table[i].count);
				break;

			case REGFLAG_END_OF_TABLE :
				break;

			default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
		}
	}
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->type   = LCM_TYPE_DSI;

	params->width  = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;
	params->physical_width = LCM_PHYSICAL_WIDTH/1000;
	params->physical_height = LCM_PHYSICAL_HEIGHT/1000;
	params->physical_width_um = LCM_PHYSICAL_WIDTH;
	params->physical_height_um = LCM_PHYSICAL_HEIGHT;

	params->dsi.mode   = SYNC_PULSE_VDO_MODE;
	params->dsi.switch_mode = CMD_MODE;
	params->dsi.switch_mode_enable = 0;

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM                = LCM_TWO_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order     = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq       = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding         = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format              = LCM_DSI_FORMAT_RGB888;

	// Highly depends on LCD driver capability.
	params->dsi.packet_size=256;
	//video mode timing

	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

	params->dsi.vertical_sync_active                = 2;
	params->dsi.vertical_backporch                  = 8;
	params->dsi.vertical_frontporch                 = 18;
	params->dsi.vertical_active_line                    = FRAME_HEIGHT;

	params->dsi.horizontal_sync_active              = 8;
	params->dsi.horizontal_backporch                = 25;
	params->dsi.horizontal_frontporch               = 200;
	params->dsi.horizontal_active_pixel             = FRAME_WIDTH;

	params->dsi.PLL_CLOCK = 270; //this value must be in MTK suggested table

	params->dsi.clk_lp_per_line_enable = 0;
	params->dsi.esd_check_enable = 0;
	params->dsi.customization_esd_check_enable = 0;
	params->dsi.lcm_esd_check_table[0].cmd          = 0x53;
	params->dsi.lcm_esd_check_table[0].count        = 1;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x24;

/*** begin: add by liuhe.zhong for round corner support on 2018.02.02 ***/
	#ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
	params->corner_pattern_width = 12;//圆角的宽度
	params->corner_pattern_height = 12;//圆角的高度
	#endif
/*** end:   add by liuhe.zhong for round corner support on 2018.02.02 ***/

}

#ifdef BUILD_LK

#define TPS65132_SLAVE_ADDR_WRITE  0x7C
static struct mt_i2c_t TPS65132_i2c;

static int TPS65132_write_byte(kal_uint8 addr, kal_uint8 value)
{
	kal_uint32 ret_code = I2C_OK;
	kal_uint8 write_data[2];
	kal_uint16 len;

	write_data[0]= addr;
	write_data[1] = value;

	TPS65132_i2c.id = I2C_I2C_LCD_BIAS_CHANNEL;//I2C2;
	/* Since i2c will left shift 1 bit, we need to set FAN5405 I2C address to >>1 */
	TPS65132_i2c.addr = (TPS65132_SLAVE_ADDR_WRITE >> 1);
	TPS65132_i2c.mode = ST_MODE;
	TPS65132_i2c.speed = 100;
	len = 2;

	ret_code = i2c_write(&TPS65132_i2c, write_data, len);
	//printf("%s: i2c_write: ret_code: %d\n", __func__, ret_code);

	return ret_code;
}

#endif

static void lcm_init_power(void)
{
/*
	unsigned char cmd = 0x0;
	unsigned char data = 0xFF;
	int ret=0;

	set_gpio_lcd_enp(1);
	cmd=0x00;
	data=0x0F;
#ifdef BUILD_LK
	ret=TPS65132_write_byte(cmd,data);
#else
	ret=tps65132_write_bytes(cmd,data);
#endif
	if (ret<0)
		LCM_LOGI("[KERNEL]nt35595----tps6132---cmd=%0x-- i2c write error-----\n",cmd);
	else
		LCM_LOGI("[KERNEL]nt35595----tps6132---cmd=%0x-- i2c write success-----\n",cmd);

	set_gpio_lcd_enn(1);
	cmd=0x01;
	data=0x0F;
#ifdef BUILD_LK
	ret=TPS65132_write_byte(cmd,data);
#else
	ret=tps65132_write_bytes(cmd,data);
#endif

	if (ret<0)
		LCM_LOGI("[KERNEL]nt35595----tps6132---cmd=%0x-- i2c write error-----\n",cmd);
	else
		LCM_LOGI("[KERNEL]nt35595----tps6132---cmd=%0x-- i2c write success-----\n",cmd);
*/
	LCM_LOGI("%s\n", __func__);
}

static void lcm_suspend_power(void)
{
	LCM_LOGI("%s\n", __func__);
}

static void lcm_resume_power(void)
{
	lcm_init_power();
	LCM_LOGI("%s\n", __func__);
}


static void lcm_init(void)
{

	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(10);

	SET_RESET_PIN(1);
	MDELAY(10);	//modified by shiqiang.chen.hz for FR5597245 on 20171211, for the blank of panel.

	//push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
	init_lcm_registers();
	LCM_LOGI("%s\n", __func__);
}

static void lcm_suspend(void)
{
	push_table(lcm_suspend_setting, sizeof(lcm_suspend_setting) / sizeof(struct LCM_setting_table), 1);
	//set_gpio_lcd_enn(0);
	//MDELAY(5);
	//set_gpio_lcd_enp(0);
	//SET_RESET_PIN(0);
	//MDELAY(10);
	LCM_LOGI("%s\n", __func__);
}

static void lcm_resume(void)
{
	lcm_init();
	LCM_LOGI("%s\n", __func__);
}

static unsigned int lcm_compare_id(void)
{
/*** begin: added by liuhe.zhong for compatible with jd lcm driver on 2017.12.7 ***/
	    unsigned int id=0;
	    unsigned char buffer[2];
	    unsigned int array[16];

	    SET_RESET_PIN(1);
	    MDELAY(2);
	    SET_RESET_PIN(0);
	    MDELAY(10);
	    SET_RESET_PIN(1);
	    MDELAY(120);
	    array[0]=0x00063902;
	    array[1]=0x0698FFFF;
	    array[2]=0x00000104;// page enable
	    dsi_set_cmdq(array, 3, 1);
	    MDELAY(10);
	    array[0] = 0x00013700;// return byte number
	    dsi_set_cmdq(array, 1, 1);
	    MDELAY(10);
	    read_reg_v2(0x00, buffer, 1);
	    read_reg_v2(0x01, buffer+1, 1);
	    id = buffer[0]<< 8 | buffer[1];

	    LCM_LOGI("%s, id = 0x%x\n", __func__, id);
	    return (LCM_ID_ILI9806E == id)?1:0;
/*** end:   added by liuhe.zhong for compatible with jd lcm driver on 2017.12.7 ***/
//	return 1;
}


LCM_DRIVER u5aplus_ili9806e_fwvga_dsi_vdo_holitech_sm5109_lcm_drv= {
	.name               = "u5aplus_ili9806e_fwvga_dsi_vdo_holitech_sm5109_drv",
	.set_util_funcs     = lcm_set_util_funcs,
	.get_params         = lcm_get_params,
	.init                   = lcm_init,
	.suspend            = lcm_suspend,
	.resume             = lcm_resume,
	.compare_id        = lcm_compare_id,
	.init_power        = lcm_init_power,
	.resume_power = lcm_resume_power,
	.suspend_power = lcm_suspend_power,
};

