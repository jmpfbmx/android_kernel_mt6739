
#ifndef __MTK_MCDI_PROFILE_H__
#define __MTK_MCDI_PROFILE_H__

enum {
	MCDI_PROF_FLAG_STOP,
	MCDI_PROF_FLAG_START,
	MCDI_PROF_FLAG_POLLING,
	NF_MCDI_PROF_FLAG
};

enum {
	MCDI_PROFILE_ENTER = 0,
	MCDI_PROFILE_MCDI_GOVERNOR_SELECT_LEAVE,
	MCDI_PROFILE_CPU_DORMANT_ENTER,
	MCDI_PROFILE_CPU_DORMANT_LEAVE,
	MCDI_PROFILE_LEAVE,
	NF_MCDI_PROFILE
};

void set_mcdi_profile_target_cpu(int cpu);
void set_mcdi_profile_sampling(int count);
void mcdi_profile_ts(int idx);
void mcdi_profile_calc(void);

int get_mcdi_profile_cpu(void);
unsigned int get_mcdi_profile_cnt(void);
unsigned int get_mcdi_profile_sum_us(int idx);

#endif /* __MTK_MCDI_PROFILE_H__ */
