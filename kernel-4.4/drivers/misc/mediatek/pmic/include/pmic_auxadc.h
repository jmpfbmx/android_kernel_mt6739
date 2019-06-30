
#ifndef _PMIC_AUXADC_H_
#define _PMIC_AUXADC_H_

#include <mach/upmu_hw.h>

extern void pmic_auxadc_suspend(void);
extern void pmic_auxadc_resume(void);
extern unsigned short pmic_set_hk_reg_value(PMU_FLAGS_LIST_ENUM flagname, unsigned int val);

/* MT6358 add it */
extern int wk_vbat_cali(int vbat_out, int vthr);

#endif				/* _PMIC_AUXADC_H_ */
