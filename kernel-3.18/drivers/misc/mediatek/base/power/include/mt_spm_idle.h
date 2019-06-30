
#ifndef __MT_SPM_IDLE__H___
#define __MT_SPM_IDLE__H___

#if defined(CONFIG_ARCH_MT6755) || defined(CONFIG_ARCH_MT6757) || defined(CONFIG_ARCH_MT6797)

#include "spm_v2/mt_spm_idle.h"

#elif defined(CONFIG_ARCH_ELBRUS)

#include "spm_v3/mt_spm_idle.h"

#endif

#endif /* __MT_SPM_IDLE__H___ */

