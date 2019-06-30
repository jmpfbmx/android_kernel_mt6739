
#ifndef __MTK_CPUIDLE_MT6799_H__
#define __MTK_CPUIDLE_MT6799_H__

enum {
	KP_IRQ_NR = 0,
	CONN_WDT_IRQ_NR,
	LOWBATTERY_IRQ_NR,
	MD1_WDT_IRQ_NR,
#ifdef CONFIG_MTK_MD3_SUPPORT
#if CONFIG_MTK_MD3_SUPPORT /* Using this to check >0 */
	C2K_WDT_IRQ_NR,
#endif
#endif
	IRQ_NR_MAX
};

#endif
