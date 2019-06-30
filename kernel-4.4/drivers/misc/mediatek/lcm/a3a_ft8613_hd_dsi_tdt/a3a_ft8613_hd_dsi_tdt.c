/* Modify lcd driver by llf for task 5325410 on 20170926*/
/* Modify lcd initcode by llf for task 5366411 on 20170928*/
/* Modify lcd ESD check and Deepmode by llf for task 5395134 on 20171011*/
/* Modify lcd ESD by liulongfang for FR 5443955 on 2017-10-24 */
/* Modify lcd FPS by liulongfang for FR 5729875 on 2017-12-06 */
/* Modify lcd Porch by liulongfang for FR 5729875 on 2017-12-13 */
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

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1440)

#define REGFLAG_DELAY             				0xFD
#define REGFLAG_END_OF_TABLE      				0xFF   // END OF REGISTERS MARKER

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef GPIO_LCD_BIAS_ENP_PIN
#define GPIO_LCD_BIAS_ENP_PIN        			(GPIO28 | 0x80000000)
#endif
#ifndef GPIO_LCD_BIAS_ENN_PIN
#define GPIO_LCD_BIAS_ENN_PIN        			(GPIO122 | 0x80000000)
#endif
#ifndef GPIO_LCM_ID0
#define GPIO_LCM_ID0				(GPIO110 | 0x80000000)
#endif
#ifndef GPIO_LCM_ID1
#define GPIO_LCM_ID1				(GPIO111 | 0x80000000)
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define SET_RESET_PIN(v)    				(lcm_util.set_reset_pin((v)))
#define MDELAY(n)					(lcm_util.mdelay(n))
#define UDELAY(n)					(lcm_util.udelay(n))
#define dsi_set_cmdq_V3(para_tbl,size,force_update)        lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)										lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)     
//BEGIN: add by fangjie for pop5s
#define set_gpio_lcd_enp(cmd) \
	lcm_util.set_gpio_lcd_enp_bias(cmd)
#define set_gpio_lcd_enn(cmd) \
	lcm_util.set_gpio_lcd_enn_bias(cmd)
struct LCM_setting_table {
	unsigned char cmd;
	unsigned char count;
	unsigned char para_list[64];
};
//update initial param for IC hx8395ad 0.01
/*
static struct LCM_setting_table  lcm_initialization_setting_ty[] = {

	{0x00,1,{0x00}},
	{0xff,3,{0x87,0x16,0x01}},
	{0x00,1,{0x80}},	
	{0xff,2,{0x87,0x16}},
	{0x00,1,{0x80}},
	{0xC0,0xf,{0x00,0xA5,0x00,0x10,0x10,0x00,0xA5,0x10,0x10,0x00,0xA5,0x00,0x10,0x10,0x00}},
	{0x00,1,{0x80}},
	{0xf3,1,{0x70}},
	{0x00,1,{0x90}},
	{0xc5,1,{0x55}},
	{0x00,1,{0xa0}},
	{0xc0,7,{0x0D,0x01,0x01,0x01,0x01,0x24,0x09}},
	{0x00,1,{0xd0}},
	{0xc0,7,{0x0D,0x01,0x01,0x01,0x01,0x24,0x09}},
	{0x00,1,{0xa8}},
	{0xf5,1,{0x22}},
	{0x00,1,{0x82}},
	{0xa5,3,{0x00,0x00,0x0c}},
	{0x00,1,{0x87}},
	{0xa5,4,{0x00,0x07,0x77,0x00}},
	{0x00,1,{0xa0}},
	{0xce,8,{0x00,0x05,0x01,0x01,0x01,0x01,0x3F,0x0A}},
	{0x00,1,{0xa0}},
	{0xb3,1,{0x32}},
	{0x00,1,{0xa6}},
	{0xb3,1,{0x58}},
	{0x00,1,{0x80}},
	{0xc2,0xc,{0x82,0x00,0x00,0x81,0x81,0x00,0x00,0x81,0x83,0x00,0x3F,0xB0}},
	{0x00,1,{0xb0}},
	{0xc2,0xf,{0x82,0x00,0x00,0x07,0x88,0x81,0x01,0x00,0x07,0x88,0x00,0x02,0x00,0x07,0x88}},
	{0x00,1,{0xc0}},
	{0xc2,5,{0x01,0x03,0x00,0x07,0x88}},
	{0x00,1,{0xda}},
	{0xc2,2,{0x33,0x33}},
	{0x00,1,{0x82}},
	{0xa5,3,{0x00,0x00,0x00}},
	{0x00,1,{0x87}},
	{0xa5,4,{0x00,0x07,0x77,0x77}},
	{0x00,1,{0x88}},
	{0xc3,2,{0x022,0x22}},
	{0x00,1,{0x98}},
	{0xc3,2,{0x22,0x22}},
	{0x00,1,{0xaa}},
	{0xc3,2,{0x99,0x9c}},
	{0x00,1,{0x90}},
	{0xc5,1,{0x55}},
	{0x00,1,{0x80}},
	{0xce,9,{0x25,0x00,0x74,0x00,0x78,0xFF,0x00,0x00,0x05}},
	{0x00,1,{0x90}},
	{0xce,8,{0x00,0x5C,0x0C,0xE4,0x00,0x5C,0x00,0x00}},
	{0x00,1,{0xb0}},
	{0xce,6,{0x00,0x00,0x60,0x60,0x00,0x60}},
	{0x00,1,{0xc0}},
	{0xf4,2,{0x93,0x36}},
	//{0x00,1,{0xb0}},
	//{0xf6,3,{0x69,0x16,0x1f}},
	{0x00,1,{0x80}},
	{0xcc,0xc,{0x02,0x03,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F}},
	{0x00,1,{0x90}},
	{0xcc,0xc,{0x03,0x02,0x09,0x08,0x07,0x06,0x0F,0x0E,0x0D,0x0C,0x0B,0x0A}},
	{0x00,1,{0xa0}},
	{0xcc,0xf,{0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x18,0x19,0x20,0x21,0x14,0x15,0x16,0x17,0x04}},
	{0x00,1,{0xb0}},
	{0xcc,5,{0x22,0x22,0x22,0x22,0x22}},
	{0x00,1,{0x80}},
	{0xcb,8,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	{0x00,1,{0x90}},
	{0xcb,0xf,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	{0x00,1,{0xa0}},
	{0xcb,0xf,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
	{0x00,1,{0xb0}},
	{0xcb,2,{0x00,0x00}},
	{0x00,1,{0xc0}},
	{0xcb,0xf,{0x05,0x05,0x05,0x05,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05,0x05}},
	{0x00,1,{0xd0}},
	{0xcb,0xf,{0x00,0x00,0x00,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x00,0x00}},
	{0x00,1,{0xe0}},
	{0xcb,2,{0x00,0x00}},
	{0x00,1,{0xf0}},
	{0xcb,8,{0xFF,0x0F,0x00,0x3F,0x30,0x30,0x30,0x00}},
	{0x00,1,{0x80}},
	{0xcd,0xf,{0x22,0x22,0x22,0x22,0x22,0x01,0x13,0x14,0x1B,0x05,0x03,0x17,0x18,0x18,0x22}},
	{0x00,1,{0x90}},
	{0xcd,3,{0x0f,0x0e,0x0d}},
	{0x00,1,{0xa0}},
	{0xcd,0xf,{0x22,0x22,0x22,0x22,0x22,0x02,0x13,0x14,0x1B,0x06,0x04,0x17,0x18,0x18,0x22}},
	{0x00,1,{0xb0}},
	{0xcd,3,{0x0f,0x0e,0x0d}},
	{0x00,1,{0x81}},
	{0xf3,0x0c,{0x40,0x01,0x00,0x40,0x01,0x00,0x40,0x01,0x00,0x40,0x01,0x00}},
//Code for gamma set as below
	{0x00,1,{0x00}},// Address Shift
	{0xe1,0x18,{0x00,0x06,0x18,0x2A,0x36,0x41,0x54,0x63,0x6A,0x72,0x7D,0x87,0x70,0x68,0x65,0x5E,0x51,0x45,0x37,0x2D,0x25,0x16,0x08,0x07}},
	{0x00,1,{0x00}}, // Address Shift
	{0xe2,0x18,{0x00,0x06,0x18,0x2A,0x36,0x41,0x54,0x63,0x6A,0x72,0x7D,0x87,0x70,0x68,0x65,0x5E,0x51,0x45,0x37,0x2D,0x25,0x16,0x08,0x07}},
//Code for gamma end
	{0x00,1,{0x80}},
	{0xc5,0x0a,{0x00,0xC1,0xDD,0xC4,0x14,0x1E,0x00,0x55,0x50,0x05}},
	{0x00,1,{0x90}},
	{0xc5,0xa,{0x77,0x1E,0x14,0x00,0x88,0x10,0x4C,0x53,0x55,0x50}},
	{0x00,1,{0x00}},
	{0xd8,2,{0x31,0x31}},
	{0x00,1,{0x00}},
	{0xd9,5,{0x80,0xBE,0xBE,0xBE,0xBE}},
	{0x00,1,{0xd1}},
	{0xcf,0xc,{0x02,0x04,0x00,0x0A,0x00,0x14,0x02,0x04,0x00,0x0C,0x00,0x12}},
	{0x00,1,{0xc0}},
	{0xc0,2,{0x11,0x11}},
	{0x00,1,{0x00}},
	{0x2a,4,{0x00,0x00,0x02,0xCF}},
	{0x00,1,{0x00}},
	{0x2b,4,{0x00,0x00,0x05,0x9f}},
	{0x00,1,{0xa1}},
	{0xb3,4,{0x02,0xD0,0x05,0xA0}},
	{0x00,1,{0x00}},
	{0x00,1,{0x8d}},
	{0xf5,1,{0x21}},
	//{0xd1,2,{0x00,0x0a}},
	{0x00,1,{0xb4}},
	{0xc0,1,{0x50}},

    	// Sleep Mode On
	{0x11,1,{0x00}},
	{REGFLAG_DELAY,200,{0x00}},
	{0x29,1,{0x00}},
	{REGFLAG_DELAY,20,{0x00}},
	{REGFLAG_END_OF_TABLE,1,{0x00}}
};*/

static void init_lcm_registers(void)
{
	unsigned int data_array[16];

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00043902;
	data_array[1] = 0x011687ff;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x001687ff;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x0200002A;
	data_array[2] = 0x000000CF;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x0500002B;
	data_array[2] = 0x0000009F;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000a000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00083902;
	data_array[1] = 0xD00203b3;
	data_array[2] = 0x5800A005;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00103902;
	data_array[1] = 0x00A600C0;
	data_array[2] = 0xA6001010;
	data_array[3] = 0xAd001010;
	data_array[4] = 0x00101000;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00083902;
	data_array[1] = 0x010105C0;
	data_array[2] = 0x0b260109;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000D000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00083902;
	data_array[1] = 0x010105C0;
	data_array[2] = 0x0b260109;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008200;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00043902;
	data_array[1] = 0x0C0444A5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008700;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x770700A5;
	data_array[2] = 0x00000077;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x000D3902;
	data_array[1] = 0x010084C2;
	data_array[2] = 0x01008381;
	data_array[3] = 0x3A008581;
	data_array[4] = 0x000000B5;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00103902;
	data_array[1] = 0x000484C2;
	data_array[2] = 0x05838607;
	data_array[3] = 0x82860700;
	data_array[4] = 0x86070002;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000C000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x000381C2;
	data_array[2] = 0x00008607;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000C500;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x000381C2;
	data_array[2] = 0x000000AF;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000DA00;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003333C2;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000AA00;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x009C39C3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000AC00;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000099C3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000D300;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000010C3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000E300;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000010C3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x000D3902;
	data_array[1] = 0x060302CC;
	data_array[2] = 0x0A090807;
	data_array[3] = 0x22222218;
	data_array[4] = 0x00000022;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x000D3902;
	data_array[1] = 0x090203CC;
	data_array[2] = 0x19060708;
	data_array[3] = 0x2222220A;
	data_array[4] = 0x00000022;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00103902;
	data_array[1] = 0x1C1B1ACC;
	data_array[2] = 0x181F1E1D;
	data_array[3] = 0x04212019;
	data_array[4] = 0x220A1514;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00063902;
	data_array[1] = 0x222222CC;
	data_array[2] = 0x00002222;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00093902;
	data_array[1] = 0x000000CB;
	data_array[2] = 0x00000000;
	data_array[3] = 0x00000000;
	dsi_set_cmdq(data_array, 4, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00103902;
	data_array[1] = 0x000000CB;
	data_array[2] = 0x00000000;
	data_array[3] = 0x00000000;
	data_array[4] = 0x00000000;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00103902;
	data_array[1] = 0x000000CB;
	data_array[2] = 0x00000000;
	data_array[3] = 0x00000000;
	data_array[4] = 0x00000000;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000000CB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000C000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00103902;
	data_array[1] = 0x050505CB;
	data_array[2] = 0x05050505;
	data_array[3] = 0x00000005;
	data_array[4] = 0x05050500;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000D000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00103902;
	data_array[1] = 0x000000CB;
	data_array[2] = 0x05050505;
	data_array[3] = 0x05000005;
	data_array[4] = 0x00000000;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000E000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000000CB;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000F000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00093902;
	data_array[1] = 0x00C00FCB;
	data_array[2] = 0x00C0003F;
	data_array[3] = 0x00000000;
	dsi_set_cmdq(data_array, 4, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00103902;
	data_array[1] = 0x191922CD;
	data_array[2] = 0x08060401;
	data_array[3] = 0x03171807;
	data_array[4] = 0x0F221A05;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00043902;
	data_array[1] = 0x220D0ECD;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00103902;
	data_array[1] = 0x191922CD;
	data_array[2] = 0x07030502;
	data_array[3] = 0x06171808;
	data_array[4] = 0x0F221A04;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00043902;
	data_array[1] = 0x220D0ECD;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008100;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x000D3902;
	data_array[1] = 0x40C108F3;
	data_array[2] = 0x0040C108;
	data_array[3] = 0x12000012;
	data_array[4] = 0x00000000;
	dsi_set_cmdq(data_array, 5, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x000A3902;
	data_array[1] = 0x780005CE;
	data_array[2] = 0x00FF7800;
	data_array[3] = 0x00000520;
	dsi_set_cmdq(data_array, 4, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00093902;
	data_array[1] = 0x0a5C00CE;
	data_array[2] = 0x005C0035;
	data_array[3] = 0x0000007b;
	dsi_set_cmdq(data_array, 4, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00073902;
	data_array[1] = 0x600000CE;
	data_array[2] = 0x00600060;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00053902;
	data_array[1] = 0xFE00FFCF;
	data_array[2] = 0x00000000;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009400;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x100000CF;
	data_array[2] = 0x00000020;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000A400;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00053902;
	data_array[1] = 0x010700CF;
	data_array[2] = 0x00000080;
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000d000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000008CF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000070F3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00193902;
	data_array[1] = 0x190700E1;
	data_array[2] = 0x5643372B;
	data_array[3] = 0x7F756C65;
	data_array[4] = 0x64676F88;
	data_array[5] = 0x36434f5d;
	data_array[6] = 0x0A17252D;
	data_array[7] = 0x00000007;
	dsi_set_cmdq(data_array, 8, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00193902;
	data_array[1] = 0x190700E2;
	data_array[2] = 0x5643372B;
	data_array[3] = 0x7F756C65;
	data_array[4] = 0x64676F88;
	data_array[5] = 0x36434f5d;
	data_array[6] = 0x0A17252D;
	data_array[7] = 0x00000007;
	dsi_set_cmdq(data_array, 8, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x000B3902;
	data_array[1] = 0xDDC100C5;
	data_array[2] = 0x001E14C4;
	data_array[3] = 0x00005055;
	dsi_set_cmdq(data_array, 4, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x000B3902;
	data_array[1] = 0x141E77C5;
	data_array[2] = 0x4B108800;
	data_array[3] = 0x0050553c;
	dsi_set_cmdq(data_array, 4, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003131D8;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008800;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003333C3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009800;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x003333C3;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008000;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000041C4;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009400;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000048C5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000C300;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000026F5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000C700;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000026F5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000D300;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000026F5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000D700;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000026F5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009500;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000026F5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00009800;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000026F5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000B100;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000021F5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008100;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000405CF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008400;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00033902;
	data_array[1] = 0x000405CF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008100;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000C0C4;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008D00;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000021F5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008c00;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000015F5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000DA00;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000016CF;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000C100;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000011C0;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000bb00;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000006C2;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000C100;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x000006C2;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00008900;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000c8CE;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x0000C200;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00007CC5;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0x00023902;
	data_array[1] = 0x00000035;
	dsi_set_cmdq(data_array, 2, 1);

	data_array[0] = 0xB4001500;
	dsi_set_cmdq(data_array, 1, 1);
	data_array[0] = 0x50C01500;
	dsi_set_cmdq(data_array, 1, 1);

	data_array[0] = 0x00110500;
	dsi_set_cmdq(data_array, 1, 1);

	MDELAY(120);//There is needed 120msec after Sleep Out -command

	data_array[0] = 0x00290500;
	dsi_set_cmdq(data_array, 1, 1);
	
	MDELAY(50);
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
	params->type   			= LCM_TYPE_DSI;
	params->width  			= FRAME_WIDTH;
	params->height 			= FRAME_HEIGHT;


	params->dsi.mode   = SYNC_PULSE_VDO_MODE;

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM		= LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

	// Highly depends on LCD driver capability.
	params->dsi.packet_size=256;

	// Video mode setting	

	params->dsi.intermediat_buffer_num = 2;	

	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

	params->dsi.word_count=480*3;

	//here is for esd protect by legen
	//params->dsi.noncont_clock = true;
	//params->dsi.noncont_clock_period=2;
	params->dsi.lcm_ext_te_enable=false;
	//for esd protest end by legen
	//params->dsi.word_count=FRAME_WIDTH*3;	
	params->dsi.vertical_sync_active=6;  //4
	params->dsi.vertical_backporch=16;	//16
	params->dsi.vertical_frontporch=18;
	params->dsi.vertical_active_line = FRAME_HEIGHT;

	//params->dsi.vertical_vfp_lp=260;//add by llf for low power vfs

	params->dsi.horizontal_sync_active=10;  
	params->dsi.horizontal_backporch=60;   //50  60 
	params->dsi.horizontal_frontporch=60;  //50   200
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;

	params->dsi.HS_TRAIL= 7;  // 4.3406868
	params->dsi.HS_PRPR = 4;
	
	params->dsi.cont_clock = 1;
	params->dsi.CLK_TRAIL= 50;
    	//params->dsi.compatibility_for_nvk = 1;
 	params->dsi.PLL_CLOCK=254;//add by longfang.liu
    	params->dsi.ssc_disable = 0;
	params->dsi.ssc_range = 5;

	params->dsi.esd_check_enable = 1;

	params->dsi.customization_esd_check_enable = 1;

	params->dsi.lcm_esd_check_table[0].cmd          = 0x0A;
	params->dsi.lcm_esd_check_table[0].count        = 1;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9C;  //page0

	params->dsi.lcm_esd_check_table[1].cmd          = 0x0D;
	params->dsi.lcm_esd_check_table[1].count        = 1;
	params->dsi.lcm_esd_check_table[1].para_list[0] = 0x00;  

	params->dsi.lcm_esd_check_table[2].cmd          = 0x0F;
	params->dsi.lcm_esd_check_table[2].count        = 1;
	params->dsi.lcm_esd_check_table[2].para_list[0] = 0xC0; 

#ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
	params->round_corner_en = 1;
	params->corner_pattern_width = 16;
	params->corner_pattern_height = 16;
#endif
}

static void push_table(struct LCM_setting_table *table, unsigned int count,
		       unsigned char force_update)
{
	unsigned int i;

	for (i = 0; i < count; i++) {
		unsigned cmd;

		cmd = table[i].cmd;

		switch (cmd) {

		case REGFLAG_DELAY:
			MDELAY(table[i].count);
			break;

		case REGFLAG_END_OF_TABLE:
			break;

		default:
			dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
		}
	}

}

static void lcm_init(void)
{
	unsigned char cmd = 0x0;
	unsigned char data = 0xFF;
	int ret=0;
    
	cmd=0x03;
	data=0x40;
	//03h,40---tablet mode

	ret=nt50358_write_bytes(cmd,data);
    	MDELAY(10);

	cmd=0x00;
	data=0x12; //vsp 5.8v
	//VPS=0x00;data=0x0A;VSP=5V,
	//         data=0x0E;VSP=5.4V,
	//VNG=0x01;data=0x0A;VNG=-5V,
	//         data=0x0E;VNG=-5.4V,
#ifndef CONFIG_FPGA_EARLY_PORTING
	//enable power
	//lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ZERO);
	//lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENN_PIN, GPIO_OUT_ZERO);
	set_gpio_lcd_enp(0);
	set_gpio_lcd_enn(0);
	MDELAY(10);

	//lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ONE);
	set_gpio_lcd_enp(1);
	MDELAY(10);

	ret=nt50358_write_bytes(cmd,data);
	if(ret<0)
	LCD_DEBUG("[KERNEL]LM3463-----nt50358a---cmd=%0x-- i2c write error-----\n",cmd);
	else
		LCD_DEBUG("[KERNEL]LM3463-----nt50358a---cmd=%0x-- i2c write success-----\n",cmd);
	MDELAY(10);
	//lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENN_PIN, GPIO_OUT_ONE);
	set_gpio_lcd_enn(1);
	MDELAY(10);
	cmd=0x01;
	data=0x12; //vsn -5.8v
	//VPS=0x00;data=0x0A;VSP=5V,
	//         data=0x0E;VSP=5.4V,
	//VNG=0x01;data=0x0A;VNG=-5V,
	//         data=0x0E;VNG=-5.4V,

	ret=nt50358_write_bytes(cmd,data);
	if(ret<0)
		LCD_DEBUG("[KERNEL]LM3463-----nt50358a---cmd=%0x-- i2c write error-----\n",cmd);
	else
		LCD_DEBUG("[KERNEL]LM3463-----nt50358a---cmd=%0x-- i2c write success-----\n",cmd);
	
	
#endif
	MDELAY(100);
	//reset high to low to high
	SET_RESET_PIN(1);
	MDELAY(20);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(120); //20170301 liujunting modify for HW requirement
	//push_table(lcm_initialization_setting_ty, sizeof(lcm_initialization_setting_ty) / sizeof(struct LCM_setting_table), 1);  
	init_lcm_registers();
	LCD_DEBUG("[LLF_Kernel]:a5a_infinish\n");
}
static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
    	// Sleep Mode On
	{0x28,1,{0x00}},
	{REGFLAG_DELAY,120,{0x00}},
	{0x10,1,{0x00}},
	{REGFLAG_DELAY,120,{0x00}},
	{0x00,1,{0xEA}},
	{0xF5,1,{0x01}},
	{0x00,1,{0x00}},
	{0xF7,4,{0x5A,0xA5,0x87,0x16}},
	{REGFLAG_DELAY,20,{0x00}},
	{REGFLAG_END_OF_TABLE,1,{0x00}},
};
static void lcm_suspend(void)
{
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);	
	//reset
    	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(5);
	SET_RESET_PIN(1);
	MDELAY(100);
	set_gpio_lcd_enn(1);
	set_gpio_lcd_enn(0);
	MDELAY(10);
	//mt_set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ONE);
	//mt_set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ZERO);
	set_gpio_lcd_enp(1);
	set_gpio_lcd_enp(0);
    	MDELAY(10);	
	LCD_DEBUG("lk:a3a_ft8613_hd_dsi_tdt_lcm_suspend\n");

}
static void lcm_resume(void)
{
/*
    //reset low to high
    SET_RESET_PIN(1);
    MDELAY(50);
    SET_RESET_PIN(0);
    MDELAY(50);
    SET_RESET_PIN(1);
    MDELAY(150);
    //enable power
    lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ZERO);
    lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENN_PIN, GPIO_OUT_ZERO);
    MDELAY(50);
    
    lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENP_PIN, GPIO_OUT_ONE);
    MDELAY(10);
    lcm_util.set_gpio_out(GPIO_LCD_BIAS_ENN_PIN, GPIO_OUT_ONE);
    MDELAY(10);

    //push_table(lcm_initialization_setting_ty, sizeof(lcm_initialization_setting_ty) / sizeof(struct LCM_setting_table), 1);
    init_lcm_registers();
*/
    lcm_init();
    LCD_DEBUG("lk:a3a_ft8613_hd_dsi_tdt_lcm_resume\n");

}

static unsigned int lcm_compare_id(void)
{	
#ifdef CONFIG_MTK_LEGACY
	int id_type=0;
	mt_set_gpio_mode(GPIO_LCM_ID0,GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_LCM_ID0, GPIO_DIR_IN);
	//mt_set_gpio_pull_select(GPIO_LCM_ID0,GPIO_PULL_DOWN);
	mt_set_gpio_pull_enable(GPIO_LCM_ID0, GPIO_PULL_DISABLE);// def 0
	
	mt_set_gpio_mode(GPIO_LCM_ID1,GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_LCM_ID1, GPIO_DIR_IN);
	//mt_set_gpio_pull_select(GPIO_LCM_ID1,GPIO_PULL_DOWN);
	mt_set_gpio_pull_enable(GPIO_LCM_ID1, GPIO_PULL_DISABLE);//def 0

	MDELAY(10);
	id_type = mt_get_gpio_in(GPIO_LCM_ID1)<<1 | mt_get_gpio_in(GPIO_LCM_ID0);
	if (id_type==0)//ID pin=01;
    		return 1;
	else 
		return 0;  
#else
	return 0;
#endif
		//return 1;//modify by xiaobin.zhai for RIM software
}
LCM_DRIVER a3a_ft8613_hd_dsi_tdt_lcm_drv =
{
    .name           = "a3a_ft8613_hd_dsi_tdt",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,/*kd init fun.*/
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
   
};
