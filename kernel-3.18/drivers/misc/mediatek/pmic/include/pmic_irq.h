
#ifndef __PMIC_IRQ_H
#define __PMIC_IRQ_H

#include "mt_pmic_irq.h"

#define PMIC_INTERRUPT_WIDTH 16

#define PMIC_S_INT_GEN(_name)	\
	{	\
		.name =  #_name,	\
	}

/* pmic irq extern variable */
extern int interrupts_size;
extern struct pmic_interrupts interrupts[];

/* pmic irq extern functions */
extern void PMIC_EINT_SETTING(void);
void buck_oc_detect(void);

#endif /*--PMIC_IRQ_H--*/
