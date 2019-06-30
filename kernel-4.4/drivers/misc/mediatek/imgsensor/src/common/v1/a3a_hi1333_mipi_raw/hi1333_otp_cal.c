/************************************************* 
History: 1.0 Added by zhanyong.yin for XR 5581518 on 2017/11/20
*************************************************/
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

#include "hi1333_mipi_raw_Sensor.h"
#include "hi1333_otp_cal.h"
#define PFX "HI1333_R2A_OTP"
//#define LOG_INF(format, args...)	//xlog_printk(ANDROID_LOG_INFO   , PFX, "[%s] " format, __FUNCTION__, ##args)
#define LOG_INF(format, args...)    pr_debug(PFX "[%s] " format, __FUNCTION__, ##args)

#define Delay(ms)  mdelay(ms)
static unsigned char HI1333MIPI_WRITE_ID = 0XA0;
int HI1333_OTP_Flag=0;
int HI1333_pdaf_Flag=0;
static unsigned char lenc[1868];
static unsigned char pdaf[1404];
kal_uint16 HI1333_R2A_read_i2c(kal_uint32 addr)
{
	    kal_uint16 get_byte=0;
    char pu_send_cmd[2] = {(char)(addr >> 8), (char)(addr & 0xFF) };
    iReadRegI2C(pu_send_cmd, 2, (u8*)&get_byte, 1,HI1333MIPI_WRITE_ID);

    return get_byte;
}

void HI1333_R2A_write_i2c(u16 addr, u32 para)
{
    char pu_send_cmd[3] = {(char)(addr >> 8), (char)(addr & 0xFF), (char)(para & 0xFF)};
    iWriteRegI2C(pu_send_cmd, 3, HI1333MIPI_WRITE_ID);
}

void hi1333_otp_cali(void)
{
	struct hi1333_otp_struct current_otp;

	memset(&current_otp, 0, sizeof(struct hi1333_otp_struct));
	hi1333_read_otp(&current_otp);
}

void hi1333_pdaf_cali(void)
{
	
	struct hi1333_pdaf_struct current_pdaf;
	memset(&current_pdaf, 0, sizeof(struct hi1333_pdaf_struct));
	hi1333_read_pdaf(&current_pdaf);
}
int hi1333_read_pdaf(struct hi1333_pdaf_struct *pdaf_ptr)
{
	int pdaf_flag=0;
	int checksumpdaf=0;
	int pdaf_offset = 0x079B;
	int i=0;

	  //pdaf
	  pdaf_flag=HI1333_R2A_read_i2c(0x079A);
	  printk("pdaf_flag : 0x%x \n", pdaf_flag);
	  if(pdaf_flag==0x02)
	  {
		for(i = 0; i < 1404; i++) {
		pdaf[i]= HI1333_R2A_read_i2c(pdaf_offset + i);
			checksumpdaf += pdaf[i];
			//printk(" Lenc (* otp_ptr).pdaf[%d] : %x \n", i, (* pdaf_ptr).pdaf[i]);	
		}
		checksumpdaf = (checksumpdaf)%256;
		(* pdaf_ptr).checksumpdaf=HI1333_R2A_read_i2c(0x0D37);
		printk(" checksumpdaf:%d, (* otp_ptr).checksumpdaf :%d\n", checksumpdaf, (* pdaf_ptr).checksumpdaf);
	  }
	  else{
		printk("no pdaf data\n");
	  }
      if((pdaf_flag==0x02)&&(checksumpdaf==(* pdaf_ptr).checksumpdaf))
      	{
		HI1333_pdaf_Flag=1;
	  }
	  else{
		HI1333_pdaf_Flag=0;
	  }
	return 0;
}
int hi1333_read_otp(struct hi1333_otp_struct *otp_ptr)
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

	otp_flag = HI1333_R2A_read_i2c(0x0000);
	LOG_INF(" module information : %x \n", otp_flag);
	addr = 0x0000;
	
	if(otp_flag == 0x01) {
		//(*otp_ptr).flag = 0x01; // valid info  in OTP
		(*otp_ptr).module_integrator_id = HI1333_R2A_read_i2c(addr+1);
		(*otp_ptr).lens_id = HI1333_R2A_read_i2c( addr + 5);
		(*otp_ptr).production_year = HI1333_R2A_read_i2c( addr + 2);
		(*otp_ptr).production_month = HI1333_R2A_read_i2c( addr + 3);
		(*otp_ptr).production_day = HI1333_R2A_read_i2c(addr + 4);
		(*otp_ptr).vcm_id = HI1333_R2A_read_i2c(addr + 6);
		(*otp_ptr).vcm_ic_id = HI1333_R2A_read_i2c(addr + 7);
		(*otp_ptr).checksumOTP = HI1333_R2A_read_i2c(addr + 8);
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
	for(i=1;i<=7;i++){
	checksum+=HI1333_R2A_read_i2c(addr+i);
	}
	checksum = (checksum)%256;
	 printk("[HI1333 OTP]otp_flag=%d,module_integrator_id=0x%x,lens_id=0x%x,data=%d-%d-%d\n",otp_flag,(*otp_ptr).module_integrator_id,(*otp_ptr).lens_id,(*otp_ptr).production_year,(*otp_ptr).production_month,(*otp_ptr).production_day);
	printk("[HI1333 OTP]vcm_id=0x%x,vcm_ic_id=0x%x",(*otp_ptr).vcm_id,(*otp_ptr).vcm_ic_id);
	printk("[HI1333 OTP]checksum =%d",checksum);
	 printk("[HI1333 OTP](*otp_ptr).checksumOTP =%d",(*otp_ptr).checksumOTP);
	//OTP AWB information
	awb_flag=HI1333_R2A_read_i2c(0x0009);
	if(awb_flag==0x01){
		//(*otp_ptr).flag = 0x01;
		(*otp_ptr).R_Gr_ratio = (HI1333_R2A_read_i2c(0x000A)<<8) | (HI1333_R2A_read_i2c(0x000B));
		(*otp_ptr).B_Gr_ratio = (HI1333_R2A_read_i2c(0x000C)<<8) | (HI1333_R2A_read_i2c(0x000D));
		(*otp_ptr).g_R_Gr_ratio = (HI1333_R2A_read_i2c(0x0010)<<8) | (HI1333_R2A_read_i2c(0x0011));
		(*otp_ptr).g_B_Gr_ratio = (HI1333_R2A_read_i2c(0x0012)<<8) | (HI1333_R2A_read_i2c(0x0013));
		(*otp_ptr).checksumAWB=HI1333_R2A_read_i2c(0x0016);
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
	for(i=0;i<12;i++){
	checksumAWB+=HI1333_R2A_read_i2c(0x000A+i);
	}
	checksumAWB = (checksumAWB)%256;
	printk("[HI1333 OTP]AWB_flag=%d,R_Gr_ratio=0x%x,B_Gr_ratio=0x%x\n",awb_flag,(*otp_ptr).R_Gr_ratio,(*otp_ptr).B_Gr_ratio);
	printk("[HI1333 OTP]g_R_Gr_ratio=0x%x,g_B_Gr_ratio=0x%x\n",(*otp_ptr).g_R_Gr_ratio,(*otp_ptr).g_B_Gr_ratio);
	printk("[HI1333 OTP](*otp_ptr).checksumAWB=%d\n",(*otp_ptr).checksumAWB);
	printk("[HI1333 OTP]checksumAWB=%d\n",checksumAWB);
	// OTP VCM Calibration
	vcm_flag = HI1333_R2A_read_i2c(0x0765);
	printk(" [HI1333 OTP]VCM calibration flag : %x \n", vcm_flag);

	if(vcm_flag == 0x01) {
		(* otp_ptr).VCM_macro = (HI1333_R2A_read_i2c(0x0769)<<8) | (HI1333_R2A_read_i2c(0x076A));
		(* otp_ptr).VCM_inf = (HI1333_R2A_read_i2c(0x0767)<<8) | (HI1333_R2A_read_i2c(0x0768));
		(* otp_ptr).VCM_dir = HI1333_R2A_read_i2c(0x0766);
		(* otp_ptr).checksumAF =HI1333_R2A_read_i2c(0x076D);
	}
	else {
		(* otp_ptr).VCM_macro = 0;
		(* otp_ptr).VCM_inf = 0;
		(* otp_ptr).VCM_dir = 0;
		//(* otp_ptr).VCM_start = 0;
		(* otp_ptr).checksumAF = 0;
	}
	af_addr=0x0766;
	for(i=0;i<7;i++){
	checksumAF+=HI1333_R2A_read_i2c(af_addr+i);
	}
	checksumAF = (checksumAF)%256;
	printk(" [HI1333 OTP]VCM_macro: %x ,VCM_inf: %x,VCM_dir: %x,(* otp_ptr).checksumAF: %d\n", (* otp_ptr).VCM_macro,(* otp_ptr).VCM_inf,(* otp_ptr).VCM_dir,(* otp_ptr).checksumAF);
	printk(" [HI1333 OTP]checksumAF:%d\n",checksumAF);
	// OTP Lenc Calibration
	lsc_flag = HI1333_R2A_read_i2c(0x0017);
	printk(" Lenc calibration flag : %x \n", lsc_flag);
	lsc_addr=0x0018;
      if(lsc_flag == 0x01) {
		for(i=0;i<1868;i++) {
			lenc[i]= HI1333_R2A_read_i2c(lsc_addr + i);
			checksumLSC += lenc[i];
			//printk(" Lenc (* otp_ptr).lenc[%d] : %x \n", i, lenc[i]);
		}
		//Decode the lenc buffer from OTP , from 186 bytes to 360 bytes
		//int lenc_out[360];

		checksumLSC = (checksumLSC)%256;
		(* otp_ptr).checksumLSC=HI1333_R2A_read_i2c(0x0764);
		printk(" checksumLSC:%d, (* otp_ptr).checksumLSC :%d\n", checksumLSC, (* otp_ptr).checksumLSC);
	}
	  else{
			for(i=0;i<1868;i++) {
			lenc[i]= 0;
			printk("HI1333 NO LSC OTP  \n");
			}

	  }
	
	if((otp_flag==0x01)&&(awb_flag==0x01)&&(lsc_flag==0x01)&&((*otp_ptr).module_integrator_id==0x44)&&((*otp_ptr).lens_id==0xd3)&&((*otp_ptr).vcm_id==0xd1)&&((*otp_ptr).vcm_ic_id==0xe)&&(checksum==(*otp_ptr).checksumOTP)&&(checksumAWB==(*otp_ptr).checksumAWB)
		     &&(checksumAF==(* otp_ptr).checksumAF)&&(checksumLSC==(* otp_ptr).checksumLSC))
	{
			HI1333_OTP_Flag=1;
	}
	else{
			HI1333_OTP_Flag=0;
		}

	return 0;
}


