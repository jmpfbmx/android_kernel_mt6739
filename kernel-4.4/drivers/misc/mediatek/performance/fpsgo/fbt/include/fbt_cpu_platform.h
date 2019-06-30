
#ifndef __FBT_CPU_PLATFORM_H__
#define __FBT_CPU_PLATFORM_H__

#include "fbt_cpu.h"

enum fbt_cpu_freq_bound_type {
	FBT_CPU_FREQ_BOUND,
	FBT_CPU_RESCUE_BOUND,
};

void fbt_set_boost_value(int cluster, unsigned int base_blc);
void fbt_init_cpuset_freq_bound_table(void);

extern int cluster_num;
extern int cluster_freq_bound[MAX_FREQ_BOUND_NUM];
extern int cluster_rescue_bound[MAX_FREQ_BOUND_NUM];

#endif
