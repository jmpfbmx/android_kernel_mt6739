
#ifndef __MTK_SPM_RESOURCE_REQ_H__
#define __MTK_SPM_RESOURCE_REQ_H__

/* SPM resource request APIs: public */

#if defined(CONFIG_ARCH_MT6755) || defined(CONFIG_ARCH_MT6797)

#elif defined(CONFIG_ARCH_MT6735) || defined(CONFIG_ARCH_MT6735M) || defined(CONFIG_ARCH_MT6753)

#elif defined(CONFIG_ARCH_MT6580)

#elif defined(CONFIG_MACH_KIBOPLUS) || defined(CONFIG_MACH_MT6757)

#include "spm_v2/mtk_spm_resource_req.h"

#elif defined(CONFIG_MACH_MT6799) \
	|| defined(CONFIG_MACH_MT6758) \
	|| defined(CONFIG_MACH_MT6759) \
	|| defined(CONFIG_MACH_MT6775)

#include "spm_v3/mtk_spm_resource_req.h"

#elif defined(CONFIG_MACH_MT6763) \
	|| defined(CONFIG_MACH_MT6739) \
	|| defined(CONFIG_MACH_MT6771)

#include "spm_v4/mtk_spm_resource_req.h"

#endif

#endif /* __MTK_SPM_RESOURCE_REQ_H__ */
