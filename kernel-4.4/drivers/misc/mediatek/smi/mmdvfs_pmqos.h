
#ifndef __MMDVFS_PMQOS_H__
#define __MMDVFS_PMQOS_H__

#define MAX_FREQ_STEP 6

#ifdef CONFIG_MTK_QOS_SUPPORT
int mmdvfs_qos_get_freq_steps(u32 pm_qos_class, u64 *freq_steps, u32 *step_size);

int mmdvfs_qos_force_step(int step);

void mmdvfs_qos_enable(bool enable);

u64 mmdvfs_qos_get_freq(u32 pm_qos_class);

#else
static inline int mmdvfs_qos_get_freq_steps(u32 pm_qos_class,
	u64 *freq_steps, u32 *step_size)
	{ *step_size = 0; return 0; }
static inline int mmdvfs_qos_force_step(int step)
	{ return 0; }
static inline void mmdvfs_qos_enable(bool enable)
	{ return; }
static inline u64 mmdvfs_qos_get_freq(u32 pm_qos_class)
	{ return 0; }
#endif
#endif /* __MMDVFS_PMQOS_H__ */
