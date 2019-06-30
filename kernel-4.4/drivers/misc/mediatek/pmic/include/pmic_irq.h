
#ifndef __PMIC_IRQ_H
#define __PMIC_IRQ_H

#include "mtk_pmic_irq.h"

#ifndef PMIC_INT_WIDTH
#define PMIC_INT_WIDTH 16
#endif

#define PMIC_S_INT_GEN(_name)	\
	{	\
		.name =  #_name,	\
	}

struct pmic_irq_dbg_st {
	unsigned int dbg_id;
};

/* pmic irq extern functions */
extern void PMIC_EINT_SETTING(void);
extern int pmic_irq_debug_init(struct dentry *);
void buck_oc_detect(void);

#endif /*--PMIC_IRQ_H--*/
