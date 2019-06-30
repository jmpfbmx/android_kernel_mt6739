
#ifndef __MTK_SPM_DPIDLE__H__
#define __MTK_SPM_DPIDLE__H__

#if defined(CONFIG_MACH_MT6757) || defined(CONFIG_MACH_KIBOPLUS)

#include "mtk_spm_dpidle_mt6757.h"

#endif

extern void spm_dpidle_pre_process(void);
extern void spm_dpidle_post_process(void);
extern void spm_deepidle_chip_init(void);

#endif /* __MTK_SPM_DPIDLE__H__ */

