/*****************************************************************************
 *
 * Filename:
 * ---------
 *	 HI1333mipi_Sensor.c
 *
 * Project:
 * --------
 *	 ALPS
 *
 * Description:
 * ------------
 *	 Source code of Sensor driver
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
/************************************************* 
History: 1.0 Added by zhanyong.yin for XR 5581518 on 2017/11/09
*************************************************/
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/atomic.h>

#include "kd_camera_typedef.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"

#include "hi1333_mipi_raw_Sensor.h"

#define PFX "HI1333_camera_sensor"

#define LOG_INF(format, args...)	pr_debug(PFX "[%s] " format, __FUNCTION__, ##args)

//#define ENABLE_IVHDR 1
#define ENABLE_PDAF 1
#define e2prom 1
//test KYM
unsigned char bTh = 0;
unsigned char bGap = 0;

bool 	pip_mode =0;

//extern bool read_hi1333_eeprom( kal_uint16 addr, BYTE *data, kal_uint32 size); 
//extern bool read_eeprom( kal_uint16 addr, BYTE * data, kal_uint32 size);
extern void kdSetI2CSpeed(u16 i2cSpeed);


#ifdef ENABLE_IVHDR
static void SetIHDR(BOOL EnableIHDR);	//Add iHDR enable function
#endif

static DEFINE_SPINLOCK(imgsensor_drv_lock);

static imgsensor_info_struct imgsensor_info = { 
	.sensor_id = HI1333_SENSOR_ID,
	
	.checksum_value = 0xabaa55c3,       //0x6d01485c // Auto Test Mode ÃßÈÄ..
	.pre = {
		.pclk = 600000000,				//record different mode's pclk
		.linelength =  5952, 			//record different mode's linelength
		.framelength = 3360,//3300, 			//record different mode's framelength
		.startx = 0,				    //record different mode's startx of grabwindow
		.starty = 0,					//record different mode's starty of grabwindow
		.grabwindow_width = 2104, //2104,		//record different mode's width of grabwindow
		.grabwindow_height = 1560,		//record different mode's height of grabwindow
		/*	 following for MIPIDataLowPwr2HighSpeedSettleDelayCount by different scenario	*/
		.mipi_data_lp2hs_settle_dc = 85,
		/*	 following for GetDefaultFramerateByScenario()	*/
		.max_framerate = 300,	
	},
	.cap = {
		.pclk = 600000000,
		.linelength = 5952, 	
		.framelength = 3360, 
		.startx = 0,	
		.starty = 0,
		.grabwindow_width = 4208,
		.grabwindow_height = 3120,
		.mipi_data_lp2hs_settle_dc = 85,
		.max_framerate = 300,
	},
	// need to setting
    .cap1 = {                            
		//.pclk = 600000000,
		.pclk = 300000000,
		//.linelength = 11904,	 
		.linelength = 5952,	 
		.framelength = 3360,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 4208,
		.grabwindow_height = 3120,
		.mipi_data_lp2hs_settle_dc = 85,
		.max_framerate = 150,
    },
	.normal_video = {
		//test
#if 0
	.pclk = 600000000,				//record different mode's pclk
		.linelength = 5952, 			//record different mode's linelength
		.framelength = 3360,//3300, 			//record different mode's framelength
		.startx = 0,				    //record different mode's startx of grabwindow
		.starty = 0,					//record different mode's starty of grabwindow
		.grabwindow_width = 2104, //2104,		//record different mode's width of grabwindow
		.grabwindow_height = 1560,		//record different mode's height of grabwindow
		/*	 following for MIPIDataLowPwr2HighSpeedSettleDelayCount by different scenario	*/
		.mipi_data_lp2hs_settle_dc = 85,
		/*	 following for GetDefaultFramerateByScenario()	*/
		.max_framerate = 300,	
#endif
#if 1 
		.pclk = 600000000,
		.linelength = 5952, 	
		.framelength = 3360,
		.startx = 0,	
		.starty = 0,
		.grabwindow_width = 4208,
		.grabwindow_height = 3120,
		.mipi_data_lp2hs_settle_dc = 85,
		.max_framerate = 300,
#endif
	},
	.hs_video = {
        .pclk = 600000000,
        .linelength = 5952,				
		.framelength = 840, //832,			
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 640 ,		
		.grabwindow_height = 480 ,
		.mipi_data_lp2hs_settle_dc = 85,//unit , ns
		.max_framerate = 1200,
	},
    .slim_video = {
		.pclk = 600000000,
		.linelength = 5952,
		.framelength = 840,//832, 
		.startx = 0,
		.starty = 0,
    	.grabwindow_width = 1280,
    	.grabwindow_height = 720,
    	.mipi_data_lp2hs_settle_dc = 85,//unit , ns
    	.max_framerate = 1200, //1200,
    },
		//.margin = 7,
		.margin = 7,
		//.margin = 10,
		.min_shutter = 7,
		.max_frame_length = 0xffff,
		.ae_shut_delay_frame = 0,
		.ae_sensor_gain_delay_frame = 0,
		.ae_ispGain_delay_frame = 2,
    	.ihdr_support = 0,        //1, support; 0,not support
   		.ihdr_le_firstline = 0,   //1,le first ; 0, se first
		.sensor_mode_num = 5,	  //support sensor mode num
	
		.cap_delay_frame = 3, 
		.pre_delay_frame = 3,
		.video_delay_frame = 3,
    	.hs_video_delay_frame = 3,    //enter high speed video  delay frame num
    	.slim_video_delay_frame = 3,  //enter slim video delay frame num
	
		.isp_driving_current = ISP_DRIVING_6MA,
		.sensor_interface_type = SENSOR_INTERFACE_TYPE_MIPI,
		.mipi_sensor_type = MIPI_OPHY_NCSI2, //0,MIPI_OPHY_NCSI2;  1,MIPI_OPHY_CSI2
		.mipi_settle_delay_mode = MIPI_SETTLEDELAY_MANUAL, //0,MIPI_SETTLEDELAY_AUTO; 1,MIPI_SETTLEDELAY_MANNUAL
		.sensor_output_dataformat = SENSOR_OUTPUT_FORMAT_RAW_Gb,
		.mclk = 24,
		.mipi_lane_num = SENSOR_MIPI_4_LANE,
		.i2c_addr_table = {0x40, 0xff},
		.i2c_speed = 400,
};


static imgsensor_struct imgsensor = {
		.mirror = IMAGE_NORMAL,				//mirrorflip information
		.sensor_mode = IMGSENSOR_MODE_INIT, //IMGSENSOR_MODE enum value,record current sensor mode,such as: INIT, Preview, Capture, Video,High Speed Video, Slim Video
		.shutter = 0x0100,					//current shutter
		.gain = 0xe0,						//current gain
		.dummy_pixel = 0,					//current dummypixel
		.dummy_line = 0,					//current dummyline
		.current_fps = 300,                 //full size current fps : 24fps for PIP, 30fps for Normal or ZSD
		.autoflicker_en = KAL_FALSE,        //auto flicker enable: KAL_FALSE for disable auto flicker, KAL_TRUE for enable auto flicker
 		.test_pattern = KAL_FALSE, 
		.current_scenario_id = MSDK_SCENARIO_ID_CAMERA_PREVIEW,//current scenario id
 		.ihdr_en = 0, 
		.i2c_write_id = 0x40,
};

static SENSOR_WINSIZE_INFO_STRUCT imgsensor_winsize_info[5] =
{
	{ 4240, 3124,   0,   0, 4240, 3124,	 2120, 1562,  16, 2, 2104, 1560, 0, 0, 2104, 1560},		// preview (2104 x 1560)
	{ 4240, 3124,   0,   0, 4240, 3124,	 4240, 3124,  16, 2, 4208, 3120, 0, 0, 4208, 3120},		// capture (4208 x 3120)
	{ 4240, 3124,   0,   0, 4240, 3124,	 4240, 3124,  16, 2, 4208, 3120, 0, 0, 4208, 3120},		// VIDEO (4208 x 3120)
	{ 4240, 3124,   0, 110, 4240, 2904,	  704,  484,  34, 2,  640,  480, 0, 0,  640,  480},		// hight speed video (640 x 480)
 	{ 4240, 3124,   0, 476, 4240, 2172,  1408,  724,  66, 2, 1280,  720, 0, 0, 1280,  720},       // slim video (1280 x 720)
};

#if ENABLE_PDAF
static SET_PD_BLOCK_INFO_T imgsensor_pd_info =
{
	.i4OffsetX	= 56,
	.i4OffsetY	= 24,
	.i4PitchX	= 64,
	.i4PitchY	= 64,
	.i4PairNum	= 16,
	.i4SubBlkW	= 16,
	.i4SubBlkH	= 16,
	.iMirrorFlip = 0, // 
	.i4PosL = 	{
		         {67,36}, {79,36}, {99,36}, {111,36}, 
				 {63,48}, {83,48}, {95,48}, {115,48}, 
				 {67,68}, {79,68}, {99,68}, {111,68}, 
				 {63,80}, {83,80}, {95,80}, {115,80}
				},
	.i4PosR = 	{
			     {67,32}, {79,32}, {99,32}, {111,32}, 
				 {63,52}, {83,52}, {95,52}, {115,52}, 
				 {67,64}, {79,64}, {99,64}, {111,64}, 
				 {63,84}, {83,84}, {95,84}, {115,84}
				}
};
#endif

static kal_uint16 read_cmos_sensor(kal_uint32 addr)
{
	kal_uint16 get_byte=0;

	char pu_send_cmd[2] = {(char)(addr >> 8), (char)(addr & 0xFF)};
	kdSetI2CSpeed(imgsensor_info.i2c_speed); // Add this func to set i2c speed by each sensor
	iReadRegI2C(pu_send_cmd, 2, (u8*)&get_byte, 1, imgsensor.i2c_write_id);

	return get_byte;
}

static void write_cmos_sensor(kal_uint32 addr, kal_uint32 para)
{
	char pu_send_cmd[4] = {(char)(addr >> 8), (char)(addr & 0xFF), (char)(para >> 8), (char)(para & 0xFF)};

	iWriteRegI2C(pu_send_cmd, 4, imgsensor.i2c_write_id);
}

#if 1 
static void write_cmos_sensor1D(kal_uint32 addr, kal_uint32 para)
{
	char pu_send_cmd[3] = {(char)(addr >> 8), (char)(addr & 0xFF), (char)(para & 0xFF)};
 
	iWriteRegI2C(pu_send_cmd, 3, imgsensor.i2c_write_id); 
}
#endif

static void set_dummy(void)
{
	LOG_INF("dummyline = %d, dummypixels = %d \n", imgsensor.dummy_line, imgsensor.dummy_pixel);
	write_cmos_sensor(0x0006, imgsensor.frame_length); 

	if(pip_mode)
		write_cmos_sensor(0x0008, imgsensor.line_length / 4);
	else
		write_cmos_sensor(0x0008, imgsensor.line_length / 8);

}	/*	set_dummy  */

static void set_max_framerate(UINT16 framerate,kal_bool min_framelength_en)
{
	//kal_int16 dummy_line;
	kal_uint32 frame_length = imgsensor.frame_length;
	//unsigned long flags;

	LOG_INF("framerate = %d, min framelength should enable = %d\n", framerate,min_framelength_en);
   
	frame_length = imgsensor.pclk / framerate * 10 / imgsensor.line_length;
	spin_lock(&imgsensor_drv_lock);
	imgsensor.frame_length = (frame_length > imgsensor.min_frame_length) ? frame_length : imgsensor.min_frame_length; 
	imgsensor.dummy_line = imgsensor.frame_length - imgsensor.min_frame_length;

	if (imgsensor.frame_length > imgsensor_info.max_frame_length)
	{
		imgsensor.frame_length = imgsensor_info.max_frame_length;
		imgsensor.dummy_line = imgsensor.frame_length - imgsensor.min_frame_length;
	}
	if (min_framelength_en)
		imgsensor.min_frame_length = imgsensor.frame_length;
	spin_unlock(&imgsensor_drv_lock);
	set_dummy();

}	/*	set_max_framerate  */

static void write_shutter(kal_uint16 shutter)
{
	kal_uint16 realtime_fps = 0;
	//kal_uint32 frame_length = 0;

	LOG_INF("E\n");
	/* 0x3500, 0x3501, 0x3502 will increase VBLANK to get exposure larger than frame exposure */
	/* AE doesn't update sensor gain at capture mode, thus extra exposure lines must be updated here. */
	
	// OV Recommend Solution
	// if shutter bigger than frame_length, should extend frame length first
	spin_lock(&imgsensor_drv_lock);
	if (shutter > imgsensor.min_frame_length - imgsensor_info.margin)		
	{
		imgsensor.frame_length = shutter + imgsensor_info.margin;
	}
	else
	{
		imgsensor.frame_length = imgsensor.min_frame_length;
	}

	if (imgsensor.frame_length > imgsensor_info.max_frame_length)
	{
		imgsensor.frame_length = imgsensor_info.max_frame_length;
	}
	spin_unlock(&imgsensor_drv_lock);
	shutter = (shutter < imgsensor_info.min_shutter) ? imgsensor_info.min_shutter : shutter;
    shutter = (shutter > (imgsensor_info.max_frame_length - imgsensor_info.margin)) ? (imgsensor_info.max_frame_length - imgsensor_info.margin) : shutter;
	
	if (imgsensor.autoflicker_en) { 
		realtime_fps = imgsensor.pclk * 10 / (imgsensor.line_length * imgsensor.frame_length);
		if(realtime_fps >= 297 && realtime_fps <= 305)
			set_max_framerate(296,0);
		else if(realtime_fps >= 147 && realtime_fps <= 150)
			set_max_framerate(146,0);	
		else{

			LOG_INF("Enable autoflicker\n");	
		    write_cmos_sensor(0x0006, imgsensor.frame_length);
	        }
	} else {
			LOG_INF("Disable autoflicker\n");	
		// Extend frame length
		    write_cmos_sensor(0x0006, imgsensor.frame_length);
	}

	//write_cmos_sensor(0x0006, imgsensor.frame_length);
	write_cmos_sensor1D(0x0073, ((shutter & 0xFF0000) >> 16));	  
	write_cmos_sensor(0x0074, shutter);
	

	LOG_INF("frame_length = %d , shutter = %d \n", imgsensor.frame_length, shutter);


}	/*	write_shutter  */



/*************************************************************************
* FUNCTION
*	enable set_shutter
*
* DESCRIPTION
*	This function set e-shutter of sensor to change exposure time.
*
* PARAMETERS
*	iShutter : exposured lines
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static void set_shutter(kal_uint16 shutter)
{
	unsigned long flags;
	spin_lock_irqsave(&imgsensor_drv_lock, flags);
	imgsensor.shutter = shutter;
	spin_unlock_irqrestore(&imgsensor_drv_lock, flags);

	write_shutter(shutter);
}	/*	set_shutter */

static kal_uint16 gain2reg(const kal_uint16 gain)
{
    kal_uint16 reg_gain = 0x0000;
    reg_gain = gain / 4 - 16;

    return (kal_uint16)reg_gain;

}
/*************************************************************************
* FUNCTION
*	set_gain
*
* DESCRIPTION
*	This function is to set global gain to sensor.
*
* PARAMETERS
*	iGain : sensor global gain(base: 0x40)
*
* RETURNS
*	the actually gain set to sensor.
*
* GLOBALS AFFECTED
*
*************************************************************************/

static kal_uint16 set_gain(kal_uint16 gain)
{
	kal_uint16 reg_gain;

    /* 0x350A[0:1], 0x350B[0:7] AGC real gain */
    /* [0:3] = N meams N /16 X    */
    /* [4:9] = M meams M X         */
    /* Total gain = M + N /16 X   */

    if (gain < BASEGAIN || gain > 16 * BASEGAIN) {
        LOG_INF("Error gain setting");

        if (gain < BASEGAIN)
            gain = BASEGAIN;
        else if (gain > 16 * BASEGAIN)
            gain = 16 * BASEGAIN;
    }

    reg_gain = gain2reg(gain);
    spin_lock(&imgsensor_drv_lock);
    imgsensor.gain = reg_gain;
    spin_unlock(&imgsensor_drv_lock);
    LOG_INF("gain = %d , reg_gain = 0x%x\n ", gain, reg_gain);
	
	reg_gain = reg_gain & 0x00FF;
    write_cmos_sensor(0x0077,reg_gain);


	return gain;
}	/*	set_gain  */

static void ihdr_write_shutter_gain(kal_uint16 le, kal_uint16 se, kal_uint16 gain)
{
//#if 1
    LOG_INF("le:0x%x, se:0x%x, gain:0x%x\n",le,se,gain);
#if 0
    if (imgsensor.ihdr_en) {
        spin_lock(&imgsensor_drv_lock);
        if (le > imgsensor.min_frame_length - imgsensor_info.margin)
            imgsensor.frame_length = le + imgsensor_info.margin;
        else
            imgsensor.frame_length = imgsensor.min_frame_length;
        if (imgsensor.frame_length > imgsensor_info.max_frame_length)
            imgsensor.frame_length = imgsensor_info.max_frame_length;
        spin_unlock(&imgsensor_drv_lock);
        if (le < imgsensor_info.min_shutter) le = imgsensor_info.min_shutter;
        if (se < imgsensor_info.min_shutter) se = imgsensor_info.min_shutter;
        // Extend frame length first
        write_cmos_sensor(0x0006, imgsensor.frame_length);
        write_cmos_sensor(0x3502, (le << 4) & 0xFF);
        write_cmos_sensor(0x3501, (le >> 4) & 0xFF);
        write_cmos_sensor(0x3500, (le >> 12) & 0x0F);
        write_cmos_sensor(0x3508, (se << 4) & 0xFF);
        write_cmos_sensor(0x3507, (se >> 4) & 0xFF);
        write_cmos_sensor(0x3506, (se >> 12) & 0x0F);
        set_gain(gain);
    }
#endif

	return;
}

/*************************************************************************
* FUNCTION
*	night_mode
*
* DESCRIPTION
*	This function night mode of sensor.
*
* PARAMETERS
*	bEnable: KAL_TRUE -> enable night mode, otherwise, disable night mode
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static void night_mode(kal_bool enable)
{
/*No Need to implement this function*/ 
}	/*	night_mode	*/


/*************************************************************************
* FUNCTION
*	Initial, preview, capture(normal_video), hs_video, slim_video
*
* DESCRIPTION
*	Normal write setting
*
* RETURNS
*	None
*
*************************************************************************/

void hi1333_init_setting_normal(void)
{
////////////////////////////////
//---------TG firmware--------//
////////////////////////////////

// Firmware tg_pt
write_cmos_sensor(0x2000, 0x4031);
write_cmos_sensor(0x2002, 0x8400);
write_cmos_sensor(0x2004, 0x4349);
write_cmos_sensor(0x2006, 0x43b2);
write_cmos_sensor(0x2008, 0x8170);
write_cmos_sensor(0x200a, 0x43b2);
write_cmos_sensor(0x200c, 0x8172);
write_cmos_sensor(0x200e, 0x43b2);
write_cmos_sensor(0x2010, 0x8174);
write_cmos_sensor(0x2012, 0x1292);
write_cmos_sensor(0x2014, 0xd80a);
write_cmos_sensor(0x2016, 0xb2a2);
write_cmos_sensor(0x2018, 0x0a84);
write_cmos_sensor(0x201a, 0x2406);
write_cmos_sensor(0x201c, 0xb0f2);
write_cmos_sensor(0x201e, 0x0020);
write_cmos_sensor(0x2020, 0x0480);
write_cmos_sensor(0x2022, 0x2402);
write_cmos_sensor(0x2024, 0x12b0);
write_cmos_sensor(0x2026, 0xf3f6);
write_cmos_sensor(0x2028, 0x0900);
write_cmos_sensor(0x202a, 0x7314);
write_cmos_sensor(0x202c, 0x1292);
write_cmos_sensor(0x202e, 0xd80c);
write_cmos_sensor(0x2030, 0x1292);
write_cmos_sensor(0x2032, 0xd80e);
write_cmos_sensor(0x2034, 0x1292);
write_cmos_sensor(0x2036, 0xd810);
write_cmos_sensor(0x2038, 0x1292);
write_cmos_sensor(0x203a, 0xd812);
write_cmos_sensor(0x203c, 0x1292);
write_cmos_sensor(0x203e, 0xd814);
write_cmos_sensor(0x2040, 0x93c2);
write_cmos_sensor(0x2042, 0x0a98);
write_cmos_sensor(0x2044, 0x340d);
write_cmos_sensor(0x2046, 0x403f);
write_cmos_sensor(0x2048, 0x023c);
write_cmos_sensor(0x204a, 0x438f);
write_cmos_sensor(0x204c, 0x5000);
write_cmos_sensor(0x204e, 0x438f);
write_cmos_sensor(0x2050, 0x6000);
write_cmos_sensor(0x2052, 0x532f);
write_cmos_sensor(0x2054, 0x903f);
write_cmos_sensor(0x2056, 0x02e4);
write_cmos_sensor(0x2058, 0x2bf8);
write_cmos_sensor(0x205a, 0xf0f2);
write_cmos_sensor(0x205c, 0x007f);
write_cmos_sensor(0x205e, 0x0a98);
write_cmos_sensor(0x2060, 0x4392);
write_cmos_sensor(0x2062, 0x732a);
write_cmos_sensor(0x2064, 0x0c0f);
write_cmos_sensor(0x2066, 0x1292);
write_cmos_sensor(0x2068, 0xd826);
write_cmos_sensor(0x206a, 0x1292);
write_cmos_sensor(0x206c, 0xd81a);
write_cmos_sensor(0x206e, 0x1292);
write_cmos_sensor(0x2070, 0xd81c);
write_cmos_sensor(0x2072, 0x1292);
write_cmos_sensor(0x2074, 0xd81e);
write_cmos_sensor(0x2076, 0x43c2);
write_cmos_sensor(0x2078, 0x009c);
write_cmos_sensor(0x207a, 0x1292);
write_cmos_sensor(0x207c, 0xd816);
write_cmos_sensor(0x207e, 0xb3e2);
write_cmos_sensor(0x2080, 0x00c3);
write_cmos_sensor(0x2082, 0x2518);
write_cmos_sensor(0x2084, 0x40b2);
write_cmos_sensor(0x2086, 0x001f);
write_cmos_sensor(0x2088, 0x7322);
write_cmos_sensor(0x208a, 0xb3d2);
write_cmos_sensor(0x208c, 0x0480);
write_cmos_sensor(0x208e, 0x2407);
write_cmos_sensor(0x2090, 0x1292);
write_cmos_sensor(0x2092, 0xd83e);
write_cmos_sensor(0x2094, 0xb2f2);
write_cmos_sensor(0x2096, 0x0482);
write_cmos_sensor(0x2098, 0x2108);
write_cmos_sensor(0x209a, 0x1292);
write_cmos_sensor(0x209c, 0xd840);
write_cmos_sensor(0x209e, 0x4392);
write_cmos_sensor(0x20a0, 0x7320);
write_cmos_sensor(0x20a2, 0x93c2);
write_cmos_sensor(0x20a4, 0x809e);
write_cmos_sensor(0x20a6, 0x2010);
write_cmos_sensor(0x20a8, 0x93c2);
write_cmos_sensor(0x20aa, 0x00d0);
write_cmos_sensor(0x20ac, 0x3403);
write_cmos_sensor(0x20ae, 0x4382);
write_cmos_sensor(0x20b0, 0x0b8a);
write_cmos_sensor(0x20b2, 0x0c0a);
write_cmos_sensor(0x20b4, 0x0900);
write_cmos_sensor(0x20b6, 0x730a);
write_cmos_sensor(0x20b8, 0x1292);
write_cmos_sensor(0x20ba, 0xd826);
write_cmos_sensor(0x20bc, 0x1292);
write_cmos_sensor(0x20be, 0xd81a);
write_cmos_sensor(0x20c0, 0x1292);
write_cmos_sensor(0x20c2, 0xd81c);
write_cmos_sensor(0x20c4, 0x1292);
write_cmos_sensor(0x20c6, 0xd81e);
write_cmos_sensor(0x20c8, 0x4258);
write_cmos_sensor(0x20ca, 0x07a3);
write_cmos_sensor(0x20cc, 0x4346);
write_cmos_sensor(0x20ce, 0xb3d2);
write_cmos_sensor(0x20d0, 0x0480);
write_cmos_sensor(0x20d2, 0x240e);
write_cmos_sensor(0x20d4, 0x93c2);
write_cmos_sensor(0x20d6, 0x809c);
write_cmos_sensor(0x20d8, 0x240b);
write_cmos_sensor(0x20da, 0x425f);
write_cmos_sensor(0x20dc, 0x0a8e);
write_cmos_sensor(0x20de, 0xc312);
write_cmos_sensor(0x20e0, 0x104f);
write_cmos_sensor(0x20e2, 0x114f);
write_cmos_sensor(0x20e4, 0x114f);
write_cmos_sensor(0x20e6, 0x114f);
write_cmos_sensor(0x20e8, 0x907f);
write_cmos_sensor(0x20ea, 0x0003);
write_cmos_sensor(0x20ec, 0x2c01);
write_cmos_sensor(0x20ee, 0x4356);
write_cmos_sensor(0x20f0, 0x4347);
write_cmos_sensor(0x20f2, 0x9346);
write_cmos_sensor(0x20f4, 0x2404);
write_cmos_sensor(0x20f6, 0x9349);
write_cmos_sensor(0x20f8, 0x2402);
write_cmos_sensor(0x20fa, 0x1292);
write_cmos_sensor(0x20fc, 0xd842);
write_cmos_sensor(0x20fe, 0xb2a2);
write_cmos_sensor(0x2100, 0x0a84);
write_cmos_sensor(0x2102, 0x2406);
write_cmos_sensor(0x2104, 0xb0f2);
write_cmos_sensor(0x2106, 0x0020);
write_cmos_sensor(0x2108, 0x0480);
write_cmos_sensor(0x210a, 0x2402);
write_cmos_sensor(0x210c, 0x12b0);
write_cmos_sensor(0x210e, 0xf30c);
write_cmos_sensor(0x2110, 0x1292);
write_cmos_sensor(0x2112, 0xd822);
write_cmos_sensor(0x2114, 0x1292);
write_cmos_sensor(0x2116, 0xd820);
write_cmos_sensor(0x2118, 0xb3d2);
write_cmos_sensor(0x211a, 0x00bf);
write_cmos_sensor(0x211c, 0x2002);
write_cmos_sensor(0x211e, 0x43c2);
write_cmos_sensor(0x2120, 0x009c);
write_cmos_sensor(0x2122, 0x1292);
write_cmos_sensor(0x2124, 0xd844);
write_cmos_sensor(0x2126, 0x4382);
write_cmos_sensor(0x2128, 0x7114);
write_cmos_sensor(0x212a, 0x43c2);
write_cmos_sensor(0x212c, 0x80a5);
write_cmos_sensor(0x212e, 0xb3e2);
write_cmos_sensor(0x2130, 0x00c3);
write_cmos_sensor(0x2132, 0x434e);
write_cmos_sensor(0x2134, 0x634e);
write_cmos_sensor(0x2136, 0xb3d2);
write_cmos_sensor(0x2138, 0x00bf);
write_cmos_sensor(0x213a, 0x2004);
write_cmos_sensor(0x213c, 0xb35e);
write_cmos_sensor(0x213e, 0x2402);
write_cmos_sensor(0x2140, 0x43d2);
write_cmos_sensor(0x2142, 0x009c);
write_cmos_sensor(0x2144, 0x4392);
write_cmos_sensor(0x2146, 0x7f0c);
write_cmos_sensor(0x2148, 0x4392);
write_cmos_sensor(0x214a, 0x7f10);
write_cmos_sensor(0x214c, 0x4392);
write_cmos_sensor(0x214e, 0x770a);
write_cmos_sensor(0x2150, 0x4392);
write_cmos_sensor(0x2152, 0x770e);
write_cmos_sensor(0x2154, 0x9392);
write_cmos_sensor(0x2156, 0x7114);
write_cmos_sensor(0x2158, 0x2009);
write_cmos_sensor(0x215a, 0x1292);
write_cmos_sensor(0x215c, 0xd82c);
write_cmos_sensor(0x215e, 0x0900);
write_cmos_sensor(0x2160, 0x7112);
write_cmos_sensor(0x2162, 0x1292);
write_cmos_sensor(0x2164, 0xd818);
write_cmos_sensor(0x2166, 0x1292);
write_cmos_sensor(0x2168, 0xd83a);
write_cmos_sensor(0x216a, 0x3ff4);
write_cmos_sensor(0x216c, 0x0b00);
write_cmos_sensor(0x216e, 0x7304);
write_cmos_sensor(0x2170, 0x0000);
write_cmos_sensor(0x2172, 0xb357);
write_cmos_sensor(0x2174, 0x247f);
write_cmos_sensor(0x2176, 0x403e);
write_cmos_sensor(0x2178, 0x0f84);
write_cmos_sensor(0x217a, 0xc2ee);
write_cmos_sensor(0x217c, 0x0000);
write_cmos_sensor(0x217e, 0x4a4f);
write_cmos_sensor(0x2180, 0x5f0f);
write_cmos_sensor(0x2182, 0x503f);
write_cmos_sensor(0x2184, 0x809a);
write_cmos_sensor(0x2186, 0x4f9b);
write_cmos_sensor(0x2188, 0x000c);
write_cmos_sensor(0x218a, 0x0000);
write_cmos_sensor(0x218c, 0x4f9b);
write_cmos_sensor(0x218e, 0x000e);
write_cmos_sensor(0x2190, 0x0002);
write_cmos_sensor(0x2192, 0x4f9b);
write_cmos_sensor(0x2194, 0x0010);
write_cmos_sensor(0x2196, 0x0004);
write_cmos_sensor(0x2198, 0x4f9b);
write_cmos_sensor(0x219a, 0x0012);
write_cmos_sensor(0x219c, 0x0006);
write_cmos_sensor(0x219e, 0x4f9b);
write_cmos_sensor(0x21a0, 0x0014);
write_cmos_sensor(0x21a2, 0x0008);
write_cmos_sensor(0x21a4, 0x4f9b);
write_cmos_sensor(0x21a6, 0x0016);
write_cmos_sensor(0x21a8, 0x000a);
write_cmos_sensor(0x21aa, 0x4f9b);
write_cmos_sensor(0x21ac, 0x0018);
write_cmos_sensor(0x21ae, 0x000c);
write_cmos_sensor(0x21b0, 0xd2ee);
write_cmos_sensor(0x21b2, 0x0000);
write_cmos_sensor(0x21b4, 0x403f);
write_cmos_sensor(0x21b6, 0x8158);
write_cmos_sensor(0x21b8, 0x429f);
write_cmos_sensor(0x21ba, 0x7100);
write_cmos_sensor(0x21bc, 0x0000);
write_cmos_sensor(0x21be, 0x4f2f);
write_cmos_sensor(0x21c0, 0x903f);
write_cmos_sensor(0x21c2, 0x0028);
write_cmos_sensor(0x21c4, 0x2812);
write_cmos_sensor(0x21c6, 0x4f0e);
write_cmos_sensor(0x21c8, 0x503e);
write_cmos_sensor(0x21ca, 0xffd8);
write_cmos_sensor(0x21cc, 0x4e82);
write_cmos_sensor(0x21ce, 0x7a04);
write_cmos_sensor(0x21d0, 0xb368);
write_cmos_sensor(0x21d2, 0x2439);
write_cmos_sensor(0x21d4, 0x4e0f);
write_cmos_sensor(0x21d6, 0xe33f);
write_cmos_sensor(0x21d8, 0xf32f);
write_cmos_sensor(0x21da, 0xc32e);
write_cmos_sensor(0x21dc, 0xdf0e);
write_cmos_sensor(0x21de, 0x4e82);
write_cmos_sensor(0x21e0, 0x7a06);
write_cmos_sensor(0x21e2, 0x4392);
write_cmos_sensor(0x21e4, 0x7a0a);
write_cmos_sensor(0x21e6, 0x0800);
write_cmos_sensor(0x21e8, 0x7a0a);
write_cmos_sensor(0x21ea, 0xb357);
write_cmos_sensor(0x21ec, 0x240c);
write_cmos_sensor(0x21ee, 0x5359);
write_cmos_sensor(0x21f0, 0x503b);
write_cmos_sensor(0x21f2, 0xf000);
write_cmos_sensor(0x21f4, 0x4947);
write_cmos_sensor(0x21f6, 0xf357);
write_cmos_sensor(0x21f8, 0x1292);
write_cmos_sensor(0x21fa, 0xd83a);
write_cmos_sensor(0x21fc, 0x930f);
write_cmos_sensor(0x21fe, 0x27aa);
write_cmos_sensor(0x2200, 0x43c2);
write_cmos_sensor(0x2202, 0x809e);
write_cmos_sensor(0x2204, 0x3f4e);
write_cmos_sensor(0x2206, 0x9346);
write_cmos_sensor(0x2208, 0x27f7);
write_cmos_sensor(0x220a, 0x421f);
write_cmos_sensor(0x220c, 0x8158);
write_cmos_sensor(0x220e, 0x921f);
write_cmos_sensor(0x2210, 0x8172);
write_cmos_sensor(0x2212, 0x2414);
write_cmos_sensor(0x2214, 0x921f);
write_cmos_sensor(0x2216, 0x8174);
write_cmos_sensor(0x2218, 0x240a);
write_cmos_sensor(0x221a, 0x921f);
write_cmos_sensor(0x221c, 0x8170);
write_cmos_sensor(0x221e, 0x23ec);
write_cmos_sensor(0x2220, 0x4367);
write_cmos_sensor(0x2222, 0x434a);
write_cmos_sensor(0x2224, 0x4075);
write_cmos_sensor(0x2226, 0x001c);
write_cmos_sensor(0x2228, 0x403b);
write_cmos_sensor(0x222a, 0x022e);
write_cmos_sensor(0x222c, 0x3fe5);
write_cmos_sensor(0x222e, 0x4357);
write_cmos_sensor(0x2230, 0x407a);
write_cmos_sensor(0x2232, 0x000e);
write_cmos_sensor(0x2234, 0x403b);
write_cmos_sensor(0x2236, 0x624a);
write_cmos_sensor(0x2238, 0x4349);
write_cmos_sensor(0x223a, 0x3fde);
write_cmos_sensor(0x223c, 0x4357);
write_cmos_sensor(0x223e, 0x434a);
write_cmos_sensor(0x2240, 0x403b);
write_cmos_sensor(0x2242, 0x629e);
write_cmos_sensor(0x2244, 0x3ff9);
write_cmos_sensor(0x2246, 0xb358);
write_cmos_sensor(0x2248, 0x2408);
write_cmos_sensor(0x224a, 0x4e0f);
write_cmos_sensor(0x224c, 0xe33f);
write_cmos_sensor(0x224e, 0xf22f);
write_cmos_sensor(0x2250, 0xc22e);
write_cmos_sensor(0x2252, 0xdf0e);
write_cmos_sensor(0x2254, 0x4e82);
write_cmos_sensor(0x2256, 0x7a0e);
write_cmos_sensor(0x2258, 0x3fc4);
write_cmos_sensor(0x225a, 0x9078);
write_cmos_sensor(0x225c, 0x0003);
write_cmos_sensor(0x225e, 0x23c1);
write_cmos_sensor(0x2260, 0x4e0f);
write_cmos_sensor(0x2262, 0xe33f);
write_cmos_sensor(0x2264, 0xf03f);
write_cmos_sensor(0x2266, 0x0006);
write_cmos_sensor(0x2268, 0xf03e);
write_cmos_sensor(0x226a, 0xfff9);
write_cmos_sensor(0x226c, 0xdf0e);
write_cmos_sensor(0x226e, 0x4e82);
write_cmos_sensor(0x2270, 0x7a10);
write_cmos_sensor(0x2272, 0x3fb7);
write_cmos_sensor(0x2274, 0xb367);
write_cmos_sensor(0x2276, 0x279e);
write_cmos_sensor(0x2278, 0x954a);
write_cmos_sensor(0x227a, 0x2c15);
write_cmos_sensor(0x227c, 0x907a);
write_cmos_sensor(0x227e, 0x000e);
write_cmos_sensor(0x2280, 0x240f);
write_cmos_sensor(0x2282, 0x403f);
write_cmos_sensor(0x2284, 0x0f84);
write_cmos_sensor(0x2286, 0xc2ef);
write_cmos_sensor(0x2288, 0x0000);
write_cmos_sensor(0x228a, 0x4b9b);
write_cmos_sensor(0x228c, 0x600e);
write_cmos_sensor(0x228e, 0x6000);
write_cmos_sensor(0x2290, 0x4b9b);
write_cmos_sensor(0x2292, 0x500e);
write_cmos_sensor(0x2294, 0x5000);
write_cmos_sensor(0x2296, 0xd2ef);
write_cmos_sensor(0x2298, 0x0000);
write_cmos_sensor(0x229a, 0x532b);
write_cmos_sensor(0x229c, 0x535a);
write_cmos_sensor(0x229e, 0x3f8a);
write_cmos_sensor(0x22a0, 0x403b);
write_cmos_sensor(0x22a2, 0x0282);
write_cmos_sensor(0x22a4, 0x3fee);
write_cmos_sensor(0x22a6, 0x4347);
write_cmos_sensor(0x22a8, 0x3f85);
write_cmos_sensor(0x22aa, 0xb3e2);
write_cmos_sensor(0x22ac, 0x008e);
write_cmos_sensor(0x22ae, 0x26f7);
write_cmos_sensor(0x22b0, 0x4359);
write_cmos_sensor(0x22b2, 0x3ef5);
write_cmos_sensor(0x22b4, 0x42b2);
write_cmos_sensor(0x22b6, 0x7322);
write_cmos_sensor(0x22b8, 0x3ee8);
write_cmos_sensor(0x22ba, 0x4030);
write_cmos_sensor(0x22bc, 0xf000);
write_cmos_sensor(0x22be, 0x930d);
write_cmos_sensor(0x22c0, 0x2405);
write_cmos_sensor(0x22c2, 0x4ebf);
write_cmos_sensor(0x22c4, 0x0000);
write_cmos_sensor(0x22c6, 0x532f);
write_cmos_sensor(0x22c8, 0x533d);
write_cmos_sensor(0x22ca, 0x23fb);
write_cmos_sensor(0x22cc, 0x4130);
write_cmos_sensor(0x22ce, 0x4292);
write_cmos_sensor(0x22d0, 0x049a);
write_cmos_sensor(0x22d2, 0x0484);
write_cmos_sensor(0x22d4, 0x4292);
write_cmos_sensor(0x22d6, 0x049c);
write_cmos_sensor(0x22d8, 0x0486);
write_cmos_sensor(0x22da, 0x4292);
write_cmos_sensor(0x22dc, 0x049e);
write_cmos_sensor(0x22de, 0x0496);
write_cmos_sensor(0x22e0, 0x4292);
write_cmos_sensor(0x22e2, 0x04a0);
write_cmos_sensor(0x22e4, 0x0498);
write_cmos_sensor(0x22e6, 0x4292);
write_cmos_sensor(0x22e8, 0x05b4);
write_cmos_sensor(0x22ea, 0x048a);
write_cmos_sensor(0x22ec, 0x4292);
write_cmos_sensor(0x22ee, 0x05b6);
write_cmos_sensor(0x22f0, 0x048c);
write_cmos_sensor(0x22f2, 0x4292);
write_cmos_sensor(0x22f4, 0x05b8);
write_cmos_sensor(0x22f6, 0x0490);
write_cmos_sensor(0x22f8, 0x4292);
write_cmos_sensor(0x22fa, 0x83b2);
write_cmos_sensor(0x22fc, 0x0492);
write_cmos_sensor(0x22fe, 0x4292);
write_cmos_sensor(0x2300, 0x05be);
write_cmos_sensor(0x2302, 0x048e);
write_cmos_sensor(0x2304, 0x4292);
write_cmos_sensor(0x2306, 0x05c0);
write_cmos_sensor(0x2308, 0x0494);
write_cmos_sensor(0x230a, 0x4130);
write_cmos_sensor(0x230c, 0x120b);
write_cmos_sensor(0x230e, 0x120a);
write_cmos_sensor(0x2310, 0x403c);
write_cmos_sensor(0x2312, 0x0063);
write_cmos_sensor(0x2314, 0x93c2);
write_cmos_sensor(0x2316, 0x809e);
write_cmos_sensor(0x2318, 0x241e);
write_cmos_sensor(0x231a, 0x4292);
write_cmos_sensor(0x231c, 0x0484);
write_cmos_sensor(0x231e, 0x049a);
write_cmos_sensor(0x2320, 0x4292);
write_cmos_sensor(0x2322, 0x0486);
write_cmos_sensor(0x2324, 0x049c);
write_cmos_sensor(0x2326, 0x4292);
write_cmos_sensor(0x2328, 0x0496);
write_cmos_sensor(0x232a, 0x049e);
write_cmos_sensor(0x232c, 0x4292);
write_cmos_sensor(0x232e, 0x0498);
write_cmos_sensor(0x2330, 0x04a0);
write_cmos_sensor(0x2332, 0x4292);
write_cmos_sensor(0x2334, 0x048a);
write_cmos_sensor(0x2336, 0x05b4);
write_cmos_sensor(0x2338, 0x4292);
write_cmos_sensor(0x233a, 0x048c);
write_cmos_sensor(0x233c, 0x05b6);
write_cmos_sensor(0x233e, 0x4292);
write_cmos_sensor(0x2340, 0x0490);
write_cmos_sensor(0x2342, 0x05b8);
write_cmos_sensor(0x2344, 0x4292);
write_cmos_sensor(0x2346, 0x0492);
write_cmos_sensor(0x2348, 0x83b2);
write_cmos_sensor(0x234a, 0x4292);
write_cmos_sensor(0x234c, 0x048e);
write_cmos_sensor(0x234e, 0x05be);
write_cmos_sensor(0x2350, 0x4292);
write_cmos_sensor(0x2352, 0x0494);
write_cmos_sensor(0x2354, 0x05c0);
write_cmos_sensor(0x2356, 0x425f);
write_cmos_sensor(0x2358, 0x0aff);
write_cmos_sensor(0x235a, 0x421d);
write_cmos_sensor(0x235c, 0x0c96);
write_cmos_sensor(0x235e, 0xf37f);
write_cmos_sensor(0x2360, 0x930f);
write_cmos_sensor(0x2362, 0x2404);
write_cmos_sensor(0x2364, 0xc312);
write_cmos_sensor(0x2366, 0x100d);
write_cmos_sensor(0x2368, 0x831f);
write_cmos_sensor(0x236a, 0x23fc);
write_cmos_sensor(0x236c, 0x425f);
write_cmos_sensor(0x236e, 0x0a9a);
write_cmos_sensor(0x2370, 0xf37f);
write_cmos_sensor(0x2372, 0x9d0f);
write_cmos_sensor(0x2374, 0x2810);
write_cmos_sensor(0x2376, 0x425f);
write_cmos_sensor(0x2378, 0x0a9a);
write_cmos_sensor(0x237a, 0x4f4e);
write_cmos_sensor(0x237c, 0x8d0e);
write_cmos_sensor(0x237e, 0x425f);
write_cmos_sensor(0x2380, 0x0a9b);
write_cmos_sensor(0x2382, 0xf37f);
write_cmos_sensor(0x2384, 0x4e0c);
write_cmos_sensor(0x2386, 0x4f0a);
write_cmos_sensor(0x2388, 0x12b0);
write_cmos_sensor(0x238a, 0xefb6);
write_cmos_sensor(0x238c, 0x903c);
write_cmos_sensor(0x238e, 0x0006);
write_cmos_sensor(0x2390, 0x2805);
write_cmos_sensor(0x2392, 0x403c);
write_cmos_sensor(0x2394, 0x0005);
write_cmos_sensor(0x2396, 0x903c);
write_cmos_sensor(0x2398, 0x0006);
write_cmos_sensor(0x239a, 0x2c25);
write_cmos_sensor(0x239c, 0x4c0f);
write_cmos_sensor(0x239e, 0x5f0f);
write_cmos_sensor(0x23a0, 0x5f0f);
write_cmos_sensor(0x23a2, 0x4f0e);
write_cmos_sensor(0x23a4, 0x503e);
write_cmos_sensor(0x23a6, 0xf42c);
write_cmos_sensor(0x23a8, 0x4ea2);
write_cmos_sensor(0x23aa, 0x0484);
write_cmos_sensor(0x23ac, 0x4e92);
write_cmos_sensor(0x23ae, 0x0002);
write_cmos_sensor(0x23b0, 0x0486);
write_cmos_sensor(0x23b2, 0x503f);
write_cmos_sensor(0x23b4, 0xf414);
write_cmos_sensor(0x23b6, 0x4fa2);
write_cmos_sensor(0x23b8, 0x0496);
write_cmos_sensor(0x23ba, 0x4f92);
write_cmos_sensor(0x23bc, 0x0002);
write_cmos_sensor(0x23be, 0x0498);
write_cmos_sensor(0x23c0, 0x4c0e);
write_cmos_sensor(0x23c2, 0x5e0e);
write_cmos_sensor(0x23c4, 0x5e0e);
write_cmos_sensor(0x23c6, 0x5e0e);
write_cmos_sensor(0x23c8, 0x4c0f);
write_cmos_sensor(0x23ca, 0x5f0f);
write_cmos_sensor(0x23cc, 0x5f0f);
write_cmos_sensor(0x23ce, 0x5f0e);
write_cmos_sensor(0x23d0, 0x503e);
write_cmos_sensor(0x23d2, 0xf444);
write_cmos_sensor(0x23d4, 0x403d);
write_cmos_sensor(0x23d6, 0x0006);
write_cmos_sensor(0x23d8, 0x403f);
write_cmos_sensor(0x23da, 0x048a);
write_cmos_sensor(0x23dc, 0x12b0);
write_cmos_sensor(0x23de, 0xf2be);
write_cmos_sensor(0x23e0, 0xc3d2);
write_cmos_sensor(0x23e2, 0x0480);
write_cmos_sensor(0x23e4, 0x3c05);
write_cmos_sensor(0x23e6, 0x12b0);
write_cmos_sensor(0x23e8, 0xf2ce);
write_cmos_sensor(0x23ea, 0xd2d2);
write_cmos_sensor(0x23ec, 0x83b4);
write_cmos_sensor(0x23ee, 0x0480);
write_cmos_sensor(0x23f0, 0x413a);
write_cmos_sensor(0x23f2, 0x413b);
write_cmos_sensor(0x23f4, 0x4130);
write_cmos_sensor(0x23f6, 0xb2f2);
write_cmos_sensor(0x23f8, 0x0481);
write_cmos_sensor(0x23fa, 0x2403);
write_cmos_sensor(0x23fc, 0x12b0);
write_cmos_sensor(0x23fe, 0xf2ce);
write_cmos_sensor(0x2400, 0x3c05);
write_cmos_sensor(0x2402, 0x425f);
write_cmos_sensor(0x2404, 0x0480);
write_cmos_sensor(0x2406, 0xf35f);
write_cmos_sensor(0x2408, 0x4fc2);
write_cmos_sensor(0x240a, 0x83b4);
write_cmos_sensor(0x240c, 0xd2f2);
write_cmos_sensor(0x240e, 0x0481);
write_cmos_sensor(0x2410, 0x4130);
write_cmos_sensor(0x2412, 0xffff);
write_cmos_sensor(0x2414, 0x0023);
write_cmos_sensor(0x2416, 0x001a);
write_cmos_sensor(0x2418, 0x0046);
write_cmos_sensor(0x241a, 0x0034);
write_cmos_sensor(0x241c, 0x0069);
write_cmos_sensor(0x241e, 0x004e);
write_cmos_sensor(0x2420, 0x00d2);
write_cmos_sensor(0x2422, 0x009c);
write_cmos_sensor(0x2424, 0x013b);
write_cmos_sensor(0x2426, 0x00ea);
write_cmos_sensor(0x2428, 0x01a4);
write_cmos_sensor(0x242a, 0x0138);
write_cmos_sensor(0x242c, 0x0164);
write_cmos_sensor(0x242e, 0x013c);
write_cmos_sensor(0x2430, 0x016c);
write_cmos_sensor(0x2432, 0x0142);
write_cmos_sensor(0x2434, 0x0170);
write_cmos_sensor(0x2436, 0x0148);
write_cmos_sensor(0x2438, 0x0188);
write_cmos_sensor(0x243a, 0x015a);
write_cmos_sensor(0x243c, 0x019c);
write_cmos_sensor(0x243e, 0x016e);
write_cmos_sensor(0x2440, 0x01b8);
write_cmos_sensor(0x2442, 0x0186);
write_cmos_sensor(0x2444, 0x0109);
write_cmos_sensor(0x2446, 0x0212);
write_cmos_sensor(0x2448, 0xb817);
write_cmos_sensor(0x244a, 0x0054);
write_cmos_sensor(0x244c, 0x00a8);
write_cmos_sensor(0x244e, 0x67b2);
write_cmos_sensor(0x2450, 0x00fd);
write_cmos_sensor(0x2452, 0x01fa);
write_cmos_sensor(0x2454, 0xb40b);
write_cmos_sensor(0x2456, 0x0051);
write_cmos_sensor(0x2458, 0x00a2);
write_cmos_sensor(0x245a, 0x65c4);
write_cmos_sensor(0x245c, 0x00f8);
write_cmos_sensor(0x245e, 0x01f0);
write_cmos_sensor(0x2460, 0xb216);
write_cmos_sensor(0x2462, 0x004e);
write_cmos_sensor(0x2464, 0x009c);
write_cmos_sensor(0x2466, 0x63e7);
write_cmos_sensor(0x2468, 0x00da);
write_cmos_sensor(0x246a, 0x01b5);
write_cmos_sensor(0x246c, 0xa72f);
write_cmos_sensor(0x246e, 0x0046);
write_cmos_sensor(0x2470, 0x008c);
write_cmos_sensor(0x2472, 0x5eb5);
write_cmos_sensor(0x2474, 0x00c6);
write_cmos_sensor(0x2476, 0x018b);
write_cmos_sensor(0x2478, 0x9f11);
write_cmos_sensor(0x247a, 0x003f);
write_cmos_sensor(0x247c, 0x007d);
write_cmos_sensor(0x247e, 0x5988);
write_cmos_sensor(0x2480, 0x00ad);
write_cmos_sensor(0x2482, 0x015b);
write_cmos_sensor(0x2484, 0x94f2);
write_cmos_sensor(0x2486, 0x0037);
write_cmos_sensor(0x2488, 0x006e);
write_cmos_sensor(0x248a, 0x5405);
write_cmos_sensor(0x2ffe, 0xd800);
//    EOFIRM
//--------------------------------------------------------------------
// end of software code
//--------------------------------------------------------------------
//----- Initial -----//
write_cmos_sensor(0x0e04, 0x0013); //rom dly
write_cmos_sensor(0x3048, 0x5030); // 0x5020); //sreg09
write_cmos_sensor(0x0f30, 0x001f); //pll_cfg_ramp0
write_cmos_sensor(0x0f36, 0x001f); //pll_cfg_mipi0
write_cmos_sensor(0x0c00, 0x11d8); //blc_ctrl0 - b4:dpc_en b0:blc_en, blc_ctrl1 - b7:bwi b4:act_ofs b0:tobp_ofs
write_cmos_sensor(0x0c02, 0x0011); //blc_ctrl2 - b0:hdr_en, blc_ctrl3 - b4:dither_en b0:channel_blc
write_cmos_sensor(0x0c04, 0x5000); //blc_ctrl4 - b7:obp bypass, blc_ctrl5
write_cmos_sensor(0x0c06, 0x01eb); //blc_dig_offset
write_cmos_sensor(0x0c10, 0x0040); //act_rr_offset
write_cmos_sensor(0x0c12, 0x0040); //act_gr_offset
write_cmos_sensor(0x0c14, 0x0040); //act_gb_offset
write_cmos_sensor(0x0c16, 0x0040); //act_bb_offset
write_cmos_sensor(0x0c18, 0x8000); //fobp_iir_ctrl0
//write_cmos_sensor(0x0c62, 0x0194); //dark_pedestal_r
//write_cmos_sensor(0x0c64, 0x0286); //dark_pedestal_gr
//write_cmos_sensor(0x0c66, 0x0294); //dark_pedestal_gb
//write_cmos_sensor(0x0c68, 0x0100); //dark_pedestal_b
write_cmos_sensor(0x0c62, 0x82ff); //dark_pedestal_r
write_cmos_sensor(0x0c64, 0x8286); //dark_pedestal_gr
write_cmos_sensor(0x0c66, 0x82ff); //dark_pedestal_gb
write_cmos_sensor(0x0c68, 0x8100); //dark_pedestal_b
write_cmos_sensor(0x0cb2, 0x0200); //black_level_reference
write_cmos_sensor(0x0714, 0xe8e8); //black_level_ratio_r_gr
write_cmos_sensor(0x0716, 0xede8); //black_level_ratio_gb_b
write_cmos_sensor(0x000e, 0x0100); //image orient
write_cmos_sensor(0x0a10, 0x400c); //data pedestal on
write_cmos_sensor(0x003e, 0x0000); //test mode sel - off       
write_cmos_sensor(0x0074, 0x0d19); //coarse integ time
write_cmos_sensor(0x0a04, 0x03ea); //isp enable
write_cmos_sensor(0x0076, 0x0000); //analog gain
write_cmos_sensor(0x0724, 0x0f1f); //extra static
write_cmos_sensor(0x0068, 0x0703); //ci margin
write_cmos_sensor(0x0060, 0x0001); //customer code - MTK Validation
write_cmos_sensor(0x0062, 0x0500); //RAM F/W version
write_cmos_sensor(0x075e, 0x0535); //LSC para otp address
write_cmos_sensor(0x0012, 0x0fcd); //PDLSC para otp address
write_cmos_sensor(0x0806, 0x0002); //fmt y start 
write_cmos_sensor(0x0900, 0x0300); //mipi_tx_op_en, mipi_tx_test_mode
write_cmos_sensor(0x0902, 0xc319); //mipi_tx_op_mode1, mipi_tx_op_mode2
write_cmos_sensor(0x095a, 0x0099); //mipi_static0
write_cmos_sensor(0x095c, 0x1111); //mipi_static1
write_cmos_sensor(0x095e, 0xb8c0); //0xaac0); //mipi_static2
write_cmos_sensor(0x0960, 0x5fae); //0x5d2e); //mipi_static3
write_cmos_sensor(0x0a38, 0x080c); //lpd00 x/y
write_cmos_sensor(0x0a3a, 0x140c); //lpd01 x/y
write_cmos_sensor(0x0a3c, 0x280c); //lpd02 x/y
write_cmos_sensor(0x0a3e, 0x340c); //lpd03 x/y
write_cmos_sensor(0x0a40, 0x0418); //lpd04 x/y
write_cmos_sensor(0x0a42, 0x1818); //lpd05 x/y
write_cmos_sensor(0x0a44, 0x2418); //lpd06 x/y
write_cmos_sensor(0x0a46, 0x3818); //lpd07 x/y
write_cmos_sensor(0x0a48, 0x082c); //lpd08 x/y
write_cmos_sensor(0x0a4a, 0x142c); //lpd09 x/y
write_cmos_sensor(0x0a4c, 0x282c); //lpd10 x/y
write_cmos_sensor(0x0a4e, 0x342c); //lpd11 x/y
write_cmos_sensor(0x0a50, 0x0438); //lpd12 x/y
write_cmos_sensor(0x0a52, 0x1838); //lpd13 x/y
write_cmos_sensor(0x0a54, 0x2438); //lpd14 x/y
write_cmos_sensor(0x0a56, 0x3838); //lpd15 x/y
write_cmos_sensor(0x0a58, 0x0808); //rgt00 x/y
write_cmos_sensor(0x0a5a, 0x1408); //rgt01 x/y
write_cmos_sensor(0x0a5c, 0x2808); //rgt02 x/y
write_cmos_sensor(0x0a5e, 0x3408); //rgt03 x/y
write_cmos_sensor(0x0a60, 0x041c); //rgt04 x/y
write_cmos_sensor(0x0a62, 0x181c); //rgt05 x/y
write_cmos_sensor(0x0a64, 0x241c); //rgt06 x/y
write_cmos_sensor(0x0a66, 0x381c); //rgt07 x/y
write_cmos_sensor(0x0a68, 0x0828); //rgt08 x/y
write_cmos_sensor(0x0a6a, 0x1428); //rgt09 x/y
write_cmos_sensor(0x0a6c, 0x2828); //rgt10 x/y
write_cmos_sensor(0x0a6e, 0x3428); //rgt11 x/y
write_cmos_sensor(0x0a70, 0x043c); //rgt12 x/y
write_cmos_sensor(0x0a72, 0x183c); //rgt13 x/y
write_cmos_sensor(0x0a74, 0x243c); //rgt14 x/y
write_cmos_sensor(0x0a76, 0x383c); //rgt15 x/y
write_cmos_sensor(0x0404, 0x015c); //blk_width
write_cmos_sensor(0x0406, 0x0138); //blk_height
write_cmos_sensor(0x040a, 0x0115); //inv_col02
write_cmos_sensor(0x040c, 0x022a); //inv_col01
write_cmos_sensor(0x040e, 0xbc52); //inv_col35
write_cmos_sensor(0x0410, 0x0056); //inv_row02
write_cmos_sensor(0x0412, 0x00ac); //inv_row1
write_cmos_sensor(0x0414, 0x6907); //inv_row35
write_cmos_sensor(0x0422, 0x0011); //inv_pd_col02
write_cmos_sensor(0x0424, 0x0023); //inv_pd_col01
write_cmos_sensor(0x0426, 0x2f15); //inv_pd_col35
write_cmos_sensor(0x0428, 0x0016); //inv_pd_row02
write_cmos_sensor(0x042a, 0x002b); //inv_pd_row1
write_cmos_sensor(0x042c, 0x3483); //inv_pd_row35
write_cmos_sensor(0x0a1a, 0x1b02); //start, gap
write_cmos_sensor(0x0c04, 0x5091); //blc_ctrl4
write_cmos_sensor(0x0c72, 0x016e); //act_win_xstart
write_cmos_sensor(0x0c76, 0x0e56); //act_win_xend
write_cmos_sensor(0x0324, 0x0100); //ADPC col_boarder on
//ite_cmos_sensor(0x0600, 0x0D3e); //pdpc_ctl0 - PDAF TYPE-3 : Dyn-ON  & PD-OFF  //TYPE3 //pd dpc off
write_cmos_sensor(0x0600, 0x013e); //pdpc_ctl0 - PDAF TYPE-3 : Dyn-ON  & PD-OFF  //TYPE3  //pd dpc on
write_cmos_sensor(0x0a78, 0x0400); //Not ROUND_ACC_initialization
}

void hi1333_preview_setting_normal(void)
{
	
write_cmos_sensor(0x0a00, 0x0000);  //stream off

/*
DISP_WIDTH = 2104
DISP_HEIGHT = 1560
frame rate = 30fps
*/
write_cmos_sensor(0x0f38, 0x037d); //pll_cfg_mipi1
write_cmos_sensor(0x0f3a, 0x4107); //pll_cfg_mipi2 b10-8:001 mipi 1/2
write_cmos_sensor(0x093e, 0x0100); //mipi_tx_col_read_ctrl
write_cmos_sensor(0x0920, 0xc103); //mipi_exit_seq, tlpx
write_cmos_sensor(0x0922, 0x030d); //tclk_prepare, tclk_zero
write_cmos_sensor(0x0924, 0x0204); //tclk_pre, ths_prepare
write_cmos_sensor(0x0926, 0x0606); //ths_zero, ths_trail
write_cmos_sensor(0x0928, 0x0704); //tclk_post, tclk_trail
write_cmos_sensor(0x092a, 0x0505); //texit, tsync
write_cmos_sensor(0x092c, 0x0a00); //tpd_sync
write_cmos_sensor(0x0910, 0x0199); //mipi_vblank_delay
write_cmos_sensor(0x0918, 0x0194); //mipi_pd_vblank_delay
write_cmos_sensor(0x0912, 0x0062); //mipi_hblank_delay
write_cmos_sensor(0x0914, 0x000a); //mipi_hblank_short_delay1
write_cmos_sensor(0x0916, 0x000a); //mipi_hblank_short_delay2
write_cmos_sensor(0x091a, 0x0005); //mipi_pd_hblank_delay
write_cmos_sensor(0x0938, 0x0000); //0x4100); //mipi_virtual_channel_ctrl
write_cmos_sensor(0x0904, 0x2bab); //mipi_data_id_ctrl, mipi_pd_data_id_ctrl
write_cmos_sensor(0x0942, 0x0008); //0x0108); //mipi_pd_sep_ctrl1, mipi_pd_sep_ctrl2
write_cmos_sensor(0x0944, 0x0100); //mipi_pd_col_size
write_cmos_sensor(0x0946, 0x0004); //mipi_pd_row_size
write_cmos_sensor(0x0a2a, 0x8060); //PDAF patch x/y num sub2
write_cmos_sensor(0x0a2c, 0x2020); //PDAF patch x/y size sub2
write_cmos_sensor(0x0a32, 0x0301); //PDAF cnt012 sub2
write_cmos_sensor(0x0a26, 0x0048); //PDAF x patch offset sub2
write_cmos_sensor(0x0a28, 0x001c); //PDAF y patch offset sub2
write_cmos_sensor(0x0a36, 0x0000); //PDAF win loc sx/sy sub2
write_cmos_sensor(0x0408, 0x0000); //lsc spare
write_cmos_sensor(0x0418, 0x0000); //lsc win_h
write_cmos_sensor(0x0c74, 0x0091); //act_win_ystart
write_cmos_sensor(0x0c78, 0x0588); //act_win_yend
write_cmos_sensor(0x0a7e, 0x0004); //lsc shift
write_cmos_sensor(0x0800, 0x0400); //fmt ctrl
write_cmos_sensor(0x0008, 0x02e8); //line length pck
write_cmos_sensor(0x000c, 0x000c); //colgen start
write_cmos_sensor(0x0804, 0x0008); //fmt x cropping
write_cmos_sensor(0x0026, 0x003c); //y addr start active
write_cmos_sensor(0x002c, 0x0c71); //y addr end active
write_cmos_sensor(0x005c, 0x0204); //y dummy size
write_cmos_sensor(0x002e, 0x1111); //y even/odd inc tobp
write_cmos_sensor(0x0032, 0x3311); //y even/odd inc active
write_cmos_sensor(0x0006, 0x0d20); //frame length lines
write_cmos_sensor(0x0074, 0x0d19); //coarse integ time
write_cmos_sensor(0x0a04, 0x036a); //isp enable//20180205 0x03ea
write_cmos_sensor(0x0a0e, 0x0002); //image mode/digial binning mode
write_cmos_sensor(0x0a12, 0x0838); //x output size
write_cmos_sensor(0x0a14, 0x0618); //y output size
write_cmos_sensor(0x075c, 0x0100); //OTP ctrl b5:lsc_flag b4:lsc_checksum b3:pdlsc_en b2:dga_en b1:lsc_en b0:adpc_en 
write_cmos_sensor(0x0050, 0x4300); //analog control b7:vblank_analog_off b6:ag_cal b5:pd_rst2 b4:rdo_set_off b3:pat_addr_en b2:sub3 b1:sreg re-load b0:sreg write 
write_cmos_sensor(0x0722, 0x0700); //d2a_pxl_drv_pwr/d2a_row_binning_en
write_cmos_sensor(0x004c, 0x0100); //tg enable,hdr off

write_cmos_sensor(0x0a00, 0x0100);  // stream on
};


void hi1333_capture_setting_normal(void)
{
		/////////////////////////////////////////
// normal captrue
//  4208x3120 (30fps)
/////////////////////////////////////////
write_cmos_sensor(0x0a00, 0x0000);  //stream off

/*
	DISP_WIDTH = 4208
	DISP_HEIGHT = 3120
	DISP_NOTE = "//-----Normal 4208x3120 1.5Gbps-----//
	"
	BEGIN
	I2C_BYTE  = 0x22
	// frame rate = 30fps

*/
write_cmos_sensor(0x0f38, 0x037d); //pll_cfg_mipi1
write_cmos_sensor(0x0f3a, 0x4007); //pll_cfg_mipi2
write_cmos_sensor(0x093e, 0x0000); //mipi_tx_col_read_ctrl
write_cmos_sensor(0x0920, 0xc106); //mipi_exit_seq, tlpx
write_cmos_sensor(0x0922, 0x061a); //tclk_prepare, tclk_zero
write_cmos_sensor(0x0924, 0x0208); //tclk_pre, ths_prepare
write_cmos_sensor(0x0926, 0x0b09); //ths_zero, ths_trail
write_cmos_sensor(0x0928, 0x0c08); //tclk_post, tclk_trail
write_cmos_sensor(0x092a, 0x0a06); //texit, tsync
write_cmos_sensor(0x092c, 0x1600); //tpd_sync
write_cmos_sensor(0x0910, 0x034e); //mipi_vblank_delay
write_cmos_sensor(0x0918, 0x0349); //mipi_pd_vblank_delay
write_cmos_sensor(0x0912, 0x00dd); //mipi_hblank_delay
write_cmos_sensor(0x0914, 0x0040); //mipi_hblank_short_delay1
write_cmos_sensor(0x0916, 0x0040); //mipi_hblank_short_delay2
write_cmos_sensor(0x091a, 0x003b); //mipi_pd_hblank_delay
write_cmos_sensor(0x0938, 0x0000); //0x4100); //mipi_virtual_channel_ctrl
write_cmos_sensor(0x0904, 0x2bab); //mipi_data_id_ctrl, mipi_pd_data_id_ctrl
write_cmos_sensor(0x0942, 0x0008); //0x0108); //mipi_pd_sep_ctrl1, mipi_pd_sep_ctrl2
write_cmos_sensor(0x0944, 0x0100); //mipi_pd_col_size
write_cmos_sensor(0x0946, 0x0004); //mipi_pd_row_size
write_cmos_sensor(0x0a2a, 0x8060); //PDAF patch x/y num normal
write_cmos_sensor(0x0a2c, 0x2020); //PDAF patch x/y size normal
write_cmos_sensor(0x0a32, 0x0301); //PDAF cnt012 normal
write_cmos_sensor(0x0a26, 0x0048); //PDAF x patch offset normal
write_cmos_sensor(0x0a28, 0x001a); //PDAF y patch offset normal
write_cmos_sensor(0x0a36, 0x0000); //PDAF win loc sx/sy normal
write_cmos_sensor(0x0408, 0x0202); //lsc spare
write_cmos_sensor(0x0418, 0x0000); //lsc win_h
write_cmos_sensor(0x0c74, 0x00ed); //act_win_ystart
write_cmos_sensor(0x0c78, 0x0adb); //act_win_yend
write_cmos_sensor(0x0a7e, 0x0005); //lsc shift
write_cmos_sensor(0x0800, 0x0000); //fmt ctrl
write_cmos_sensor(0x0008, 0x02e8); //line length pck
write_cmos_sensor(0x000c, 0x000c); //colgen start
write_cmos_sensor(0x0804, 0x0010); //fmt x cropping
write_cmos_sensor(0x0026, 0x003e); //y addr start active
write_cmos_sensor(0x002c, 0x0c71); //y addr end active
write_cmos_sensor(0x005c, 0x0202); //y dummy size
write_cmos_sensor(0x002e, 0x1111); //y even/odd inc tobp
write_cmos_sensor(0x0032, 0x1111); //y even/odd inc active
write_cmos_sensor(0x0006, 0x0d20); //frame length lines
write_cmos_sensor(0x0074, 0x0d19); //coarse integ time
write_cmos_sensor(0x0a04, 0x03ea); //isp enable//20180205 0x03ea
write_cmos_sensor(0x0a0e, 0x0001); //image mode/digial binning mode
write_cmos_sensor(0x0a12, 0x1070); //x output size
write_cmos_sensor(0x0a14, 0x0c30); //y output size
write_cmos_sensor(0x075c, 0x0100); //OTP ctrl b5:lsc_flag b4:lsc_checksum b3:pdlsc_en b2:dga_en b1:lsc_en b0:adpc_en 
write_cmos_sensor(0x0050, 0x4300); //analog control b7:vblank_analog_off b6:ag_cal b5:pd_rst2 b4:rdo_set_off b3:pat_addr_en b2:sub3 b1:sreg re-load b0:sreg write 
write_cmos_sensor(0x0722, 0x0700); //d2a_pxl_drv_pwr/d2a_row_binning_en
write_cmos_sensor(0x004c, 0x0100); //tg enable,hdr off

write_cmos_sensor(0x0a00, 0x0100);  // stream on
}


void hi1333_capture_setting_pip(void)
{	
write_cmos_sensor(0x0a00, 0x0000);
write_cmos_sensor(0x0f38, 0x077d);  //pll_cfg_mipi1 b7-0: 4C mipi_mdiv
write_cmos_sensor(0x0f3a, 0x4007);  //pll_cfg_mipi2
write_cmos_sensor(0x093e, 0x0000);  //mipi_tx_col_read_ctrl
write_cmos_sensor(0x0920, 0xc103);  //mipi_exit_seq, tlpx
write_cmos_sensor(0x0922, 0x030d);  //tclk_prepare, tclk_zero
write_cmos_sensor(0x0924, 0x0204);  //tclk_pre, ths_prepare
write_cmos_sensor(0x0926, 0x0606);  //ths_zero, ths_trail
write_cmos_sensor(0x0928, 0x0704);  //tclk_post, tclk_trail
write_cmos_sensor(0x092a, 0x0505);  //texit, tsync
write_cmos_sensor(0x092c, 0x0a00);  //tpd_sync
write_cmos_sensor(0x0910, 0x036a);  //mipi_vblank_delay
write_cmos_sensor(0x0918, 0x0365);  //mipi_pd_vblank_delay
write_cmos_sensor(0x0912, 0x00eb);  //mipi_hblank_delay
write_cmos_sensor(0x0914, 0x004e);  //mipi_hblank_short_delay1
write_cmos_sensor(0x0916, 0x004e);  //mipi_hblank_short_delay2
write_cmos_sensor(0x091a, 0x004a);  //mipi_pd_hblank_delay
write_cmos_sensor(0x0938, 0x0000);  //0x4100);  //mipi_virtual_channel_ctrl
write_cmos_sensor(0x0904, 0x2bab);  //mipi_data_id_ctrl, mipi_pd_data_id_ctrl
write_cmos_sensor(0x0942, 0x0008);  //0x0108);  //mipi_pd_sep_ctrl1, mipi_pd_sep_ctrl2
write_cmos_sensor(0x0944, 0x0100);  //mipi_pd_col_size
write_cmos_sensor(0x0946, 0x0004);  //mipi_pd_row_size
write_cmos_sensor(0x0a2a, 0x8060);  //PDAF patch x/y num normal
write_cmos_sensor(0x0a2c, 0x2020);  //PDAF patch x/y size normal
write_cmos_sensor(0x0a32, 0x0301);  //PDAF cnt012 normal
write_cmos_sensor(0x0a26, 0x0048);  //PDAF x patch offset normal
write_cmos_sensor(0x0a28, 0x001a);  //PDAF y patch offset normal
write_cmos_sensor(0x0a36, 0x0000);  //PDAF win loc sx/sy normal
write_cmos_sensor(0x0408, 0x0202);  //lsc spare
write_cmos_sensor(0x0418, 0x0000);  //lsc win_h
write_cmos_sensor(0x0c74, 0x00ed);  //act_win_ystart
write_cmos_sensor(0x0c78, 0x0adb);  //act_win_yend
write_cmos_sensor(0x0a7e, 0x0005);  //lsc shift
write_cmos_sensor(0x0800, 0x0000);  //fmt ctrl
write_cmos_sensor(0x0008, 0x05d0);  //line length pck
write_cmos_sensor(0x000c, 0x000c);  //colgen start
write_cmos_sensor(0x0804, 0x0010);  //fmt x cropping
write_cmos_sensor(0x0026, 0x003e);  //y addr start active
write_cmos_sensor(0x002c, 0x0c71);  //y addr end active
write_cmos_sensor(0x005c, 0x0202);  //y dummy size
write_cmos_sensor(0x002e, 0x1111);  //y even/odd inc tobp
write_cmos_sensor(0x0032, 0x1111);  //y even/odd inc active
write_cmos_sensor(0x0006, 0x0d20);  //frame length lines
write_cmos_sensor(0x0074, 0x0d19);  //coarse integ time
write_cmos_sensor(0x0a04, 0x03ea); //isp enable//20180205 0x03ea
write_cmos_sensor(0x0a0e, 0x0001);  //image mode/digial binning mode
write_cmos_sensor(0x0a12, 0x1070);  //x output size
write_cmos_sensor(0x0a14, 0x0c30);  //y output size
write_cmos_sensor(0x075c, 0x0100);  //OTP ctrl b5:lsc_flag b4:lsc_checksum b3:pdlsc_en b2:dga_en b1:lsc_en b0:adpc_en 
write_cmos_sensor(0x0050, 0x4300);  //analog control b7:vblank_analog_off b6:ag_cal b5:pd_rst2 b4:rdo_set_off b3:pat_addr_en b2:sub3 b1:sreg re-load b0:sreg write 
write_cmos_sensor(0x0722, 0x0700);  //d2a_pxl_drv_pwr/d2a_row_binning_en
write_cmos_sensor(0x004c, 0x0100);  //tg enable,hdr off
write_cmos_sensor(0x0a00, 0x0100);
}

void hi1333_hs_video_setting_normal(void)
{
	write_cmos_sensor(0x0a00, 0x0000);
	// setting ver5.0 None PD 640x480 120fps
write_cmos_sensor(0x0f38, 0x037d); //pll_cfg_mipi1
write_cmos_sensor(0x0f3a, 0x4407); //pll_cfg_mipi2 b10-8:100 mipi 1/6 
write_cmos_sensor(0x093e, 0x0500); //mipi_tx_col_read_ctrl
write_cmos_sensor(0x0920, 0xc101); //mipi_exit_seq, tlpx
write_cmos_sensor(0x0922, 0x0105); //tclk_prepare, tclk_zero
write_cmos_sensor(0x0924, 0x0201); //tclk_pre, ths_prepare
write_cmos_sensor(0x0926, 0x0103); //ths_zero, ths_trail
write_cmos_sensor(0x0928, 0x0602); //tclk_post, tclk_trail
write_cmos_sensor(0x092a, 0x0203); //texit, tsync
write_cmos_sensor(0x092c, 0x0100); //tpd_sync
write_cmos_sensor(0x0910, 0x007b); //mipi_vblank_delay
write_cmos_sensor(0x0918, 0x0077); //mipi_pd_vblank_delay
write_cmos_sensor(0x0912, 0x001f); //mipi_hblank_delay
write_cmos_sensor(0x0942, 0x0008); //mipi_pd_sep_ctrl1, mipi_pd_sep_ctrl2 0x0108 -> PD / 0x0008 -> nonPD
write_cmos_sensor(0x0a2a, 0x4030); //PDAF patch x/y num vga
write_cmos_sensor(0x0a2c, 0x4040); //PDAF patch x/y size vga
write_cmos_sensor(0x0a32, 0x030c); //PDAF cnt012 vga
write_cmos_sensor(0x0a26, 0x0048); //PDAF x patch offset vga
write_cmos_sensor(0x0a28, 0x0000); //PDAF y patch offset vga
write_cmos_sensor(0x0a36, 0x0002); //PDAF win loc sx/sy vga
write_cmos_sensor(0x0408, 0x0200); //lsc spare
write_cmos_sensor(0x0418, 0x006c); //lsc win_h
write_cmos_sensor(0x0c74, 0x0052); //act_win_ystart
write_cmos_sensor(0x0c78, 0x018f); //act_win_yend
write_cmos_sensor(0x0a7e, 0x0002); //lsc shift
write_cmos_sensor(0x0800, 0x1400); //fmt ctrl
write_cmos_sensor(0x0008, 0x02e8); //line length pck
write_cmos_sensor(0x000c, 0x000c); //colgen start
write_cmos_sensor(0x0804, 0x0022); //fmt x cropping
write_cmos_sensor(0x0026, 0x00ac); //y addr start active
write_cmos_sensor(0x002c, 0x0bff); //y addr end active
write_cmos_sensor(0x005c, 0x040c); //y dummy size
write_cmos_sensor(0x002e, 0x3311); //y even/odd inc tobp
write_cmos_sensor(0x0032, 0x5577); //y even/odd inc active
write_cmos_sensor(0x0006, 0x0348); //frame length lines
write_cmos_sensor(0x0074, 0x0341); //coarse integ time
write_cmos_sensor(0x0a04, 0x036a); //isp enable//20180205 0x03ea
write_cmos_sensor(0x0a0e, 0x0006); //image mode/digial binning mode
write_cmos_sensor(0x0a12, 0x0280); //x output size
write_cmos_sensor(0x0a14, 0x01e0); //y output size
write_cmos_sensor(0x075c, 0x0100); //OTP ctrl b5:lsc_flag b4:lsc_checksum b3:pdlsc_en b2:dga_en b1:lsc_en b0:adpc_en 
write_cmos_sensor(0x0050, 0x4720); //analog control b7:vblank_analog_off b6:ag_cal b5:pd_rst2 b4:rdo_set_off b3:pat_addr_en b2:sub3 b1:sreg re-load b0:sreg write 
write_cmos_sensor(0x0722, 0x0702); //d2a_pxl_drv_pwr/d2a_row_binning_en - on
write_cmos_sensor(0x004c, 0x0100); //tg enable,hdr off

#if 0
	write_cmos_sensor(0x0f38, 0x037d); //pll_cfg_mipi1
	write_cmos_sensor(0x0f3a, 0x4407); //pll_cfg_mipi2 b10-8:100 mipi 1/6 
	write_cmos_sensor(0x093e, 0x0500); //mipi_tx_col_read_ctrl
	write_cmos_sensor(0x0920, 0xc101); //mipi_exit_seq, tlpx
	write_cmos_sensor(0x0922, 0x0105); //tclk_prepare, tclk_zero
	write_cmos_sensor(0x0924, 0x0201); //tclk_pre, ths_prepare
	write_cmos_sensor(0x0926, 0x020a); //ths_zero, ths_trail
	write_cmos_sensor(0x0928, 0x0202); //tclk_post, tclk_trail
	write_cmos_sensor(0x092a, 0x0203); //texit, tsync
	write_cmos_sensor(0x092c, 0x0100); //tpd_sync
	write_cmos_sensor(0x0910, 0x010f); //mipi_vblank_delay
	write_cmos_sensor(0x0912, 0x001f); //mipi_hblank_delay
	write_cmos_sensor(0x0a2a, 0x4030); //PDAF patch x/y num vga
	write_cmos_sensor(0x0a2c, 0x4040); //PDAF patch x/y size vga
	write_cmos_sensor(0x0a32, 0x030c); //PDAF cnt012 vga
	write_cmos_sensor(0x0a26, 0x0048); //PDAF x patch offset vga
	write_cmos_sensor(0x0a28, 0x0000); //PDAF y patch offset vga
	write_cmos_sensor(0x0a36, 0x0002); //PDAF win loc sx/sy vga
	write_cmos_sensor(0x0408, 0x0200); //lsc spare
	write_cmos_sensor(0x0418, 0x006c); //lsc win_h
	write_cmos_sensor(0x0c74, 0x0052); //act_win_ystart
	write_cmos_sensor(0x0c78, 0x018f); //act_win_yend
	write_cmos_sensor(0x0a7e, 0x0002); //lsc shift
	write_cmos_sensor(0x0800, 0x1400); //fmt ctrl
	write_cmos_sensor(0x0008, 0x02e8); //line length pck
	write_cmos_sensor(0x000c, 0x000c); //colgen start
	write_cmos_sensor(0x0804, 0x0022); //fmt x cropping
	write_cmos_sensor(0x0026, 0x00ac); //y addr start active
	write_cmos_sensor(0x002c, 0x0bff); //y addr end active
	write_cmos_sensor(0x005c, 0x040c); //y dummy size
	write_cmos_sensor(0x002e, 0x3311); //y even/odd inc tobp
	write_cmos_sensor(0x0032, 0x5577); //y even/odd inc active
	write_cmos_sensor(0x0006, 0x0348); //frame length lines
	write_cmos_sensor(0x0a0e, 0x0006); //image mode/digial binning mode
	write_cmos_sensor(0x0a12, 0x0280); //x output size
	write_cmos_sensor(0x0a14, 0x01e0); //y output size
	write_cmos_sensor(0x0050, 0x4720); //analog control b7:vblank_analog_off b6:ag_cal b5:pd_rst2 b4:rdo_set_off b3:pat_addr_en b2:sub3 b1:sreg re-load b0:sreg write 
	write_cmos_sensor(0x0722, 0x0702); //d2a_pxl_drv_pwr/d2a_row_binning_en - on
	write_cmos_sensor(0x004c, 0x0100); //tg enable,hdr off
#endif
	
	write_cmos_sensor(0x0a00, 0x0100);

}

void hi1333_slim_video_setting_normal(void)
{
//////////////////////////////////
//  HD 1280x720 120fps
//
//////////////////////////////////

write_cmos_sensor(0x0a00, 0x0000);
write_cmos_sensor(0x0f38, 0x037d);  //pll_cfg_mipi1
write_cmos_sensor(0x0f3a, 0x4207);  //pll_cfg_mipi2 b10-8:010 mipi 1/3
write_cmos_sensor(0x093e, 0x0200);  //mipi_tx_col_read_ctrl
write_cmos_sensor(0x0920, 0xc102);  //mipi_exit_seq, tlpx
write_cmos_sensor(0x0922, 0x0209);  //tclk_prepare, tclk_zero
write_cmos_sensor(0x0924, 0x0203);  //tclk_pre, ths_prepare
write_cmos_sensor(0x0926, 0x0304);  //ths_zero, ths_trail
write_cmos_sensor(0x0928, 0x0703);  //tclk_post, tclk_trail
write_cmos_sensor(0x092a, 0x0304);  //texit, tsync
write_cmos_sensor(0x092c, 0x0400);  //tpd_sync
write_cmos_sensor(0x0910, 0x0112);  //mipi_vblank_delay
write_cmos_sensor(0x0918, 0x010d);  //mipi_pd_vblank_delay
write_cmos_sensor(0x0912, 0x0057);  //mipi_hblank_delay
write_cmos_sensor(0x0938, 0x0000);  //0x4100);  //mipi_virtual_channel_ctrl

write_cmos_sensor(0x0942, 0x0008);  //0x0108);  //mipi_pd_sep_ctrl1, mipi_pd_sep_ctrl2
write_cmos_sensor(0x0a2a, 0x4030);  //PDAF patch x/y num hd
write_cmos_sensor(0x0a2c, 0x4040);  //PDAF patch x/y size hd
write_cmos_sensor(0x0a32, 0x1300);  //PDAF cnt012 hd
write_cmos_sensor(0x0a26, 0x0048);  //PDAF x patch offset hd
write_cmos_sensor(0x0a28, 0x0000);  //PDAF y patch offset hd
write_cmos_sensor(0x0a36, 0x0000);  //PDAF win loc sx/sy hd
write_cmos_sensor(0x0408, 0x0200);  //lsc spare
write_cmos_sensor(0x0418, 0x01a2);  //lsc win_h
write_cmos_sensor(0x0c74, 0x002b);  //act_win_ystart
write_cmos_sensor(0x0c78, 0x017a);  //act_win_yend
write_cmos_sensor(0x0a7e, 0x0003);  //lsc shift
write_cmos_sensor(0x0800, 0x0800);  //fmt ctrl
write_cmos_sensor(0x0008, 0x02e8);  //line length pck
write_cmos_sensor(0x000c, 0x000c);  //colgen start
write_cmos_sensor(0x0804, 0x0042);  //fmt x cropping
write_cmos_sensor(0x0026, 0x021a);  //y addr start active
write_cmos_sensor(0x002c, 0x0a93);  //y addr end active
write_cmos_sensor(0x005c, 0x0206);  //y dummy size
write_cmos_sensor(0x002e, 0x1111);  //y even/odd inc tobp
write_cmos_sensor(0x0032, 0x3333);  //y even/odd inc active
write_cmos_sensor(0x0006, 0x0348);  //frame length lines
write_cmos_sensor(0x0074, 0x0341);  //coarse integ time
write_cmos_sensor(0x0a04, 0x036a); //isp enable//20180205 0x03ea
write_cmos_sensor(0x0a0e, 0x0003);  //image mode/digial binning mode
write_cmos_sensor(0x0a12, 0x0500);  //x output size
write_cmos_sensor(0x0a14, 0x02d0);  //y output size
write_cmos_sensor(0x075c, 0x0100);  //OTP ctrl b5:lsc_flag b4:lsc_checksum b3:pdlsc_en b2:dga_en b1:lsc_en b0:adpc_en 
write_cmos_sensor(0x0050, 0x4720);  //analog control b7:vblank_analog_off b6:ag_cal b5:pd_rst2 b4:rdo_set_off b3:pat_addr_en b2:sub3 b1:sreg re-load b0:sreg write 
write_cmos_sensor(0x0722, 0x0700);  //d2a_pxl_drv_pwr/d2a_row_binning_en
write_cmos_sensor(0x004c, 0x0100);  //tg enable,hdr off

write_cmos_sensor(0x0a00, 0x0100);

}
extern char Back_Camera_Name[256];
extern char Back_Camera_OTP [256];
/*************************************************************************
* FUNCTION
*	get_imgsensor_id
*
* DESCRIPTION
*	This function get the sensor ID 
*
* PARAMETERS
*	*sensorID : return the sensor ID 
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 get_imgsensor_id(UINT32 *sensor_id) 
{
	kal_uint8 i = 0;
	kal_uint8 retry = 2;

	LOG_INF("E\n");

	while (imgsensor_info.i2c_addr_table[i] != 0xff) {
			spin_lock(&imgsensor_drv_lock);
			imgsensor.i2c_write_id = imgsensor_info.i2c_addr_table[i];
			spin_unlock(&imgsensor_drv_lock);
			do {
				*sensor_id = ((read_cmos_sensor(0x0F17) << 8) | read_cmos_sensor(0x0F16));
				
				if (*sensor_id == imgsensor_info.sensor_id) {				
					LOG_INF("i2c write id  : 0x%x, sensor id: 0x%x\n", imgsensor.i2c_write_id,*sensor_id);	  
					sprintf(Back_Camera_Name,"HI1333:BYD:13M_AF");
					sprintf(Back_Camera_OTP,"OTP:OK");
                    			return ERROR_NONE;
			}	
				LOG_INF("get_imgsensor_id Read sensor id fail, id: 0x%x 0x%x\n", imgsensor.i2c_write_id,*sensor_id);
				retry--;
			} while(retry > 0);
			i++;
			retry = 2;
}
    
	if (*sensor_id != imgsensor_info.sensor_id) {
		// if Sensor ID is not correct, Must set *sensor_id to 0xFFFFFFFF 
		*sensor_id = 0xFFFFFFFF;
		return ERROR_SENSOR_CONNECT_FAIL;
	}
	
	return ERROR_NONE;
}


/*************************************************************************
* FUNCTION
*	open
*
* DESCRIPTION
*	This function initialize the registers of CMOS sensor
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 open(void)
{
	kal_uint8 i = 0;
	kal_uint8 retry = 2;
	kal_uint16 sensor_id = 0; 
    LOG_INF("E\n");

	while (imgsensor_info.i2c_addr_table[i] != 0xff) {
		
		spin_lock(&imgsensor_drv_lock);
		imgsensor.i2c_write_id = imgsensor_info.i2c_addr_table[i];
		spin_unlock(&imgsensor_drv_lock);
		LOG_INF("SP\n");
		do {
			sensor_id = ((read_cmos_sensor(0x0F17) << 8) | read_cmos_sensor(0x0F16));
			if (sensor_id == imgsensor_info.sensor_id) {				
				LOG_INF("i2c write id: 0x%x, sensor id: 0x%x\n", imgsensor.i2c_write_id,sensor_id);	  

				break;
			}	
			LOG_INF("Read sensor id fail, write id:0x%x id: 0x%x\n", imgsensor.i2c_write_id,sensor_id);
			retry--;
		} while(retry > 0);
		i++;
		if (sensor_id == imgsensor_info.sensor_id)
			break;
		retry = 2;
	}	
    
	if (imgsensor_info.sensor_id != sensor_id)
		return ERROR_SENSOR_CONNECT_FAIL;



	/* initail sequence write in  */
	LOG_INF("hi1333_find sensor pass!! Start initial setting\n");
	hi1333_init_setting_normal();
	spin_lock(&imgsensor_drv_lock);

	imgsensor.autoflicker_en= KAL_FALSE;
	imgsensor.sensor_mode = IMGSENSOR_MODE_INIT;
	imgsensor.pclk = imgsensor_info.pre.pclk;
	imgsensor.frame_length = imgsensor_info.pre.framelength;
	imgsensor.line_length = imgsensor_info.pre.linelength;
	imgsensor.min_frame_length = imgsensor_info.pre.framelength;
	imgsensor.dummy_pixel = 0;
	imgsensor.dummy_line = 0;
	imgsensor.ihdr_en = 0;
   	imgsensor.test_pattern = KAL_FALSE;
	imgsensor.current_fps = imgsensor_info.pre.max_framerate;
	spin_unlock(&imgsensor_drv_lock);

	return ERROR_NONE;
}	/*	open  */



/*************************************************************************
* FUNCTION
*	close
*
* DESCRIPTION
*	
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 close(void)
{
    LOG_INF("E\n");

	/*No Need to implement this function*/ 
	
	return ERROR_NONE;
}	/*	close  */


/*************************************************************************
* FUNCTION
* preview
*
* DESCRIPTION
*	This function start the sensor preview.
*
* PARAMETERS
*	*image_window : address pointer of pixel numbers in one period of HSYNC
*  *sensor_config_data : address pointer of line numbers in one period of VSYNC
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/

static kal_uint32 preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
    LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_PREVIEW;
	imgsensor.pclk = imgsensor_info.pre.pclk;
	imgsensor.line_length = imgsensor_info.pre.linelength;
	imgsensor.frame_length = imgsensor_info.pre.framelength; 
	imgsensor.min_frame_length = imgsensor_info.pre.framelength;
	imgsensor.current_fps = imgsensor_info.pre.max_framerate;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);

	hi1333_preview_setting_normal();

	pip_mode =0;


	return ERROR_NONE;
}	/*	preview   */


static void capture_setting(kal_uint16 currentfps)
{
	LOG_INF(" capture, currefps = %d\n",currentfps);  

	LOG_INF("burst disable\n");

	if( currentfps < 300 )	hi1333_capture_setting_pip();
	else                    hi1333_capture_setting_normal();

}

/*************************************************************************
* FUNCTION
*	capture
*
* DESCRIPTION
*	This function setup the CMOS sensor in capture MY_OUTPUT mode
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
						  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
    LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_CAPTURE;

    if (imgsensor.current_fps == imgsensor_info.cap.max_framerate) // 30fps
    {
		imgsensor.pclk = imgsensor_info.cap.pclk;
		imgsensor.line_length = imgsensor_info.cap.linelength;
		imgsensor.frame_length = imgsensor_info.cap.framelength;  
		imgsensor.min_frame_length = imgsensor_info.cap.framelength;
		imgsensor.autoflicker_en = KAL_FALSE;        

		pip_mode = 0;
	}
	else //PIP capture: 15fps for 13M
    {		
		if (imgsensor.current_fps != imgsensor_info.cap1.max_framerate)
			LOG_INF("Warning: current_fps %d fps is not support, so use cap1's setting: %d fps!\n",imgsensor.current_fps, imgsensor_info.cap1.max_framerate/10);   
		imgsensor.pclk = imgsensor_info.cap1.pclk;
		imgsensor.line_length = imgsensor_info.cap1.linelength;
		imgsensor.frame_length = imgsensor_info.cap1.framelength;  
		imgsensor.min_frame_length = imgsensor_info.cap1.framelength;
		imgsensor.autoflicker_en = KAL_FALSE;
		
		pip_mode = 1;

	} 
	
	spin_unlock(&imgsensor_drv_lock);

	//hi1333_preview_setting_normal();
	capture_setting(imgsensor.current_fps); 

	return ERROR_NONE;
}	/* capture() */

static kal_uint32 normal_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E");
	LOG_INF("Normal_Video\n");
	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_VIDEO;
	imgsensor.pclk = imgsensor_info.normal_video.pclk;
	imgsensor.line_length = imgsensor_info.normal_video.linelength;
	imgsensor.frame_length = imgsensor_info.normal_video.framelength;  
	imgsensor.min_frame_length = imgsensor_info.normal_video.framelength;
	imgsensor.current_fps = 300;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	//normal_video_setting(imgsensor.current_fps);

	hi1333_capture_setting_normal();
    //hi1333_preview_setting_normal();

	pip_mode =0;
	return ERROR_NONE;
}	/*	normal_video   */

static kal_uint32 hs_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
                      MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
    LOG_INF("E\n");
    
	spin_lock(&imgsensor_drv_lock);
    imgsensor.sensor_mode = IMGSENSOR_MODE_HIGH_SPEED_VIDEO;
    imgsensor.pclk = imgsensor_info.hs_video.pclk;
    //imgsensor.video_mode = KAL_TRUE;
    imgsensor.line_length = imgsensor_info.hs_video.linelength;
    imgsensor.frame_length = imgsensor_info.hs_video.framelength;
    imgsensor.min_frame_length = imgsensor_info.hs_video.framelength;
    imgsensor.dummy_line = 0;
    imgsensor.dummy_pixel = 0;
    imgsensor.autoflicker_en = KAL_FALSE;
    spin_unlock(&imgsensor_drv_lock);

  	hi1333_hs_video_setting_normal();

	pip_mode =0;
    return ERROR_NONE;
}    /*    hs_video   */

static kal_uint32 slim_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
                      MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
    LOG_INF("E\n");

    spin_lock(&imgsensor_drv_lock);
    imgsensor.sensor_mode = IMGSENSOR_MODE_SLIM_VIDEO;
    imgsensor.pclk = imgsensor_info.slim_video.pclk;
    imgsensor.line_length = imgsensor_info.slim_video.linelength;
    imgsensor.frame_length = imgsensor_info.slim_video.framelength;
    imgsensor.min_frame_length = imgsensor_info.slim_video.framelength;
    imgsensor.dummy_line = 0;
    imgsensor.dummy_pixel = 0;
    imgsensor.autoflicker_en = KAL_FALSE;
    spin_unlock(&imgsensor_drv_lock);

	hi1333_slim_video_setting_normal();

	pip_mode =0;
    return ERROR_NONE;
}    /*    slim_video     */

static kal_uint32 get_resolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *sensor_resolution)
{
    LOG_INF("E\n");
    
    sensor_resolution->SensorFullWidth = imgsensor_info.cap.grabwindow_width;
    sensor_resolution->SensorFullHeight = imgsensor_info.cap.grabwindow_height;

    sensor_resolution->SensorPreviewWidth = imgsensor_info.pre.grabwindow_width;
    sensor_resolution->SensorPreviewHeight = imgsensor_info.pre.grabwindow_height;

    sensor_resolution->SensorVideoWidth = imgsensor_info.normal_video.grabwindow_width;
    sensor_resolution->SensorVideoHeight = imgsensor_info.normal_video.grabwindow_height;

    sensor_resolution->SensorHighSpeedVideoWidth     = imgsensor_info.hs_video.grabwindow_width;
    sensor_resolution->SensorHighSpeedVideoHeight     = imgsensor_info.hs_video.grabwindow_height;

    sensor_resolution->SensorSlimVideoWidth     = imgsensor_info.slim_video.grabwindow_width;
    sensor_resolution->SensorSlimVideoHeight     = imgsensor_info.slim_video.grabwindow_height;
    return ERROR_NONE;
}    /*    get_resolution    */


static kal_uint32 get_info(MSDK_SCENARIO_ID_ENUM scenario_id,
                      MSDK_SENSOR_INFO_STRUCT *sensor_info,
                      MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
    LOG_INF("scenario_id = %d\n", scenario_id);


    //sensor_info->SensorVideoFrameRate = imgsensor_info.normal_video.max_framerate/10; /* not use */
    //sensor_info->SensorStillCaptureFrameRate= imgsensor_info.cap.max_framerate/10; /* not use */
    //imgsensor_info->SensorWebCamCaptureFrameRate= imgsensor_info.v.max_framerate; /* not use */

    sensor_info->SensorClockPolarity = SENSOR_CLOCK_POLARITY_LOW;
    sensor_info->SensorClockFallingPolarity = SENSOR_CLOCK_POLARITY_LOW; /* not use */
    sensor_info->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW; // inverse with datasheet
    sensor_info->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
    sensor_info->SensorInterruptDelayLines = 4; /* not use */
    sensor_info->SensorResetActiveHigh = FALSE; /* not use */
    sensor_info->SensorResetDelayCount = 5; /* not use */

    sensor_info->SensroInterfaceType = imgsensor_info.sensor_interface_type;
    //sensor_info->MIPIsensorType = imgsensor_info.mipi_sensor_type;
    //sensor_info->SettleDelayMode = imgsensor_info.mipi_settle_delay_mode;
    sensor_info->SensorOutputDataFormat = imgsensor_info.sensor_output_dataformat;

    sensor_info->CaptureDelayFrame = imgsensor_info.cap_delay_frame;
    sensor_info->PreviewDelayFrame = imgsensor_info.pre_delay_frame;
    sensor_info->VideoDelayFrame = imgsensor_info.video_delay_frame;
    sensor_info->HighSpeedVideoDelayFrame = imgsensor_info.hs_video_delay_frame;
    sensor_info->SlimVideoDelayFrame = imgsensor_info.slim_video_delay_frame;

    sensor_info->SensorMasterClockSwitch = 0; /* not use */
    sensor_info->SensorDrivingCurrent = imgsensor_info.isp_driving_current;

    sensor_info->AEShutDelayFrame = imgsensor_info.ae_shut_delay_frame;          /* The frame of setting shutter default 0 for TG int */
    sensor_info->AESensorGainDelayFrame = imgsensor_info.ae_sensor_gain_delay_frame;    /* The frame of setting sensor gain */
    sensor_info->AEISPGainDelayFrame = imgsensor_info.ae_ispGain_delay_frame;
    sensor_info->IHDR_Support = imgsensor_info.ihdr_support;
    sensor_info->IHDR_LE_FirstLine = imgsensor_info.ihdr_le_firstline;
    sensor_info->SensorModeNum = imgsensor_info.sensor_mode_num;
    
	#ifdef ENABLE_PDAF
			sensor_info->PDAF_Support = 1;
	#else
		sensor_info->PDAF_Support = 0;
	#endif

    sensor_info->SensorMIPILaneNumber = imgsensor_info.mipi_lane_num;
    sensor_info->SensorClockFreq = imgsensor_info.mclk;
    sensor_info->SensorClockDividCount = 3; /* not use */
    sensor_info->SensorClockRisingCount = 0;
    sensor_info->SensorClockFallingCount = 2; /* not use */
    sensor_info->SensorPixelClockCount = 3; /* not use */
    sensor_info->SensorDataLatchCount = 2; /* not use */

    sensor_info->MIPIDataLowPwr2HighSpeedTermDelayCount = 0;
    sensor_info->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
    sensor_info->SensorWidthSampling = 0;  // 0 is default 1x
    sensor_info->SensorHightSampling = 0;    // 0 is default 1x
    sensor_info->SensorPacketECCOrder = 1;
	sensor_info->PDAF_Support = 1;


    switch (scenario_id) {
        case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
            sensor_info->SensorGrabStartX = imgsensor_info.pre.startx;
            sensor_info->SensorGrabStartY = imgsensor_info.pre.starty;
            sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.pre.mipi_data_lp2hs_settle_dc;

            break;
        case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
            sensor_info->SensorGrabStartX = imgsensor_info.cap.startx;
            sensor_info->SensorGrabStartY = imgsensor_info.cap.starty;
			sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.cap.mipi_data_lp2hs_settle_dc;
            break;
        case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
            sensor_info->SensorGrabStartX = imgsensor_info.normal_video.startx;
            sensor_info->SensorGrabStartY = imgsensor_info.normal_video.starty;
            sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.normal_video.mipi_data_lp2hs_settle_dc;
            break;
        case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
            sensor_info->SensorGrabStartX = imgsensor_info.hs_video.startx;
            sensor_info->SensorGrabStartY = imgsensor_info.hs_video.starty;
            sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.hs_video.mipi_data_lp2hs_settle_dc;

            break;
        case MSDK_SCENARIO_ID_SLIM_VIDEO:
            sensor_info->SensorGrabStartX = imgsensor_info.slim_video.startx;
            sensor_info->SensorGrabStartY = imgsensor_info.slim_video.starty;
            sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.slim_video.mipi_data_lp2hs_settle_dc;

            break;
        default:
            sensor_info->SensorGrabStartX = imgsensor_info.pre.startx;
            sensor_info->SensorGrabStartY = imgsensor_info.pre.starty;
            sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.pre.mipi_data_lp2hs_settle_dc;
            break;
    }

    return ERROR_NONE;
}    /*    get_info  */


static kal_uint32 control(MSDK_SCENARIO_ID_ENUM scenario_id, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("[contrlo]scenario_id = %d", scenario_id);
	spin_lock(&imgsensor_drv_lock);
	imgsensor.current_scenario_id = scenario_id;
	spin_unlock(&imgsensor_drv_lock);
	switch (scenario_id) {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			preview(image_window, sensor_config_data);
			break;	
       // case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
       //     capture(image_window, sensor_config_data);
       //     break;
		// 2016.09.09 Coby
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		case MSDK_SCENARIO_ID_CAMERA_ZSD:
			capture(image_window, sensor_config_data);
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			normal_video(image_window, sensor_config_data);
			break;	  
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
            hs_video(image_window, sensor_config_data);
            break;
        case MSDK_SCENARIO_ID_SLIM_VIDEO:
       		slim_video(image_window, sensor_config_data);
       		break;	  
		default:
			LOG_INF("Error ScenarioId setting");
			preview(image_window, sensor_config_data);
			return ERROR_INVALID_SCENARIO_ID;
	}
	return ERROR_NONE;
}	/* control() */



static kal_uint32 set_video_mode(UINT16 framerate)
{
	LOG_INF("framerate = %d ", framerate);
	// SetVideoMode Function should fix framerate
	if (framerate == 0)
		// Dynamic frame rate
		return ERROR_NONE;
	spin_lock(&imgsensor_drv_lock);
	
	if ((framerate == 30) && (imgsensor.autoflicker_en == KAL_TRUE))
		imgsensor.current_fps = 296;
	else if ((framerate == 15) && (imgsensor.autoflicker_en == KAL_TRUE))
		imgsensor.current_fps = 146;
	else
		imgsensor.current_fps = 10 * framerate;
	spin_unlock(&imgsensor_drv_lock);
	set_max_framerate(imgsensor.current_fps,1);

	return ERROR_NONE;
}

static kal_uint32 set_auto_flicker_mode(kal_bool enable, UINT16 framerate)
{
	LOG_INF("enable = %d, framerate = %d ", enable, framerate);
	spin_lock(&imgsensor_drv_lock);
	if (enable) 	  
		imgsensor.autoflicker_en = KAL_TRUE;
	else //Cancel Auto flick
		imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	return ERROR_NONE;
}


static kal_uint32 set_max_framerate_by_scenario(MSDK_SCENARIO_ID_ENUM scenario_id, MUINT32 framerate)
{
    kal_uint32 frame_length;

    LOG_INF("scenario_id = %d, framerate = %d\n", scenario_id, framerate);

    switch (scenario_id) {
        case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
            frame_length = imgsensor_info.pre.pclk / framerate * 10 / imgsensor_info.pre.linelength;
            spin_lock(&imgsensor_drv_lock);
            imgsensor.dummy_line = (frame_length > imgsensor_info.pre.framelength) ? (frame_length - imgsensor_info.pre.framelength) : 0;
            imgsensor.frame_length = imgsensor_info.pre.framelength + imgsensor.dummy_line;
            imgsensor.min_frame_length = imgsensor.frame_length;
            spin_unlock(&imgsensor_drv_lock);

			if( imgsensor.frame_length > imgsensor.shutter)
            	set_dummy();

            break;
        case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
            if(framerate == 0)
                return ERROR_NONE;

            frame_length = imgsensor_info.normal_video.pclk / framerate * 10 / imgsensor_info.normal_video.linelength;
            spin_lock(&imgsensor_drv_lock);
            imgsensor.dummy_line = (frame_length > imgsensor_info.normal_video.framelength) ? (frame_length - imgsensor_info.normal_video.framelength) : 0;
            imgsensor.frame_length = imgsensor_info.normal_video.framelength + imgsensor.dummy_line;
            imgsensor.min_frame_length = imgsensor.frame_length;
            spin_unlock(&imgsensor_drv_lock);

			if( imgsensor.frame_length > imgsensor.shutter)
            	set_dummy();

            break;
        case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
        	  if (imgsensor.current_fps == imgsensor_info.cap1.max_framerate) {
                frame_length = imgsensor_info.cap1.pclk / framerate * 10 / imgsensor_info.cap1.linelength;
                spin_lock(&imgsensor_drv_lock);
		            imgsensor.dummy_line = (frame_length > imgsensor_info.cap1.framelength) ? (frame_length - imgsensor_info.cap1.framelength) : 0;
					//imgsensor.dummy_line = 0;
		            imgsensor.frame_length = imgsensor_info.cap1.framelength + imgsensor.dummy_line;
		            imgsensor.min_frame_length = imgsensor.frame_length;
		            spin_unlock(&imgsensor_drv_lock);
            } else {
        		    if (imgsensor.current_fps != imgsensor_info.cap.max_framerate)
                    LOG_INF("Warning: current_fps %d fps is not support, so use cap's setting: %d fps!\n",framerate,imgsensor_info.cap.max_framerate/10);
                frame_length = imgsensor_info.cap.pclk / framerate * 10 / imgsensor_info.cap.linelength;
                spin_lock(&imgsensor_drv_lock);
		            imgsensor.dummy_line = (frame_length > imgsensor_info.cap.framelength) ? (frame_length - imgsensor_info.cap.framelength) : 0;
		            imgsensor.frame_length = imgsensor_info.cap.framelength + imgsensor.dummy_line;
		            imgsensor.min_frame_length = imgsensor.frame_length;
		            spin_unlock(&imgsensor_drv_lock);
            }

			if( imgsensor.frame_length > imgsensor.shutter)
            	set_dummy();

            break;
        case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
            frame_length = imgsensor_info.hs_video.pclk / framerate * 10 / imgsensor_info.hs_video.linelength;
            spin_lock(&imgsensor_drv_lock);
            imgsensor.dummy_line = (frame_length > imgsensor_info.hs_video.framelength) ? (frame_length - imgsensor_info.hs_video.framelength) : 0;
            imgsensor.frame_length = imgsensor_info.hs_video.framelength + imgsensor.dummy_line;
            imgsensor.min_frame_length = imgsensor.frame_length;
            spin_unlock(&imgsensor_drv_lock);

			if( imgsensor.frame_length > imgsensor.shutter)
            	set_dummy();

            break;
        case MSDK_SCENARIO_ID_SLIM_VIDEO:
            frame_length = imgsensor_info.slim_video.pclk / framerate * 10 / imgsensor_info.slim_video.linelength;
            spin_lock(&imgsensor_drv_lock);
            imgsensor.dummy_line = (frame_length > imgsensor_info.slim_video.framelength) ? (frame_length - imgsensor_info.slim_video.framelength): 0;
            imgsensor.frame_length = imgsensor_info.slim_video.framelength + imgsensor.dummy_line;
            imgsensor.min_frame_length = imgsensor.frame_length;
            spin_unlock(&imgsensor_drv_lock);

			if( imgsensor.frame_length > imgsensor.shutter)
            	set_dummy();

            break;
        default:  //coding with  preview scenario by default
            frame_length = imgsensor_info.pre.pclk / framerate * 10 / imgsensor_info.pre.linelength;
            spin_lock(&imgsensor_drv_lock);
            imgsensor.dummy_line = (frame_length > imgsensor_info.pre.framelength) ? (frame_length - imgsensor_info.pre.framelength) : 0;
            imgsensor.frame_length = imgsensor_info.pre.framelength + imgsensor.dummy_line;
            imgsensor.min_frame_length = imgsensor.frame_length;
            spin_unlock(&imgsensor_drv_lock);

			if( imgsensor.frame_length > imgsensor.shutter)
            	set_dummy();
            LOG_INF("error scenario_id = %d, we use preview scenario \n", scenario_id);
            break;
    }
    return ERROR_NONE;
}


static kal_uint32 get_default_framerate_by_scenario(MSDK_SCENARIO_ID_ENUM scenario_id, MUINT32 *framerate)
{
    LOG_INF("scenario_id = %d\n", scenario_id);

    switch (scenario_id) {
        case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
            *framerate = imgsensor_info.pre.max_framerate;
            break;
        case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
            *framerate = imgsensor_info.normal_video.max_framerate;
            break;
        case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
            *framerate = imgsensor_info.cap.max_framerate;
            break;
        case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
            *framerate = imgsensor_info.hs_video.max_framerate;
            break;
        case MSDK_SCENARIO_ID_SLIM_VIDEO:
            *framerate = imgsensor_info.slim_video.max_framerate;
            break;
        default:
            break;
    }

    return ERROR_NONE;
}

static kal_uint32 set_test_pattern_mode(kal_bool enable)
{
	UINT16 enable_TP = 0; 
	enable_TP = ((read_cmos_sensor(0x0A04) << 8) | read_cmos_sensor(0x0A05));

	LOG_INF("enable: %d", enable);
    // 0x0A05[0]: 1 enable,  0 disable               
    // 0x020A[1:0]; 0x01:BLACK, 0x02:COLOR BAR, 0x03:GREY, 0x04:RANDOM
    
	if (enable) {
//        enable_TP |= 0x0001;  
        write_cmos_sensor(0x0A04,0x0141);
		write_cmos_sensor(0x020A,0x0200);

	} else {
//        enable_TP &= 0xFFFE;  
//        LOG_INF("mook 0x0A04: 0x%x\n", enable_TP);
        write_cmos_sensor(0x0A04,0x03C1);
		write_cmos_sensor(0x020A,0x0000);
	}	 
	return ERROR_NONE;
}



static kal_uint32 feature_control(MSDK_SENSOR_FEATURE_ENUM feature_id,
                             UINT8 *feature_para,UINT32 *feature_para_len)
{
    UINT16 *feature_return_para_16=(UINT16 *) feature_para;
    UINT16 *feature_data_16=(UINT16 *) feature_para;
    UINT32 *feature_return_para_32=(UINT32 *) feature_para;
    UINT32 *feature_data_32=(UINT32 *) feature_para;
    SET_PD_BLOCK_INFO_T *PDAFinfo;	//KYM ADD 13M

    unsigned long long *feature_data=(unsigned long long *) feature_para;

    //unsigned long long *feature_return_para=(unsigned long long *) feature_para;

    SENSOR_WINSIZE_INFO_STRUCT *wininfo;
    MSDK_SENSOR_REG_INFO_STRUCT *sensor_reg_data=(MSDK_SENSOR_REG_INFO_STRUCT *) feature_para;

    LOG_INF("feature_id = %d\n", feature_id);
    switch (feature_id) {
        case SENSOR_FEATURE_GET_PERIOD:
            *feature_return_para_16++ = imgsensor.line_length;
            *feature_return_para_16 = imgsensor.frame_length;
            *feature_para_len=4;
            break;
        case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
            LOG_INF("feature_Control imgsensor.pclk = %d,imgsensor.current_fps = %d\n", imgsensor.pclk,imgsensor.current_fps);
            *feature_return_para_32 = imgsensor.pclk;
            *feature_para_len=4;
            break;
        case SENSOR_FEATURE_SET_ESHUTTER:
            set_shutter(*feature_data);
            break;
        case SENSOR_FEATURE_SET_NIGHTMODE:
            night_mode((BOOL) *feature_data);
            break;
        case SENSOR_FEATURE_SET_GAIN:
            set_gain((UINT16) *feature_data);
            break;
        case SENSOR_FEATURE_SET_FLASHLIGHT:
            break;
        case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
            break;
        case SENSOR_FEATURE_SET_REGISTER:
            write_cmos_sensor(sensor_reg_data->RegAddr, sensor_reg_data->RegData);
            break;
        case SENSOR_FEATURE_GET_REGISTER:
            sensor_reg_data->RegData = read_cmos_sensor(sensor_reg_data->RegAddr);
            break;
        case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
            // get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
            // if EEPROM does not exist in camera module.
            *feature_return_para_32=LENS_DRIVER_ID_DO_NOT_CARE;
            *feature_para_len=4;
            break;
        case SENSOR_FEATURE_SET_VIDEO_MODE:
            set_video_mode(*feature_data);
            break;
        case SENSOR_FEATURE_CHECK_SENSOR_ID:
            get_imgsensor_id(feature_return_para_32);
            break;
        case SENSOR_FEATURE_SET_AUTO_FLICKER_MODE:
            set_auto_flicker_mode((BOOL)*feature_data_16,*(feature_data_16+1));
            break;
        case SENSOR_FEATURE_SET_MAX_FRAME_RATE_BY_SCENARIO:
            set_max_framerate_by_scenario((MSDK_SCENARIO_ID_ENUM)*feature_data, *(feature_data+1));
            break;
        case SENSOR_FEATURE_GET_DEFAULT_FRAME_RATE_BY_SCENARIO:
            get_default_framerate_by_scenario((MSDK_SCENARIO_ID_ENUM)*(feature_data), (MUINT32 *)(uintptr_t)(*(feature_data+1)));
            break;
        case SENSOR_FEATURE_SET_TEST_PATTERN:
            set_test_pattern_mode((BOOL)*feature_data);
            break;
        case SENSOR_FEATURE_GET_TEST_PATTERN_CHECKSUM_VALUE: //for factory mode auto testing
            *feature_return_para_32 = imgsensor_info.checksum_value;
            *feature_para_len=4;
            break;
        case SENSOR_FEATURE_SET_FRAMERATE:
            LOG_INF("current fps :%d\n", (UINT32)*feature_data);
            spin_lock(&imgsensor_drv_lock);
            imgsensor.current_fps = *feature_data;
            spin_unlock(&imgsensor_drv_lock);
            break;
        case SENSOR_FEATURE_SET_HDR:
            LOG_INF("ihdr enable :%d\n", (BOOL)*feature_data);
            spin_lock(&imgsensor_drv_lock);
            imgsensor.ihdr_en = (BOOL)*feature_data;
            spin_unlock(&imgsensor_drv_lock);

            break;
        case SENSOR_FEATURE_GET_CROP_INFO:
            LOG_INF("SENSOR_FEATURE_GET_CROP_INFO scenarioId:%d\n", (UINT32)*feature_data);

            wininfo = (SENSOR_WINSIZE_INFO_STRUCT *)(uintptr_t)(*(feature_data+1));

            switch (*feature_data_32) {
                case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
                    memcpy((void *)wininfo,(void *)&imgsensor_winsize_info[1],sizeof(SENSOR_WINSIZE_INFO_STRUCT));
                    break;
                case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
                    memcpy((void *)wininfo,(void *)&imgsensor_winsize_info[2],sizeof(SENSOR_WINSIZE_INFO_STRUCT));
                    break;
                case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
                    memcpy((void *)wininfo,(void *)&imgsensor_winsize_info[3],sizeof(SENSOR_WINSIZE_INFO_STRUCT));
                    break;
                case MSDK_SCENARIO_ID_SLIM_VIDEO:
                    memcpy((void *)wininfo,(void *)&imgsensor_winsize_info[4],sizeof(SENSOR_WINSIZE_INFO_STRUCT));
                    break;
                case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
                default:
                    memcpy((void *)wininfo,(void *)&imgsensor_winsize_info[0],sizeof(SENSOR_WINSIZE_INFO_STRUCT));
                    break;
            }
	    break;
		
#if ENABLE_PDAF
		case SENSOR_FEATURE_GET_PDAF_DATA:
		LOG_INF("KYM_PDAF GET_PDAF_DATA EEPROM\n");
		// read from e2prom
#ifdef e2prom
		//read_eeprom((kal_uint16)(*feature_data), 
			//	(char *)(uintptr_t)(*(feature_data+1)), 
			//	(kal_uint32)(*(feature_data+2)) );
#else
		// read from file

	        LOG_INF("READ PDCAL DATA\n");
		//read_hi1333_eeprom((kal_uint16)(*feature_data), 
		//		(char *)(uintptr_t)(*(feature_data+1)), 
		//		(kal_uint32)(*(feature_data+2)) );

#endif
		break;

	case SENSOR_FEATURE_GET_PDAF_INFO:
		//LOG_INF("KYM_PDAF SENSOR_FEATURE_GET_PDAF_INFO scenarioId:%d\n", (UINT32)*feature_data);
		PDAFinfo= (SET_PD_BLOCK_INFO_T *)(uintptr_t)(*(feature_data+1)); // coby
		switch( *feature_data) 
		{
		 case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		 case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			LOG_INF("KYM GET_PDAF_DATA MSDK_SCENARIO_ID_VIDEO_PREVIEW\n");
			memcpy((void *)PDAFinfo, (void *)&imgsensor_pd_info, sizeof(SET_PD_BLOCK_INFO_T));
			break;
	 	 default:
			break;

		}
		break;


	case SENSOR_FEATURE_GET_SENSOR_PDAF_CAPACITY:
           	 //LOG_INF("KYM_PDAF SENSOR_FEATURE_GET_SENSOR_PDAF_CAPACITY scenarioId:%d\n", *feature_data);
           	 switch (*feature_data) 
	   	 {
           	     case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
           	          *(MUINT32 *)(uintptr_t)(*(feature_data+1)) = 1;
	   	         break;
           	     case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
	   	          LOG_INF("KYM_PDAF ID_VIDEO_PREVIEW\n");
           	          *(MUINT32 *)(uintptr_t)(*(feature_data+1)) = 0;
	   	          break;
           	     case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
           	          *(MUINT32 *)(uintptr_t)(*(feature_data+1)) = 0;
	   	           break;
           	     case MSDK_SCENARIO_ID_SLIM_VIDEO:
           	          *(MUINT32 *)(uintptr_t)(*(feature_data+1)) = 0;
           	         break;
           	     case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
	   	          LOG_INF("KYM_PDAF ID_CAMERA_PREVIEW\n");
	   	          *(MUINT32 *)(uintptr_t)(*(feature_data+1)) = 0;
           	         break;
           	     default:
           	          *(MUINT32 *)(uintptr_t)(*(feature_data+1)) = 0;
           	         break;
           	 }
	   	 break;
#endif
        case SENSOR_FEATURE_SET_IHDR_SHUTTER_GAIN:
            LOG_INF("SENSOR_SET_SENSOR_IHDR LE=%d, SE=%d, Gain=%d\n",(UINT16)*feature_data,(UINT16)*(feature_data+1),(UINT16)*(feature_data+2));
            ihdr_write_shutter_gain((UINT16)*feature_data,(UINT16)*(feature_data+1),(UINT16)*(feature_data+2));
            break;
        case SENSOR_FEATURE_SET_HDR_SHUTTER:
            LOG_INF("SENSOR_FEATURE_SET_HDR_SHUTTER LE=%d, SE=%d\n",(UINT16)*feature_data,(UINT16)*(feature_data+1));
            ihdr_write_shutter_gain((UINT16)*feature_data,(UINT16)*(feature_data+1),(UINT16)*(feature_data+2));
            break;
        default:
            break;
    }

    return ERROR_NONE;
}   /*  feature_control()  */



SENSOR_FUNCTION_STRUCT sensor_func = {
	open,
	get_info,
	get_resolution,
	feature_control, 
	control,
	close
};

UINT32 HI1333_MIPI_RAW_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&sensor_func;
	return ERROR_NONE;
}	/*	HI1333_MIPI_RAW_SensorInit	*/
