
#ifndef __MTK_MCDI_GOVERNOR_H__
#define __MTK_MCDI_GOVERNOR_H__

enum {
	PAUSE_CNT	= 0,
	MULTI_CORE_CNT,
	RESIDENCY_CNT,
	LAST_CORE_CNT,
	NF_ANY_CORE_CPU_COND_INFO
};

struct mcdi_feature_status {
	bool enable;
	bool pause;
	bool cluster_off;
	bool any_core;
	int s_state;
	unsigned int buck_off;
};

int mcdi_governor_select(int cpu, int cluster_idx);
void mcdi_governor_reflect(int cpu, int state);
void mcdi_avail_cpu_cluster_update(void);
void mcdi_governor_init(void);
void set_mcdi_enable_status(bool enabled);
void set_mcdi_s_state(int state);
void set_mcdi_buck_off_mask(unsigned int buck_off_mask);
void mcdi_enable_buck_off(unsigned int enable);
bool mcdi_is_buck_off(int cluster_idx);
void get_mcdi_feature_status(struct mcdi_feature_status *stat);
void get_mcdi_avail_mask(unsigned int *cpu_mask, unsigned int *cluster_mask);
int get_residency_latency_result(int cpu);
void mcdi_state_pause(bool pause);
void any_core_cpu_cond_get(unsigned long buf[NF_ANY_CORE_CPU_COND_INFO]);
bool is_mcdi_working(void);
unsigned int mcdi_get_gov_data_num_mcusys(void);

void idle_refcnt_inc(void);
void idle_refcnt_dec(void);
int all_cpu_idle_ratio_get(void);
bool is_all_cpu_idle_criteria(void);

#endif /* __MTK_MCDI_GOVERNOR_H__ */
