#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>


#include <linux/proc_fs.h> 


#include <linux/dma-mapping.h>


#include <linux/types.h>

#include "kd_camera_typedef.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"

#include "ov13855mipiraw_Sensor.h"
#include "ov13855_otp_cal.h"

#if 1
//extern int iReadReg(u16 a_u2Addr , u8 * a_puBuff , u16 i2cId);
//extern int iWriteReg(u16 a_u2Addr , u32 a_u4Data , u32 a_u4Bytes , u16 i2cId);

//#define OV13855_R2A_write_i2c(addr, para) iWriteReg((u16) addr , (u32) para , 1, OV13855MIPI_WRITE_ID)
#define PFX "OV13855_R2A_OTP"
//#define LOG_INF(format, args...)	//xlog_printk(ANDROID_LOG_INFO   , PFX, "[%s] " format, __FUNCTION__, ##args)
#define LOG_INF(format, args...)    pr_debug(PFX "[%s] " format, __FUNCTION__, ##args)

#define Delay(ms)  mdelay(ms)
static unsigned char OV13855MIPI_WRITE_ID = 0XA0;
int OV13855_OTP_Flag=0;
int OV13855_pdaf_Flag=0;
static unsigned char lenc[1868];
kal_uint16 OV13855_R2A_read_i2c(kal_uint32 addr)
{
	    kal_uint16 get_byte=0;
    char pu_send_cmd[2] = {(char)(addr >> 8), (char)(addr & 0xFF) };
	//kdSetI2CSpeed(imgsensor_info.i2c_speed);
    iReadRegI2C(pu_send_cmd, 2, (u8*)&get_byte, 1,OV13855MIPI_WRITE_ID);

    return get_byte;
}

void OV13855_R2A_write_i2c(u16 addr, u32 para)
{
    char pu_send_cmd[3] = {(char)(addr >> 8), (char)(addr & 0xFF), (char)(para & 0xFF)};
	//kdSetI2CSpeed(imgsensor_info.i2c_speed);
    iWriteRegI2C(pu_send_cmd, 3, OV13855MIPI_WRITE_ID);
}

#endif

void ov13855_otp_cali(void)
{
	struct otp_struct current_otp;
	//struct pdaf_struct current_pdaf;
	//OV13855MIPI_WRITE_ID = writeid;
	memset(&current_otp, 0, sizeof(struct otp_struct));
	//memset(&current_pdaf, 0, sizeof(struct pdaf_struct));
	ov13855_read_otp(&current_otp);
	//read_pdaf(&current_pdaf);
	//apply_otp(&current_otp);
}
void ov13855_pdaf_cali(void)
{
	
	struct pdaf_struct current_pdaf;
	memset(&current_pdaf, 0, sizeof(struct pdaf_struct));
	ov13855_read_pdaf(&current_pdaf);
}
int ov13855_read_pdaf(struct pdaf_struct *pdaf_ptr)
{
	int pdaf_flag=0;
	int checksumpdaf=0;
	int pdaf_offset = 0x0801;
	int i=0;
	
	//set 0x5002[1] to "0"
	int temp1=0;
	temp1 = OV13855_R2A_read_i2c(0x5002);
	OV13855_R2A_write_i2c(0x5002, (0x00 & 0x02) | (temp1 & (~0x02)));
	// read OTP into buffer
	OV13855_R2A_write_i2c(0x3d84, 0xC0);
	OV13855_R2A_write_i2c(0x3d88, 0x00); // OTP start address
	OV13855_R2A_write_i2c(0x3d89, 0x00);
	OV13855_R2A_write_i2c(0x3d8A, 0x01); // OTP end address
	OV13855_R2A_write_i2c(0x3d8B, 0xE5);
	OV13855_R2A_write_i2c(0x3d81, 0x01); // load otp into buffer
	Delay(10);
	  //pdaf
	  pdaf_flag=OV13855_R2A_read_i2c(0x0800);
	  printk("pdaf_flag : 0x%x \n", pdaf_flag);
	  if((pdaf_flag&0xCF)==0xC1)
	  {
		for(i = 0; i < 1372; i++) {
		(* pdaf_ptr).pdaf[i]= OV13855_R2A_read_i2c(pdaf_offset + i);
			checksumpdaf += (* pdaf_ptr).pdaf[i];
			//printk(" Lenc (* otp_ptr).pdaf[%d] : %x \n", i, (* pdaf_ptr).pdaf[i]);	
		}
		checksumpdaf = (checksumpdaf)%256;
		(* pdaf_ptr).checksumpdaf=OV13855_R2A_read_i2c(0x0D5D);
		printk(" checksumpdaf:%d, (* otp_ptr).checksumpdaf :%d\n", checksumpdaf, (* pdaf_ptr).checksumpdaf);
	  }
	  else{
		printk("no pdaf data\n");
	  }
      if(((pdaf_flag&0xCF)==0xC1)&&(checksumpdaf==(* pdaf_ptr).checksumpdaf))
      	{
		OV13855_pdaf_Flag=1;
	  }
	  else{
		OV13855_pdaf_Flag=0;
	  }
	//set 0x5002[1] to "1"
	temp1 = OV13855_R2A_read_i2c(0x5002);
	OV13855_R2A_write_i2c(0x5002, (0x02 & 0x02) | (temp1 & (~0x02)));

	//return (*otp_ptr).flag;
	return 0;
}
int ov13855_read_otp(struct otp_struct *otp_ptr)
{
	int otp_flag=0;
	int awb_flag=0;
	int vcm_flag=0;
	int lsc_flag=0;
	int addr=0;
	int lsc_addr=0;
	int af_addr=0;
	int i=0;
	int checksum=0;
	int checksumAWB=0;
	int checksumAF=0;
	int checksumLSC = 0;
	//int pdaf_flag=0;
	//int checksumpdaf=0;
	//int pdaf_offset = 0x79b;
	
	
	//set 0x5002[1] to "0"
	int temp1=0;
	temp1 = OV13855_R2A_read_i2c(0x5002);
	OV13855_R2A_write_i2c(0x5002, (0x00 & 0x02) | (temp1 & (~0x02)));
	// read OTP into buffer
	OV13855_R2A_write_i2c(0x3d84, 0xC0);
	OV13855_R2A_write_i2c(0x3d88, 0x00); // OTP start address
	OV13855_R2A_write_i2c(0x3d89, 0x00);
	OV13855_R2A_write_i2c(0x3d8A, 0x01); // OTP end address
	OV13855_R2A_write_i2c(0x3d8B, 0xE5);
	OV13855_R2A_write_i2c(0x3d81, 0x01); // load otp into buffer
	Delay(10);
	// OTP base information 
	otp_flag = OV13855_R2A_read_i2c(0x0000);
	LOG_INF(" module information : %x \n", otp_flag);
	addr = 0x0000;
	
	if(otp_flag == 0x80) {
		//(*otp_ptr).flag = 0x01; // valid info  in OTP
		(*otp_ptr).module_integrator_id = OV13855_R2A_read_i2c(addr+4);
		(*otp_ptr).lens_id = OV13855_R2A_read_i2c( addr + 5);
		(*otp_ptr).production_year = OV13855_R2A_read_i2c( addr + 1);
		(*otp_ptr).production_month = OV13855_R2A_read_i2c( addr + 2);
		(*otp_ptr).production_day = OV13855_R2A_read_i2c(addr + 3);
		(*otp_ptr).vcm_id = OV13855_R2A_read_i2c(addr + 6);
		(*otp_ptr).vcm_ic_id = OV13855_R2A_read_i2c(addr + 7);
		(*otp_ptr).checksumOTP = OV13855_R2A_read_i2c(addr + 21);
	}
	else {
	       //(*otp_ptr).flag = 0x00; // not info in OTP
		(*otp_ptr).module_integrator_id = 0;
		(*otp_ptr).lens_id = 0;
		(*otp_ptr).production_year = 0;
		(*otp_ptr).production_month = 0;
		(*otp_ptr).production_day = 0;
		(*otp_ptr).vcm_id=0;
		(*otp_ptr).vcm_ic_id=0;
		(*otp_ptr).checksumOTP = 0;
	}
	for(i=1;i<=20;i++){
	checksum+=OV13855_R2A_read_i2c(addr+i);
	}
	checksum = (checksum)%256;
	 printk("[OV13855 OTP]otp_flag=%d,module_integrator_id=0x%x,lens_id=0x%x,data=%d-%d-%d\n",otp_flag,(*otp_ptr).module_integrator_id,(*otp_ptr).lens_id,(*otp_ptr).production_year,(*otp_ptr).production_month,(*otp_ptr).production_day);
	printk("[OV13855 OTP]vcm_id=0x%x,vcm_ic_id=0x%x",(*otp_ptr).vcm_id,(*otp_ptr).vcm_ic_id);
	printk("[OV13855 OTP]checksum =%d",checksum);
	 printk("[OV13855 OTP](*otp_ptr).checksumOTP =%d",(*otp_ptr).checksumOTP);
	//OTP AWB information
	awb_flag=OV13855_R2A_read_i2c(0x0020);
	if(awb_flag==0x80){
		//(*otp_ptr).flag = 0x01;
		(*otp_ptr).R_Gr_ratio = (OV13855_R2A_read_i2c(0x0022)<<8) | (OV13855_R2A_read_i2c(0x0021));
		(*otp_ptr).B_Gr_ratio = (OV13855_R2A_read_i2c(0x0024)<<8) | (OV13855_R2A_read_i2c(0x0023));
		(*otp_ptr).g_R_Gr_ratio = (OV13855_R2A_read_i2c(0x0026)<<8) | (OV13855_R2A_read_i2c(0x0025));
		(*otp_ptr).g_B_Gr_ratio = (OV13855_R2A_read_i2c(0x0028)<<8) | (OV13855_R2A_read_i2c(0x0027));
		(*otp_ptr).checksumAWB=OV13855_R2A_read_i2c(0x0035);
	}
	else{
		(*otp_ptr).R_Gr_ratio = 0;
		(*otp_ptr).B_Gr_ratio = 0;
		//(*otp_ptr).Gb_Gr_ratio =0;
		(*otp_ptr).g_R_Gr_ratio = 0;
		(*otp_ptr).g_B_Gr_ratio = 0;
		//(*otp_ptr).g_Gb_Gr_ratio = 0;
		(*otp_ptr).checksumAWB=0;
	}
	for(i=0;i<20;i++){
	checksumAWB+=OV13855_R2A_read_i2c(0x0021+i);
	}
	checksumAWB = (checksumAWB)%256;
	printk("[OV13855 OTP]AWB_flag=%d,R_Gr_ratio=0x%x,B_Gr_ratio=0x%x\n",awb_flag,(*otp_ptr).R_Gr_ratio,(*otp_ptr).B_Gr_ratio);
	printk("[OV13855 OTP]g_R_Gr_ratio=0x%x,g_B_Gr_ratio=0x%x\n",(*otp_ptr).g_R_Gr_ratio,(*otp_ptr).g_B_Gr_ratio);
	printk("[OV13855 OTP](*otp_ptr).checksumAWB=%d\n",(*otp_ptr).checksumAWB);
	printk("[OV13855 OTP]checksumAWB=%d\n",checksumAWB);
	// OTP VCM Calibration
	vcm_flag = OV13855_R2A_read_i2c(0x07C0);
	printk(" [OV13855 OTP]VCM calibration flag : %x \n", vcm_flag);

	if(vcm_flag == 0x80) {
		//(*otp_ptr).flag == 0x01;
		(* otp_ptr).VCM_macro = (OV13855_R2A_read_i2c(0x07C5)<<8) | (OV13855_R2A_read_i2c(0x07C4));
		(* otp_ptr).VCM_inf = (OV13855_R2A_read_i2c(0x07C3)<<8) | (OV13855_R2A_read_i2c(0x07C2));
		(* otp_ptr).VCM_dir = OV13855_R2A_read_i2c(0x07C1);
		//(* otp_ptr).VCM_start = ((OV13855_R2A_read_i2c(0x076B)<<8)&0xff00) | (OV13855_R2A_read_i2c(0x076C));
		(* otp_ptr).checksumAF = OV13855_R2A_read_i2c(0x07CA);
	}
	else {
		(* otp_ptr).VCM_macro = 0;
		(* otp_ptr).VCM_inf = 0;
		(* otp_ptr).VCM_dir = 0;
		//(* otp_ptr).VCM_start = 0;
		(* otp_ptr).checksumAF = 0;
	}
	af_addr=0x07C1;
	for(i=0;i<9;i++){
	checksumAF+=OV13855_R2A_read_i2c(af_addr+i);
	}
	checksumAF = (checksumAF)%256;
	printk(" [OV13855 OTP]VCM_macro: %x ,VCM_inf: %x,VCM_dir: %x,(* otp_ptr).checksumAF: %d\n", (* otp_ptr).VCM_macro,(* otp_ptr).VCM_inf,(* otp_ptr).VCM_dir,(* otp_ptr).checksumAF);
	printk(" [OV13855 OTP]checksumAF:%d\n",checksumAF);
	// OTP Lenc Calibration
	lsc_flag = OV13855_R2A_read_i2c(0x0040);
	printk(" Lenc calibration flag : %x \n", lsc_flag);
	lsc_addr=0x0041;
      if(lsc_flag == 0x81) {
		for(i=0;i<1868;i++) {
			lenc[i]= OV13855_R2A_read_i2c(lsc_addr + i);
			checksumLSC += lenc[i];
			//printk(" Lenc (* otp_ptr).lenc[%d] : %x \n", i, lenc[i]);
		}
		//Decode the lenc buffer from OTP , from 186 bytes to 360 bytes
		//int lenc_out[360];

		checksumLSC = (checksumLSC)%256;
		(* otp_ptr).checksumLSC=OV13855_R2A_read_i2c(0x078D);
		printk(" checksumLSC:%d, (* otp_ptr).checksumLSC :%d\n", checksumLSC, (* otp_ptr).checksumLSC);
	}
	  else{
			for(i=0;i<1868;i++) {
			lenc[i]= 0;
			printk("OV13855 NO LSC OTP  \n");
			}

	  }
	
	if((otp_flag==0x80)&&(awb_flag==0x80)&&(lsc_flag==0x81)&&((*otp_ptr).module_integrator_id==0x6)&&((*otp_ptr).lens_id==0xd1)&&((*otp_ptr).vcm_id==0x65)&&((*otp_ptr).vcm_ic_id==0xe)&&(checksum==(*otp_ptr).checksumOTP)&&(checksumAWB==(*otp_ptr).checksumAWB)
		     &&(checksumAF==(* otp_ptr).checksumAF)&&(checksumLSC==(* otp_ptr).checksumLSC))
	{
			OV13855_OTP_Flag=1;
	}
	else{
			OV13855_OTP_Flag=0;
		}

	//set 0x5002[1] to "1"
	temp1 = OV13855_R2A_read_i2c(0x5002);
	OV13855_R2A_write_i2c(0x5002, (0x02 & 0x02) | (temp1 & (~0x02)));

	//return (*otp_ptr).flag;
	return 0;
}


