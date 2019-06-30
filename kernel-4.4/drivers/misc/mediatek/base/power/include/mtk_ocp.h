
#ifndef _MTK_OCP_H_
#define _MTK_OCP_H_

#if defined(CONFIG_MACH_MT6759)

#include "ocp_v1/mtk_ocp.h"

#elif defined(CONFIG_MACH_MT6799)

#include "../mt6799/mtk_ocp.h"

#else

#error NO corresponding project of mtk_ocp.h header file can be found!!!

#endif

#endif /* _MTK_OCP_H_ */

