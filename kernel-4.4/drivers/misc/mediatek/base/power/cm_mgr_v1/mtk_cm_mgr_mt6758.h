
#ifndef __MTK_CM_MGR_MT6758_H__
#define __MTK_CM_MGR_MT6758_H__

#include <mtk_dramc.h>

#define ATF_SECURE_SMC
/* #define PER_CPU_STALL_RATIO */
#define LIGHT_LOAD
/* #define USE_AVG_PMU */
/* #define DEBUG_CM_MGR */
/* #define USE_TIMER_CHECK */
/* #define USE_IDLE_NOTIFY */
/* #define USE_NEW_CPU_OPP */

#define CM_MGR_EMI_OPP	2
#define CM_MGR_LOWER_OPP 9
#define CM_MGR_CPU_CLUSTER 2
#define CM_MGR_CPU_COUNT 8

#define VCORE_ARRAY_SIZE CM_MGR_EMI_OPP
#define CM_MGR_CPU_ARRAY_SIZE (CM_MGR_CPU_CLUSTER * CM_MGR_EMI_OPP)
#define RATIO_COUNT (100 / 5 - 1)
#define IS_UP 1
#define IS_DOWN 0

#endif	/* __MTK_CM_MGR_MT6758_H__ */
