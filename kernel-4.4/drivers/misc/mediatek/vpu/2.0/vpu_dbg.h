
#ifndef __VPU_DBG_H__
#define __VPU_DBG_H__

extern int g_vpu_log_level;
extern unsigned int g_func_mask;

enum VpuFuncMask {
	VFM_NEED_WAIT_VCORE		= 0x1,
	VFM_ROUTINE_PRT_SYSLOG = 0x2
};

enum VpuLogThre {
	VpuLogThre_PERFORMANCE    = 1,	/* >1, performance break down check */
	VpuLogThre_ALGO_OPP_INFO  = 2,	/* >2, algo info, opp info check */
	VpuLogThre_STATE_MACHINE  = 3,	/* >3, state machine check, while wait vcore/do running */
	VpuLogThre_DUMP_BUF_MVA   = 4	/* >4, dump buffer mva */
};

#endif
