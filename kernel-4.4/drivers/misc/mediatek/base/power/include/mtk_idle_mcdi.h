
#ifndef __MTK_IDLE_MCDI_COMMON_H__
#define __MTK_IDLE_MCDI_COMMON_H__

#if defined(CONFIG_MACH_MT6763) || defined(CONFIG_MACH_MT6739) \
	|| defined(CONFIG_MACH_MT6771)

#include "spm_v4/mtk_idle_mcdi.h"

#elif defined(CONFIG_MACH_MT6758) || defined(CONFIG_MACH_MT6775)

#include "spm_v3/mtk_idle_mcdi.h"

#endif

#endif /* __MTK_IDLE_MCDI_COMMON_H__ */

