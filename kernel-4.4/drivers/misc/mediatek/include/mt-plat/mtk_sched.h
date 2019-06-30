#include <linux/notifier.h>

#ifdef CONFIG_MTK_SCHED_RQAVG_US
extern unsigned int sched_get_percpu_load(int cpu, bool reset, bool use_maxfreq);

extern unsigned int sched_get_nr_heavy_task(void);

extern unsigned int sched_get_nr_heavy_task_by_threshold(unsigned int threshold);
#endif /* CONFIG_MTK_SCHED_RQAVG_US */

#ifdef CONFIG_MTK_SCHED_CPULOAD
extern unsigned int sched_get_cpu_load(int cpu);
#endif

int register_sched_hint_notifier(struct notifier_block *nb);
int unregister_sched_hint_notifier(struct notifier_block *nb);

enum sched_status_t {
	SCHED_STATUS_INIT,
	SCHED_STATUS_OVERUTIL,
	SCHED_STATUS_UNDERUTIL
};

enum {
	LT_WALT_POWERHAL  = 0,
	LT_WALT_FPSGO,
	LT_WALT_SCHED,
	LT_WALT_DEBUG,
	LT_UNKNOWN_USER
};

extern int sched_walt_enable(int user, int en);
