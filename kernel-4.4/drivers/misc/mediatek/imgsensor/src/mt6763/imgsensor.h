
#ifndef __IMGSENSOR_H__
#define __IMGSENSOR_H__
#define IMGSENSOR_DEVICE_NNUMBER 255

#include "imgsensor_common.h"
#include "imgsensor_i2c.h"
#include "imgsensor_hw.h"
#include "imgsensor_clk.h"

struct IMGSENSOR {
	struct IMGSENSOR_HW     hw;
	struct IMGSENSOR_CLK    clk;
	struct IMGSENSOR_SENSOR sensor[IMGSENSOR_SENSOR_IDX_MAX_NUM];
	atomic_t imgsensor_open_cnt;
};

MINT32
imgsensor_sensor_open(struct IMGSENSOR_SENSOR *psensor);

MINT32
imgsensor_sensor_close(struct IMGSENSOR_SENSOR *psensor);

MUINT32
imgsensor_sensor_get_info(
	struct IMGSENSOR_SENSOR *psensor,
	MUINT32 ScenarioId,
	MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
	MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData);

MUINT32
imgsensor_sensor_get_resolution(
	struct IMGSENSOR_SENSOR *psensor,
	MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution);

MUINT32
imgsensor_sensor_feature_control(
	struct IMGSENSOR_SENSOR *psensor,
	MSDK_SENSOR_FEATURE_ENUM FeatureId,
	MUINT8 *pFeaturePara,
	MUINT32 *pFeatureParaLen);

MUINT32
imgsensor_sensor_control(
	struct IMGSENSOR_SENSOR *psensor,
	MSDK_SCENARIO_ID_ENUM ScenarioId);

extern unsigned int mt_get_ckgen_freq(int ID);

#endif

