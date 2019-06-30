
#ifndef __MCDI_STATE_COMMON_H__
#define __MCDI_STATE_COMMON_H__

#if defined(CONFIG_MACH_MT6763) || defined(CONFIG_MACH_MT6758) \
	|| defined(CONFIG_MACH_MT6771) || defined(CONFIG_MACH_MT6775)

#include "mcdi_v1/mtk_mcdi_state.h"

#elif defined(CONFIG_MACH_MT6739)

#include "mcdi_v2/mtk_mcdi_state.h"

#endif

#endif /* __MCDI_STATE_COMMON_H__ */

