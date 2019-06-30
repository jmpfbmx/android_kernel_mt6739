

enum mt_cpu_dvfs_id {
	MT_CPU_DVFS_LL,
	MT_CPU_DVFS_L,
	MT_CPU_DVFS_B,
	MT_CPU_DVFS_CCI,

	NR_MT_CPU_DVFS,
};

enum mt_dvfs_debug_id {
	DEBUG_FREQ_CLUSTER0,
	DEBUG_FREQ_CLUSTER1,
	DEBUG_FREQ_CLUSTER2,
	DEBUG_FREQ_ALL,
	DEBUG_FREQ_DISABLED = 100,
};

enum throttle_type {
	DVFS_THROTTLE_UP,
	DVFS_THROTTLE_DOWN,
};

#ifdef CONFIG_MTK_BASE_POWER
extern int  mt_cpufreq_set_by_schedule_load_cluster(int cid, unsigned int freq);
extern int  mt_cpufreq_set_by_wfi_load_cluster(int cid, unsigned int freq);
extern unsigned int mt_cpufreq_find_close_freq(unsigned int cluster_id, unsigned int freq);
extern unsigned int mt_cpufreq_get_freq_by_idx(enum mt_cpu_dvfs_id id, int idx);
extern int mt_cpufreq_get_sched_enable(void);
extern unsigned int mt_cpufreq_get_cur_freq(enum mt_cpu_dvfs_id id);
#else
static inline int  mt_cpufreq_set_by_schedule_load_cluster(int cid, unsigned int freq) { return 0; }
static inline int  mt_cpufreq_set_by_wfi_load_cluster(int cid, unsigned int freq) { return 0; }
static inline unsigned int mt_cpufreq_find_close_freq(unsigned int cluster_id, unsigned int freq) { return 0; }
static inline unsigned int mt_cpufreq_get_freq_by_idx(enum mt_cpu_dvfs_id id, int idx) { return 0; }
static inline int mt_cpufreq_get_sched_enable(void) { return 0; }
static inline  int mt_cpufreq_get_cur_freq(enum mt_cpu_dvfs_id id) { return 0; };
#endif

#ifdef CONFIG_SCHED_WALT
extern int walt_cpufreq_notifier_trans(unsigned int cpu, unsigned int new_freq);
#endif

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDPLUS
extern unsigned long int min_boost_freq[3];
extern unsigned long int cap_min_freq[3];
extern void update_cpu_freq_quick(int cpu, int freq);
#else
static inline void update_cpu_freq_quick(int cpu, int freq) { return; }
#endif

#ifdef CONFIG_CGROUP_SCHEDTUNE
extern int schedtune_cpu_capacity_min(int cpu);
#endif
