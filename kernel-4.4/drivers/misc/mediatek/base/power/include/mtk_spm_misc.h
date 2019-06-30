
#ifndef __MTK_SPM_MISC_H___
#define __MTK_SPM_MISC_H___

#if defined(CONFIG_MACH_MT6757) || defined(CONFIG_MACH_KIBOPLUS)

#include "spm_v2/mtk_spm_misc.h"

#elif defined(CONFIG_MACH_MT6799) \
	|| defined(CONFIG_MACH_MT6758) \
	|| defined(CONFIG_MACH_MT6759) \
	|| defined(CONFIG_MACH_MT6775)

#include "spm_v3/mtk_spm_misc.h"

#elif defined(CONFIG_MACH_MT6763) || defined(CONFIG_MACH_MT6739) \
	|| defined(CONFIG_MACH_MT6771)

#include "spm_v4/mtk_spm_misc.h"

#endif

#endif /* __MTK_SPM_MISC_H___ */

