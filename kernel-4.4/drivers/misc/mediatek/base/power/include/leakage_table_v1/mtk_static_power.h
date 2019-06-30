
#ifndef __MTK_STATIC_POWER_H__
#define __MTK_STATIC_POWER_H__

#include <linux/types.h>
#if defined(CONFIG_MACH_MT6799)
#include "mtk_static_power_mt6799.h"
#endif

#if defined(CONFIG_MACH_MT6759)
#include "mtk_static_power_mt6759.h"
#endif

/* #define MTK_SPOWER_UT */
extern u32 get_devinfo_with_index(u32 index);

extern int mt_spower_get_leakage(int dev, unsigned int voltage, int degree);
extern int mt_spower_get_efuse_lkg(int dev);

extern int mt_spower_init(void);


#endif
