
#ifndef __MTK_ICCS_H__
#define __MTK_ICCS_H__

#ifdef CONFIG_MTK_ICCS_SUPPORT
extern int iccs_get_target_state(unsigned char *target_power_state_bitmask,
		unsigned char *target_cache_shared_state_bitmask);

extern unsigned int iccs_get_curr_cache_shared_state(void);
extern void iccs_set_cache_shared_state(unsigned int cluster, int state);
extern int iccs_get_shared_cluster_freq(void);

extern int iccs_governor_suspend(void);
extern int iccs_governor_resume(void);

#endif

#endif

