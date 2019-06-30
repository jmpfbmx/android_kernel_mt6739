
#ifndef __FBT_USEDEXT_H__
#define __FBT_USEDEXT_H__

extern int prefer_idle_for_perf_idx(int idx, int prefer_idle);
extern unsigned int mt_cpufreq_get_freq_by_idx(int id, int idx);
extern unsigned int mt_ppm_userlimit_freq_limit_by_others(
		unsigned int cluster);
extern unsigned long get_cpu_orig_capacity(unsigned int cpu);

extern struct workqueue_struct *g_psNotifyWorkQueue;

#endif
