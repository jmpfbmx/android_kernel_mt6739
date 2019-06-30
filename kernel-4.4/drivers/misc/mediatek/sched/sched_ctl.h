

#ifdef CONFIG_MTK_SCHED_BOOST
/* For multi-scheduling boost support */
enum {
	SCHED_NO_BOOST = 0,
	SCHED_ALL_BOOST,
	SCHED_FG_BOOST,
	SCHED_FORCE_BOOST,
	SCHED_FORCE_STOP,
	SCHED_UNKNOWN_BOOST
};

extern void set_user_space_global_cpuset(struct cpumask *global_cpus, int cgroup_id);
extern void unset_user_space_global_cpuset(int cgroup_id);
extern int sched_scheduler_switch(SCHED_LB_TYPE new_sched);
int set_sched_boost(unsigned int val);
#endif

extern int idle_prefer_mode;
extern bool idle_prefer_need(void);

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDPLUS
extern void temporary_dvfs_down_throttle_change(int change, unsigned long new_throttle);
#endif

#ifdef CONFIG_SCHED_TUNE
extern int prefer_idle_for_perf_idx(int idx, int prefer_idle);
#endif

extern int display_set_wait_idle_time(unsigned int wait_idle_time);
