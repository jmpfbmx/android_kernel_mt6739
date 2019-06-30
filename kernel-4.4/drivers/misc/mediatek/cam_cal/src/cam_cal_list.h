#ifndef __CAM_CAL_LIST_H
#define __CAM_CAL_LIST_H
#include <linux/i2c.h>

typedef unsigned int (*cam_cal_cmd_func)(struct i2c_client *client, unsigned int addr,
	unsigned char *data, unsigned int size);

typedef unsigned int (*cam_cal_check_func)(struct i2c_client *client,
	cam_cal_cmd_func readCamCalData);

enum CAM_CAL_CMD_TYPE {
	CMD_NONE = 0,
	CMD_AUTO,
	CMD_MAIN,
	CMD_MAIN2,
	CMD_SUB,
	CMD_SUB2,
	CMD_BRCB032GWZ,
	CMD_CAT24C16,
	CMD_GT24C32A,
//begin, add by zhijun.fu for fm24c64d on 2017-11-29
	CMD_FM24C64D,
//end, add by zhijun.fu for fm24c64d on 2017-11-29
//begin, add by zhijun.fu for hi846_eeprom on 2017-12-13
	CMD_HI846OTP,
//end, add by zhijun.fu for hi846_eeprom on 2017-12-13
	CMD_NUM
};

struct stCAM_CAL_LIST_STRUCT {
	unsigned int sensorID;
	unsigned int slaveID;
	enum CAM_CAL_CMD_TYPE cmdType;
	cam_cal_check_func checkFunc;
};

struct stCAM_CAL_FUNC_STRUCT {
	enum CAM_CAL_CMD_TYPE cmdType;
	cam_cal_cmd_func readCamCalData;
};


unsigned int cam_cal_get_sensor_list(struct stCAM_CAL_LIST_STRUCT **ppCamcalList);
unsigned int cam_cal_get_func_list(struct stCAM_CAL_FUNC_STRUCT **ppCamcalFuncList);
unsigned int cam_cal_check_mtk_cid(struct i2c_client *client, cam_cal_cmd_func readCamCalData);
//Begin added by zhanyong.yin for XR5424731 on 2017/10/19
unsigned int cam_cal_check_mtk_cid_ov13855(struct i2c_client *client, cam_cal_cmd_func readCamCalData);
//begin, add by zhijun.fu for hi846_eeprom on 2017-12-13
unsigned int cam_cal_check_mtk_cid_hi846(struct i2c_client *client, cam_cal_cmd_func readCamCalData);
//end, add by zhijun.fu for hi846_eeprom on 2017-12-13
unsigned int cam_cal_check_mtk_cid_hi1333(struct i2c_client *client, cam_cal_cmd_func readCamCalData);
//End added by zhanyong.yin for XR5424731 on 2017/10/19
unsigned int cam_cal_check_double_eeprom(struct i2c_client *client,
	cam_cal_cmd_func readCamCalData);

/*defined on kd_sensorlist.c*/
void CAMERA_HW_Get_i2C_BusNum(unsigned int *I2C_Bus_Num);

#endif /* __CAM_CAL_LIST_H */

