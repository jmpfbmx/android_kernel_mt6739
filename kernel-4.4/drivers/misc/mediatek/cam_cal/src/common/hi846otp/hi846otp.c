

#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
//#include <asm/atomic.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "cam_cal.h"
#include "cam_cal_define.h"




#define RGr_ratio_Typical  617	//modify by v3702 
#define BGr_ratio_Typical  577  //modify by v3702
#define GbGr_ratio_Typical  1024

#define LOG_TAG  "HI846OTP:"

//#define SENSORDB_HI846_OTP(fmt,arg...) xlog_printk(ANDROID_LOG_DEBUG , "[S5K3H7_OTP]", fmt, ##arg)                             //printk(LOG_TAG "%s: " fmt "\n", __FUNCTION__ ,##arg)


#define SENSORDB_HI846_OTP(fmt,arg...)  printk(LOG_TAG "%s %d: " fmt "\n", __FUNCTION__ ,__LINE__ ,##arg)

#define CAM_CAL_I2C_BUSNUM 2
#define HI846_DEVICE_ID 0x40
#define CAM_CAL_DEV_MAJOR_NUMBER 226


/*******************************************************************************
*
********************************************************************************/
#define CAM_CAL_ICS_REVISION 1 //seanlin111208
//#define HI846OTP_SWITCH
/*******************************************************************************
*
********************************************************************************/
#define CAM_CAL_DRVNAME "CAM_CAL_DRV_HI846"
#define CAM_CAL_I2C_GROUP_ID 0

struct HI846_otp_struct 
{
	int Base_Info_Flag;	//bit[7]:info, bit[6]:wb
	int module_integrator_id;
	int prodyction_year;
	int production_month;
	int production_day;
	int sensor_id;
	int lens_id;
	int vcm_id;
	int Driver_ic_id;
	int F_num_id;
	int WB_FLAG;
	int wb_data[9];  //wb_data[8] is checksum
	int AF_FLAG;
	int af_data[5];
	int LSC_FLAG;
	//int lsc_data[1868];
	int infocheck;
	int wbcheck;
	int checksum;
	int wb_checksum;
	int af_checksum;
	//int rg_ratio;
	//int bg_ratio;
};

extern struct HI846_otp_struct HI846_otp;
extern struct HI846_otp_struct HI847_otp;

int hi846_read_lsc(unsigned short *buffer,int size);
unsigned short hi846_lsc_data[1919];

int hi847_read_lsc(unsigned short *buffer,int size);
unsigned short hi847_lsc_data[1919];

int hi846_otp_flag = 0, hi847_otp_flag = 0;
unsigned int hi846otp_selective_read_region(struct i2c_client *client, unsigned int addr,unsigned char *data, unsigned int size);
unsigned int hi847otp_selective_read_region(struct i2c_client *client, unsigned int addr,unsigned char *data, unsigned int size);

EXPORT_SYMBOL(hi846_lsc_data);
EXPORT_SYMBOL(hi846otp_selective_read_region);
EXPORT_SYMBOL(hi847_lsc_data);
EXPORT_SYMBOL(hi847otp_selective_read_region);


static int hi847_selective_read_region(u32 offset, u8* data, u32 size)
{    
	int i  = 0,checksum = 0,count = 0;
	SENSORDB_HI846_OTP("[hi847_CAM_CAL] selective_read_region offset =%d size %d data read = %d\n", offset,size, *data);
	/*for(i=0; i< 9; i++)
		SENSORDB_HI846_OTP("HI847_otp.wb_data[%d] = 0x%x",i,HI847_otp.wb_data[i]);
	for(i=0; i< 4; i++)
		SENSORDB_HI846_OTP("HI847_otp.af_data[%d] = 0x%x",i+1,HI847_otp.af_data[i+1]);*/
	if(offset==0x0002)
		*data=0x10;
	else if(offset==0x0008)//for awb data
	{
		for(i=0; i< size; i++)
		{
			*data=HI847_otp.wb_data[i];
			data++;
		}
	}
	else if(offset==0x000c)
	{
		for(i=0; i< size; i++)
		{
			*data=HI847_otp.wb_data[i+4];
			data++;
		}
	}
	else if(offset==0x0010)//for af data
	{
		for(i=0; i< size; i++)
		{
			*data=HI847_otp.af_data[i+1];
			data++;
		}
	}
	else if(offset==0x0012)
	{
		for(i=0; i< size; i++)
		{
			*data=HI847_otp.af_data[i+3];
			data++;
		}
	}
	else if(offset==0x0014)//for lsc data
	{
		if(hi847_otp_flag == 0)
		{
			for(count = 0; count < 5; count++)
			{
				hi847_read_lsc(hi847_lsc_data,1919);
				checksum = 0;
				for(i=0; i< 1918; i++)
				{
					checksum += hi847_lsc_data[i];
					//SENSORDB_HI846_OTP("hi846otp_lsc_data, 0x%x , ", hi846_lsc_data[i]);
				}
				checksum = checksum%0xff +1;
				SENSORDB_HI846_OTP("count = %d, checksum = %d, hi847_lsc_data[1918] = %d",count,checksum,hi847_lsc_data[1918]);
				if(checksum == hi847_lsc_data[1918])
				{
					SENSORDB_HI846_OTP("hi847_lsc_data check success");
					hi847_otp_flag = 1;
					break;
				}
				else{
					SENSORDB_HI846_OTP("hi847_lsc_data check failed, do it again");
					continue;
				}
			}
		}
		
		for(i=0; i< size; i++)
		{
			*data=hi847_lsc_data[i];
			//SENSORDB_HI846_OTP("hi847_lsc_data[%d] = %d, *data= %d",i,hi847_lsc_data[i],*data);
			data++;
		}

	}
	return size;
	
}


static int hi846_selective_read_region(u32 offset, u8* data, u32 size)
{    
	int i  = 0,checksum = 0,count = 0;
	SENSORDB_HI846_OTP("[hi846_CAM_CAL] selective_read_region offset =%d size %d data read = %d\n", offset,size, *data);
	/*for(i=0; i< 9; i++)
		SENSORDB_HI846_OTP("HI846_otp.wb_data[%d] = 0x%x",i,HI846_otp.wb_data[i]);
	for(i=0; i< 4; i++)
		SENSORDB_HI846_OTP("HI846_otp.af_data[%d] = 0x%x",i+1,HI846_otp.af_data[i+1]);*/
	if(offset==0x0002)
		*data=0x20;
	else if(offset==0x0008)//for awb data
	{
		for(i=0; i< size; i++)
		{
			*data=HI846_otp.wb_data[i];
			data++;
		}
	}
	else if(offset==0x000c)
	{
		for(i=0; i< size; i++)
		{
			*data=HI846_otp.wb_data[i+4];
			data++;
		}
	}
	else if(offset==0x0010)//for af data
	{
		for(i=0; i< size; i++)
		{
			*data=HI846_otp.af_data[i+1];
			data++;
		}
	}
	else if(offset==0x0012)
	{
		for(i=0; i< size; i++)
		{
			*data=HI846_otp.af_data[i+3];
			data++;
		}
	}
	else if(offset==0x0014)//for lsc data
	{
		if(hi846_otp_flag == 0)
		{
			for(count = 0; count < 5; count++)
			{
				hi846_read_lsc(hi846_lsc_data,1919);
				checksum = 0;
				for(i=0; i< 1918; i++)
				{
					checksum += hi846_lsc_data[i];
					//SENSORDB_HI846_OTP("hi846otp_lsc_data, 0x%x , ", hi846_lsc_data[i]);
				}
				checksum = checksum%0xff +1;
				SENSORDB_HI846_OTP("count = %d,checksum = %d, hi846_lsc_data[1918] = %d",count,checksum,hi846_lsc_data[1918]);
				if(checksum == hi846_lsc_data[1918])
				{
					SENSORDB_HI846_OTP("hi846_lsc_data check success");
					hi846_otp_flag = 1;
					break;
				}
				else{
					SENSORDB_HI846_OTP("hi846_lsc_data check failed, do it again");
					continue;
				}
			}
		}
		
		for(i=0; i< size; i++)
		{
			*data=hi846_lsc_data[i];
			//SENSORDB_HI846_OTP("hi846_lsc_data[%d] = %d, *data= %d",i,hi846_lsc_data[i],*data);
			data++;
		}

	}
	return size;
	
}

unsigned int hi846otp_selective_read_region(struct i2c_client *client, unsigned int addr,
	unsigned char *data, unsigned int size)
{
	SENSORDB_HI846_OTP("addr 0x%x, size is 0x%x",addr,size);

	if (hi846_selective_read_region(addr, data, size))
		return size;
	else
		return 0;
}

unsigned int hi847otp_selective_read_region(struct i2c_client *client, unsigned int addr,
	unsigned char *data, unsigned int size)
{
	SENSORDB_HI846_OTP("addr 0x%x, size is 0x%x",addr,size);

	if (hi847_selective_read_region(addr, data, size))
		return size;
	else
		return 0;
}

