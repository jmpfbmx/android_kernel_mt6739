
#include "eas_controller.h"
#include "fbt_cpu_platform.h"

void fbt_set_boost_value(unsigned int base_blc)
{
	base_blc = (base_blc << 10) / 100U;
	base_blc = clamp(base_blc, 1U, 1024U);
	capacity_min_write_for_perf_idx(CGROUP_TA, (int)base_blc);
}

void fbt_clear_boost_value(void)
{
	capacity_min_write_for_perf_idx(CGROUP_TA, 0);
}

int fbt_is_mips_different(void)
{
	return 1;
}

int fbt_get_L_min_ceiling(void)
{
	return 1400000;
}

int fbt_get_L_cluster_num(void)
{
	return 0;
}

