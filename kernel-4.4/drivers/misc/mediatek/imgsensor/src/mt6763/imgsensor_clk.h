
#ifndef __IMGSENSOR_CLK_H__
#define __IMGSENSOR_CLK_H__

#include <asm/atomic.h>
#include <linux/platform_device.h>
#include <kd_imgsensor_define.h>
#include "imgsensor_common.h"

extern void mipi_26m_en(unsigned int module_idx, int en);

struct IMGSENSOR_CLK {
	struct clk *mclk_sel[IMGSENSOR_MCLK_MAX_NUM];
	atomic_t    enable_cnt[IMGSENSOR_MCLK_MAX_NUM];
};

enum IMGSENSOR_RETURN imgsensor_clk_init(struct IMGSENSOR_CLK *pclk);
int  imgsensor_clk_set(struct IMGSENSOR_CLK *pclk, ACDK_SENSOR_MCLK_STRUCT *pmclk);
void imgsensor_clk_enable_all(struct IMGSENSOR_CLK *pclk);
void imgsensor_clk_disable_all(struct IMGSENSOR_CLK *pclk);
#endif

