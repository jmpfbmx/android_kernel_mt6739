
#ifndef _REGULATOR_CODEGEN_H_
#define _REGULATOR_CODEGEN_H_

#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/regulator/of_regulator.h>
#include <linux/of_device.h>
#include <linux/of_fdt.h>
#endif
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/consumer.h>

#include <mach/upmu_hw.h>
#include <mach/upmu_sw.h>

#ifdef CONFIG_MTK_PMIC_CHIP_MT6335
#include "mt6335/mt_regulator_codegen.h"
#endif

#ifdef CONFIG_MTK_PMIC_CHIP_MT6353
#include "mt6353/mt_regulator_codegen.h"
#endif

extern int mtk_regulator_enable(struct regulator_dev *rdev);
extern int mtk_regulator_disable(struct regulator_dev *rdev);
extern int mtk_regulator_is_enabled(struct regulator_dev *rdev);
extern int mtk_regulator_set_voltage_sel(struct regulator_dev *rdev, unsigned selector);
extern int mtk_regulator_get_voltage_sel(struct regulator_dev *rdev);
extern int mtk_regulator_list_voltage(struct regulator_dev *rdev, unsigned selector);

#endif				/* _REGULATOR_CODEGEN_H_ */
