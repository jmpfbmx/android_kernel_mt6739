
#include "eas_controller.h"
#include "fbt_cpu_platform.h"

#define CLUSTER_FREQ 1287000

void fbt_set_boost_value(int cluster, unsigned int base_blc)
{
	if (cluster == 1)
		update_eas_boost_value(EAS_KIR_FBC, CGROUP_TA, (base_blc - 1) + 3200);
	else if (cluster == 0)
		update_eas_boost_value(EAS_KIR_FBC, CGROUP_TA, (base_blc - 1) + 3100);
}

void fbt_init_cpuset_freq_bound_table(void)
{
	switch (cluster_num) {
	case 2:
		cluster_freq_bound[0] = CLUSTER_FREQ;
		cluster_rescue_bound[0] = CLUSTER_FREQ;
		break;
	default:
		break;
	}
}

