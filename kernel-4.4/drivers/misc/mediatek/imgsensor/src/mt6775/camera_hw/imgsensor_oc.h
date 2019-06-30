
#ifndef __IMGSENSOR_OC_H__
#define __IMGSENSOR_OC_H__

#include "imgsensor.h"
#include "imgsensor_common.h"
#include "imgsensor_hw.h"

#if 0
#define IMGSENSOR_OC_ENABLE
#endif

enum IMGSENSOR_RETURN imgsensor_oc_init(void);
enum IMGSENSOR_RETURN imgsensor_oc_interrupt(enum IMGSENSOR_HW_POWER_STATUS pwr_status);

extern struct IMGSENSOR gimgsensor;

#endif

