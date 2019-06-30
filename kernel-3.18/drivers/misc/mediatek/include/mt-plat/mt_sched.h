
#ifdef CONFIG_MTK_SCHED_RQAVG_US
extern unsigned int sched_get_percpu_load(int cpu, bool reset, bool use_maxfreq);

extern unsigned int sched_get_nr_heavy_task(void);

extern unsigned int sched_get_nr_heavy_task_by_threshold(unsigned int threshold);
#endif /* CONFIG_MTK_SCHED_RQAVG_US */

