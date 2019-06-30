#include <linux/kernel.h>
#include "cam_cal_list.h"
#include "kd_imgsensor.h"

/*Common EEPRom Driver*/
#include "common/BRCB032GWZ_3/BRCB032GWZ_3.h"
#include "common/cat24c16/cat24c16.h"
#include "common/GT24c32a/GT24c32a.h"
#include "common/fm24c64d/fm24c64d.h"
#include "common/hi846otp/hi846otp.h"

#define CAM_CAL_DEBUG
#ifdef CAM_CAL_DEBUG
/*#include <linux/log.h>*/
#include <linux/kern_levels.h>
#define PFX "cam_cal_list"

#define CAM_CALINF(format, args...)     pr_err(PFX "[%s] " format, __func__, ##args)
#define CAM_CALDB(format, args...)      pr_err(PFX "[%s] " format, __func__, ##args)
#define CAM_CALERR(format, args...)     pr_err(format, ##args)
#else
#define CAM_CALINF(x, ...)
#define CAM_CALDB(x, ...)
#define CAM_CALERR(x, ...)
#endif
#define CAM_CALDB_HI846otp(format, args...)      pr_info("HI846otp:" "[%s] " format, __func__, ##args)
//Begin modified by zhanyong.yin for XR5424731 on 2017/10/19
//begin, add by zhijun.fu for fm24c64d on 2017-11-29

#define MTK_MAX_CID_NUM 8
unsigned int mtkCidList[MTK_MAX_CID_NUM] = {
//begin, add by zhijun.fu for hi846_eeprom on 2017-12-13
	0x00000010,//add for hi1333
	0x00000020,
//end, add by zhijun.fu for hi846_eeprom on 2017-12-13
	0x00000006,
	0x00000044,
//End modified by zhanyong.yin for XR5424731 on 2017/10/19
	0x010b00ff,/*Single MTK Format*/
	0x020b00ff,/*Double MTK Format in One OTP/EEPRom - Legacy*/
	0x030b00ff,/*Double MTK Format in One OTP/EEPRom*/
	0x040b00ff /*Double MTK Format in One OTP/EEPRom V1.4*/
};

struct stCAM_CAL_FUNC_STRUCT g_camCalCMDFunc[] = {
//begin, add by zhijun.fu for hi846_eeprom on 2017-12-13
	{CMD_HI846OTP, hi846otp_selective_read_region},
//end, add by zhijun.fu for hi846_eeprom on 2017-12-13
//begin, add by zhijun.fu for fm24c64d on 2017-11-29
	{CMD_FM24C64D, fm24c64d_selective_read_region},
//end, add by zhijun.fu for fm24c64d on 2017-11-29
	{CMD_BRCB032GWZ, brcb032gwz_selective_read_region},
	{CMD_CAT24C16, cat24c16_selective_read_region},
	{CMD_GT24C32A, gt24c32a_selective_read_region},
	/*      ADD before this line */
	{0, 0} /*end of list*/
};

struct stCAM_CAL_LIST_STRUCT g_camCalList[] = {
/*Below is commom sensor */
//begin, add by zhijun.fu for hi846_eeprom on 2017-12-17
	{HI846_SENSOR_ID, 0x40, CMD_HI846OTP, cam_cal_check_mtk_cid_hi846},//add by jimfu
	{HI1333_SENSOR_ID, 0xA0, CMD_FM24C64D, cam_cal_check_mtk_cid_hi1333},
//end, add by zhijun.fu for hi846_eeprom on 2017-12-17
//Begin added by zhanyong.yin for XR5424731 on 2017/10/19
	{OV13855_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid_ov13855},
//End added by zhanyong.yin for XR5424731 on 2017/10/19
	{IMX338_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{S5K4E6_SENSOR_ID, 0xA8, CMD_AUTO, cam_cal_check_mtk_cid},
	{IMX386_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{S5K3M3_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{S5K2L7_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{IMX398_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_double_eeprom},
	{IMX318_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{OV8858_SENSOR_ID, 0xA8, CMD_AUTO, cam_cal_check_mtk_cid},
	{IMX350_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{S5K3P8SX_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_double_eeprom},
	{IMX386_MONO_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{IMX576_SENSOR_ID, 0xA2, CMD_AUTO, cam_cal_check_double_eeprom},
/*99 */
	{IMX258_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{IMX258_MONO_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
/*97*/
	{OV23850_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{OV23850_SENSOR_ID, 0xA8, CMD_AUTO, cam_cal_check_mtk_cid},
	{S5K3M2_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
/*39*/
	{OV13870_SENSOR_ID, 0xA8, CMD_AUTO, cam_cal_check_mtk_cid},
	{OV8856_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
/*55*/
	{S5K2P8_SENSOR_ID, 0xA2, CMD_AUTO, cam_cal_check_mtk_cid},
	{S5K2P8_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{OV8858_SENSOR_ID, 0xA2, CMD_AUTO, cam_cal_check_mtk_cid},
/* Others */
	{S5K2X8_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{IMX376_SENSOR_ID, 0xA2, CMD_AUTO, cam_cal_check_mtk_cid},
	{IMX214_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	{IMX214_MONO_SENSOR_ID, 0xA0, CMD_AUTO, cam_cal_check_mtk_cid},
	/*  ADD before this line */
	{0, 0, CMD_NONE, 0} /*end of list*/
};


unsigned int cam_cal_get_sensor_list(struct stCAM_CAL_LIST_STRUCT **ppCamcalList)

{
	if (ppCamcalList == NULL)
		return 1;

	*ppCamcalList = &g_camCalList[0];
	return 0;
}


unsigned int cam_cal_get_func_list(struct stCAM_CAL_FUNC_STRUCT **ppCamcalFuncList)
{
	if (ppCamcalFuncList == NULL)
		return 1;

	*ppCamcalFuncList = &g_camCalCMDFunc[0];
	return 0;
}

unsigned int cam_cal_check_mtk_cid(struct i2c_client *client, cam_cal_cmd_func readCamCalData)
{
	unsigned int calibrationID = 0, ret = 0;
	int j = 0;
	CAM_CALINF("zhijun.fu , enter cam_cal_check_mtk_cid");
	if (readCamCalData != NULL) {
		readCamCalData(client, 1, (unsigned char *)&calibrationID, 4);
		CAM_CALDB("calibrationID = %x\n", calibrationID);
	}

	if (calibrationID != 0)
		for (j = 0; j < MTK_MAX_CID_NUM; j++) {
			CAM_CALDB("mtkCidList[%d] == %x\n", j, calibrationID);
			if (mtkCidList[j] == calibrationID) {
				ret = 1;
				break;
			}
		}

	CAM_CALDB("ret =%d\n", ret);
	return ret;
}
//begin, add by zhijun.fu for hi846_eeprom on 2017-12-13
unsigned int cam_cal_check_mtk_cid_hi846(struct i2c_client *client, cam_cal_cmd_func readCamCalData)
{
	unsigned int calibrationID = 0, ret = 0;
	int j = 0;
	CAM_CALDB_HI846otp("zhijun.fu , enter cam_cal_check_mtk_cid_hi846");
	if (readCamCalData != NULL) {
		readCamCalData(client, 0x0002, (unsigned char *)&calibrationID, 1);
		CAM_CALDB_HI846otp("calibrationID = %x\n", calibrationID);
		if(calibrationID == 0)
		{
			calibrationID = 0x20;
			ret = 1;
		}
	}

	if (calibrationID != 0)
		for (j = 0; j < MTK_MAX_CID_NUM; j++) {
			CAM_CALDB_HI846otp("mtkCidList[%d] == %x\n", j, calibrationID);
			if (mtkCidList[j] == calibrationID) {
				ret = 1;
				break;
			}
		}
	CAM_CALDB_HI846otp("ret =%d\n", ret);
	return ret;
}

//end, add by zhijun.fu for hi846_eeprom on 2017-12-13
//Begin added by zhanyong.yin for XR5424731 on 2017/10/19
unsigned int cam_cal_check_mtk_cid_ov13855(struct i2c_client *client, cam_cal_cmd_func readCamCalData)
{
	unsigned int calibrationID = 0, ret = 0;
	int j = 0;

	if (readCamCalData != NULL) {
		readCamCalData(client, 1, (unsigned char *)&calibrationID, 1);
		CAM_CALDB("calibrationID = %x\n", calibrationID);
		if(calibrationID == 0)
		{
			calibrationID = 0x10;
			ret = 1;
		}
	}
	calibrationID=((calibrationID>>24)&0xff);
	CAM_CALDB("calibrationID1 = %x\n", calibrationID);
	if (calibrationID != 0)
		for (j = 0; j < MTK_MAX_CID_NUM; j++) {
			CAM_CALDB("mtkCidList[%d] == %x\n", j, calibrationID);
			if (mtkCidList[j] == calibrationID) {
				ret = 1;
				break;
			}
		}

	CAM_CALDB("ret =%d\n", ret);
	return ret;
}
unsigned int cam_cal_check_mtk_cid_hi1333(struct i2c_client *client, cam_cal_cmd_func readCamCalData)
{
	unsigned int calibrationID = 0, ret = 0;
	int j = 0;
	CAM_CALDB("zhijun.fu , enter cam_cal_check_mtk_cid_hi1333");
	if (readCamCalData != NULL) {
		readCamCalData(client, 0x0001, (unsigned char *)&calibrationID, 1);
		CAM_CALDB("calibrationID = %x\n", calibrationID);
		if(calibrationID == 0)
		{
			calibrationID = 0x10;
			ret = 1;
		}
	}

	if (calibrationID != 0)
		for (j = 0; j < MTK_MAX_CID_NUM; j++) {
			CAM_CALDB("mtkCidList[%d] == %x\n", j, calibrationID);
			if (mtkCidList[j] == calibrationID) {
				ret = 1;
				break;
			}
		}
	CAM_CALDB("ret =%d\n", ret);
	return ret;
}
//End added by zhanyong.yin for XR5424731 on 2017/10/19
unsigned int cam_cal_check_double_eeprom(struct i2c_client *client, cam_cal_cmd_func readCamCalData)
{
	unsigned int calibrationID = 0, ret = 1;

	CAM_CALDB("start cam_cal_check_double_eeprom !\n");
	if (readCamCalData != NULL) {
		CAM_CALDB("readCamCalData != NULL !\n");
		readCamCalData(client, 1, (unsigned char *)&calibrationID, 4);
		CAM_CALDB("calibrationID = %x\n", calibrationID);
	}

	return ret;
}



