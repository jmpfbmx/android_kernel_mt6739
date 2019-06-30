
#ifndef _MT_EEM_TURBO_
#define _MT_EEM_TURBO_

#ifdef __KERNEL__
#if defined(CONFIG_MTK_PMIC_CHIP_MT6353) && defined(CONFIG_MTK_MT6750TT)
	extern unsigned int *get_turbo(unsigned int binLevel, unsigned int binLevelEng);
#endif
#endif
#endif
