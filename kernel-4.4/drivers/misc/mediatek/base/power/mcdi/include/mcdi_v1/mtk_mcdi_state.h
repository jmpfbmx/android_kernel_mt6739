
#ifndef __MCDI_STATE_H__
#define __MCDI_STATE_H__

#include <linux/cpuidle.h>

enum {
	MCDI_STATE_TABLE_SET_0      = 0,
	MCDI_STATE_TABLE_SET_1      = 1,
	MCDI_STATE_TABLE_SET_2      = 2,
	NF_MCDI_STATE_TABLE_TYPE    = 3
};

enum mcdi_s_state {
	MCDI_STATE_WFI = 0,
	MCDI_STATE_CPU_OFF,
	MCDI_STATE_CLUSTER_OFF,
	MCDI_STATE_SODI,
	MCDI_STATE_DPIDLE,
	MCDI_STATE_SODI3
};

extern struct cpuidle_driver *mcdi_state_tbl_get(int cpu);

#endif /* __MCDI_STATE_H__ */

