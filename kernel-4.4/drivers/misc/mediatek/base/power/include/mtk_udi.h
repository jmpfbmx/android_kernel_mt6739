
#ifndef _MTK_UDI_H_
#define _MTK_UDI_H_

#if defined(CONFIG_MACH_MT6775)

#include "udi_v1/mtk_udi_mt6775.h"

#elif defined(CONFIG_MACH_MT6771)

#include "udi_v1/mtk_udi_mt6771.h"

#elif defined(CONFIG_MACH_MT6758)

#include "udi_v1/mtk_udi_mt6758.h"

#elif defined(CONFIG_MACH_MT6763)

#include "udi_v1/mtk_udi_mt6763.h"

#elif defined(CONFIG_MACH_MT6759)

#include "udi_v1/mtk_udi_mt6759.h"

#elif defined(CONFIG_MACH_MT6799)

#include "../mt6799/mtk_udi.h"

#else

#error NO corresponding project of mtk_udi.h header file can be found!!!

#endif

#endif /* _MTK_UDI_H_ */

