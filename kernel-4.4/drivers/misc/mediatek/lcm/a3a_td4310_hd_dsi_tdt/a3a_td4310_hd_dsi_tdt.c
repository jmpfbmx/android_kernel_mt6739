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
#define FRAME_HEIGHT 										(1280)

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
#define set_gpio_lcd_enn(cmd) \
	lcm_util.set_gpio_lcd_enn_bias(cmd)
#define set_gpio_lcd_enp(cmd) \
	lcm_util.set_gpio_lcd_enp_bias(cmd)
struct LCM_setting_table {
	unsigned char cmd;
	unsigned char count;
	unsigned char para_list[64];
};
//update initial param for IC hx8395ad 0.01

static struct LCM_setting_table  lcm_initialization_setting_ty[] = {
/*
//mipi.write 0x29 0xB0 0x00
	{0xB0, 1,{0x00}},  
//mipi.write 0x29 0xB6 0x33 0x4B 0x81 0x12 0x00
	{0xB6, 5,{0x33,0x4B,0x81,0x12,0x00}},
//mipi.write 0x29 0xC5 0x08 0x00 0x00 0x00 0x00 0x70 0x00 0x03
	{0xC5, 8,{0x08,0x00,0x00,0x00,0x00,0x70,0x00,0x03}},
//mipi.write 0x29 0xCD 0x00 0x00 0x1b 0x00 0x1b 0x00 0x5C 0x02 0x74 0x74 0x6d 0x6d 0xA0 0xA0 0xA0 0xA0 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0xA2
	{0xCD, 38,{0x00,0x00,0x1b,0x00,0x1b,0x00,0x5C,0x02,0x74,0x74,
		0x6d,0x6d,0xA0,0xA0,0xA0,0xA0,0x01,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA2}},
//mipi.write 0x29 0xDD 0x31 0x06 0x23 0x65
	{0xDD, 4,{0x31,0x06,0x23,0x65}},
//mipi.write 0x29 0xD6 0x01
	{0xD6, 1,{0x01}},*/
//mipi.write 0x39 0x2A 0x00 0x00 0x02 0xCF
	{0x2A, 4,{0x00, 0x00, 0x02, 0xCF}},
	{0x35, 1,{0x00}},
	//{0x36, 1,{0xC0}},

    	// Sleep Mode On
	{0x11,1,{0x00}},
	{REGFLAG_DELAY,200,{0x00}},
	{0x29,1,{0x00}},
	{REGFLAG_DELAY,20,{0x00}},
	{REGFLAG_END_OF_TABLE,1,{0x00}},
};
/*
static void init_lcm_registers(void)
{
	unsigned int data_array[16];
//mipi.write 0x29 0xB0 0x00
	data_array[0]= 0x00022902;
	data_array[1]= 0x000000B0;
	dsi_set_cmdq(data_array, 2, 1);
//mipi.write 0x29 0xB6 0x33 0x4B 0x81 0x12 0x00
	data_array[0]= 0x00062902;
	data_array[1]= 0x814B33B6;
	data_array[2]= 0x00000012;
	dsi_set_cmdq(data_array, 3, 1);
	
//mipi.write 0x29 0xC5 0x08 0x00 0x00 0x00 0x00 0x70 0x00 0x03
	data_array[0]= 0x00092902;
	data_array[1]= 0x000008C5;
	data_array[2]= 0x00700000;
	data_array[3]= 0x00000003;
	dsi_set_cmdq(data_array, 4, 1);
//mipi.write 0x29 0xCD 0x00 0x00 0x1b 0x00 0x1b 0x00 0x5C 0x02 0x74 0x74 0x6d 0x6d 0xA0 0xA0 0xA0 0xA0 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0xA2
	data_array[0]= 0x00272902;
	data_array[1]= 0x1B0000CD;
	data_array[2]= 0x5C001B00;
	data_array[3]= 0x6D747402;
	data_array[4]= 0xA0A0A06D;
	data_array[5]= 0x000001A0;
	data_array[6]= 0x00000000;
	data_array[7]= 0x00000000;
	data_array[8]= 0x00000000;
	data_array[9]= 0x00000000;
	data_array[10]= 0x000000A2;
	dsi_set_cmdq(data_array, 11, 1);
//mipi.write 0x29 0xDD 0x31 0x06 0x23 0x65
	data_array[0]= 0x00052902;
	data_array[1]= 0x230631DD;
	data_array[2]= 0x00000065;
	dsi_set_cmdq(data_array, 3, 1);
//mipi.write 0x29 0xD6 0x01
	data_array[0]= 0x00022902;
	data_array[1]= 0x000001D6;
	dsi_set_cmdq(data_array, 2, 1);
//mipi.write 0x39 0x2A 0x00 0x00 0x02 0xCF
	data_array[0]= 0x00053902;
	data_array[1]= 0x0200002A;
	data_array[2]= 0x000000CF;
	dsi_set_cmdq(data_array, 3, 1);
//mipi.write 0x39 0x35 0x00
	data_array[0]= 0x00022902;
	data_array[1]= 0x00000035;
	dsi_set_cmdq(data_array, 2, 1);
//mipi.write 0x39 0x36 0xC0
	data_array[0]= 0x00022902;
	data_array[1]= 0x0000C036;
	dsi_set_cmdq(data_array, 2, 1);
	
	data_array[0] = 0x00110500;	//Sleep out
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(200);//There is needed 120msec after Sleep Out -command

	data_array[0] = 0x00290500;	//Display on
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(20);
}*/

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


	params->dsi.mode   = BURST_VDO_MODE;

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
	params->dsi.vertical_sync_active=2;  //4
	params->dsi.vertical_backporch=4;	//16
	params->dsi.vertical_frontporch=43;
	params->dsi.vertical_active_line = FRAME_HEIGHT;

	params->dsi.horizontal_sync_active=2;  

	params->dsi.horizontal_backporch=140;   //50  60 
	params->dsi.horizontal_frontporch=250;  //50   200
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;

	params->dsi.HS_TRAIL= 7;  // 4.3406868
	params->dsi.HS_PRPR = 4;
	
	params->dsi.CLK_TRAIL= 50;
    //params->dsi.compatibility_for_nvk = 1;
 	params->dsi.PLL_CLOCK=280;//add by longfang.liu PR527476
    //params->dsi.ssc_disable = 1;

#ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
	params->round_corner_en = 1;
	params->corner_pattern_width = 32;  //must match with LK config
	params->corner_pattern_height = 32; //must match with LK config
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

	//reset high to low to high
	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(5);
	SET_RESET_PIN(1);
	MDELAY(120); //20170301 liujunting modify for HW requirement
    
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
	LCD_DEBUG("[KERNEL]LM3463-----nt50358---cmd=%0x-- i2c write error-----\n",cmd);
	else
		LCD_DEBUG("[KERNEL]LM3463-----nt50358---cmd=%0x-- i2c write success-----\n",cmd);
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
		LCD_DEBUG("[KERNEL]LM3463-----nt50358---cmd=%0x-- i2c write error-----\n",cmd);
	else
		LCD_DEBUG("[KERNEL]LM3463-----nt50358---cmd=%0x-- i2c write success-----\n",cmd);
	
	
#endif
	MDELAY(10);
	push_table(lcm_initialization_setting_ty, sizeof(lcm_initialization_setting_ty) / sizeof(struct LCM_setting_table), 1);  
	//init_lcm_registers();
	LCD_DEBUG("[LLF_Kernel]:a5a_infini\n");
}
static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
    	// Sleep Mode On
	{0x28,1,{0x00}},
	{REGFLAG_DELAY,50,{0x00}},
	{0xB0,1,{0x00}},
	{0xD3, 21,{0x13,0x3B,0xBB,0x77,0x77,0x77,0xB3,0xB3,0x33,0x00,0x00,0x6E,0x61,0xD3,0xD3,0x33,0xBB,0xF2,0xFD,0xC6,0x0B}},
	{REGFLAG_DELAY,20,{0x00}},
	{0x10,1,{0x00}},
	{REGFLAG_DELAY,120,{0x00}},
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
	//LCD_DEBUG("lk:a3a_td4310_hd_dsi_tdt_lcm_suspend\n");

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
    //LCD_DEBUG("lk:a3a_td4310_hd_dsi_tdt_lcm_resume\n");

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
	if (id_type==1)//ID pin=01;
    		return 1;
	else 
		return 0;  
#else
	return 0;
#endif
		//return 1;//modify by xiaobin.zhai for RIM software
}
LCM_DRIVER a3a_td4310_hd_dsi_tdt_lcm_drv =
{
    .name           = "a3a_td4310_hd_dsi_tdt",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,/*kd init fun.*/
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
   
};
