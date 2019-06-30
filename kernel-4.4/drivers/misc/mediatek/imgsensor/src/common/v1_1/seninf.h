
#ifndef __SENINF_H__
#define __SENINF_H__

#include "seninf_cfg.h"
#include "seninf_clk.h"

#define SENINF_DEV_NAME "seninf"

struct SENINF {
	dev_t dev_no;
	struct cdev *pchar_dev;
	struct class *pclass;

	struct SENINF_CLK clk;

	void __iomem *pseninf_base[SENINF_MAX_NUM];
};

#endif

