
#ifndef __MT_SLEEP_H__
#define __MT_SLEEP_H__

#if defined(CONFIG_MACH_MT6757) || defined(CONFIG_MACH_KIBOPLUS)

#include "spm_v2/mtk_sleep.h"

#elif defined(CONFIG_MACH_MT6799) \
	|| defined(CONFIG_MACH_MT6758) \
	|| defined(CONFIG_MACH_MT6759) \
	|| defined(CONFIG_MACH_MT6775)

#include "spm_v3/mtk_sleep.h"

#elif defined(CONFIG_MACH_MT6763) || defined(CONFIG_MACH_MT6739) || defined(CONFIG_MACH_MT6771)

#include "spm_v4/mtk_sleep.h"

#endif

#endif /* __MT_SLEEP_H__ */

