
#ifndef __MT_PMIC_IRQ_H
#define __MT_PMIC_IRQ_H

#define PMIC_M_INTS_GEN(adr, enA, setA, clearA, interrupt)	\
	{	\
		.address =  adr,	\
		.en =  enA,	\
		.set =  setA,	\
		.clear =  clearA,	\
		.interrupts = interrupt,	\
	}

struct pmic_interrupt_bit {
	const char *name;
	void (*callback)(void);
	unsigned int times;
};

struct pmic_interrupts {
	unsigned int address;
	unsigned int en;
	unsigned int set;
	unsigned int clear;
	struct pmic_interrupt_bit *interrupts;
};

extern struct task_struct *pmic_thread_handle;
extern int pmic_thread_kthread(void *x);
#endif /*--PMIC_IRQ_H--*/
