#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/atomic.h>
/*#include <asm/atomic.h>*/
/*#include <asm/system.h>*/
#include "kd_camera_typedef.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"

#include "gc8034mipi_Sensor.h"

#define LOG_TAG  "GC8034_OTP:"

#define SENSORDB_GC8034_OTP(fmt,arg...)  printk(LOG_TAG "%s %d: " fmt "\n", __FUNCTION__ ,__LINE__ ,##arg)
#if 0
struct gc8034_otp {
	kal_uint8  dd_cnt;
	kal_uint8  dd_flag;
	kal_uint8  reg_flag;
	kal_uint8  reg_num;
	kal_uint8  reg_page[10];
	kal_uint8  reg_addr[10];
	kal_uint8  reg_value[10];
#ifdef GC8034OTP_FOR_CUSTOMER
	kal_uint8  module_id;
	kal_uint8  lens_id;
	kal_uint8  vcm_id;
	kal_uint8  vcm_driver_id;
	kal_uint8  year;
	kal_uint8  month;
	kal_uint8  day;
	kal_uint8  af_flag;
	kal_uint16 af_infinity;
	kal_uint16 af_macro;
	kal_uint8  wb_flag;
	kal_uint16 rg_gain;
	kal_uint16 bg_gain;
	kal_uint8  golden_flag;
	kal_uint16 golden_rg;
	kal_uint16 golden_bg;
	kal_uint8  lsc_flag;		/* 0:Empty 1:Success 2:Invalid */
#endif
};
#endif
extern struct gc8034_otp gc8034_otp_info;

int gc8034_read_lsc(unsigned short *buffer,int size);
unsigned short gc8034_lsc_data[397];

int  gc8034_otp_flag = 0;
unsigned int gc8034otp_selective_read_region(struct i2c_client *client, unsigned int addr,unsigned char *data, unsigned int size);

EXPORT_SYMBOL(gc8034_lsc_data);
EXPORT_SYMBOL(gc8034otp_selective_read_region);


static int gc8034_selective_read_region(u32 offset, u8* data, u32 size)
{    
	int i  = 0, checksum = 0,count = 0;
	SENSORDB_GC8034_OTP("[gc8034_CAM_CAL] selective_read_region offset =%d size %d data read = %d\n", offset,size, *data);
	/*for(i=0; i< 9; i++)
		SENSORDB_GC8034_OTP("gc8034_otp_info.wb_data[%d] = 0x%x",i,gc8034_otp_info.wb_data[i]);
	for(i=0; i< 4; i++)
		SENSORDB_GC8034_OTP("gc8034_otp_info.af_data[%d] = 0x%x",i+1,gc8034_otp_info.af_data[i+1]);*/
	if(offset==0x0001)
		*data=0x16;
	else if(offset==0x0008)//for awb data
	{
		for(i=0; i< size; i++)
		{
			if(i<2)
				*data= gc8034_otp_info.rg_gain >> (i*8);
			else
				*data= gc8034_otp_info.bg_gain >> ((i%2)*8);
			data++;
		}
	}
	else if(offset==0x000c)
	{
		for(i=0; i< size; i++)
		{
			if(i<2)
				*data= gc8034_otp_info.golden_rg >> (i*8);
			else
				*data= gc8034_otp_info.golden_bg >> ((i%2)*8);
			data++;
		}
	}
	else if(offset==0x0010)//for af_inf data
	{
		for(i=0; i< size; i++)
		{
			*data= gc8034_otp_info.af_infinity >> (i*8);
			data++;
		}
	}
	else if(offset==0x0012)
	{
		for(i=0; i< size; i++)
		{
			*data= gc8034_otp_info.af_macro >> (i*8);
			data++;
		}
	}
	else if(offset==0x0014)//for lsc data
	{
		if(gc8034_otp_flag == 0)
		{
			for(count = 0; count < 5; count++)
			{
				gc8034_read_lsc(gc8034_lsc_data,397);
				checksum = 0;
				for(i=0; i< 396; i++)
				{
					checksum += gc8034_lsc_data[i];
					//SENSORDB_GC8034_OTP("hi846otp_lsc_data, 0x%x , ", hi846_lsc_data[i]);
				}
				checksum = checksum%0xff +1;
				SENSORDB_GC8034_OTP("count = %d, checksum = %d, gc8034_lsc_data[396] = %d",count,checksum,gc8034_lsc_data[396]);
				if(checksum == gc8034_lsc_data[396])
				{
					SENSORDB_GC8034_OTP("gc8034_lsc_data check success");
					gc8034_otp_flag = 1;
					break;
				}
				else{
					SENSORDB_GC8034_OTP("gc8034_lsc_data check failed, do it again");
					continue;
				}
			}
		}
		
		for(i=0; i< size; i++)
		{
			*data=gc8034_lsc_data[i];
			SENSORDB_GC8034_OTP("gc8034_lsc_data[%d] = %d, *data= %d",i,gc8034_lsc_data[i],*data);
			data++;
		}

	}
	return size;
	
}

unsigned int gc8034otp_selective_read_region(struct i2c_client *client, unsigned int addr,
	unsigned char *data, unsigned int size)
{
	SENSORDB_GC8034_OTP("addr 0x%x, size is 0x%x",addr,size);

	if (gc8034_selective_read_region(addr, data, size))
		return size;
	else
		return 0;
}

