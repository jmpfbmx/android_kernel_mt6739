
#ifndef _MT_GPUFREQ_H
#define _MT_GPUFREQ_H

#include <linux/module.h>

#define MT_GPUFREQ_LOW_BATT_VOLT_PROTECT

#define MT_GPUFREQ_LOW_BATT_VOLUME_PROTECT

/* #define MT_GPUFREQ_OC_PROTECT */

#define MT_GPUFREQ_VCOREFS_ENABLED

#define MT_GPUFREQ_INPUT_BOOST

#define MT_GPUFREQ_DYNAMIC_POWER_TABLE_UPDATE

/* #define MT_GPU_DVFS_RANDOM_TEST */

/* #define MT_GPUFREQ_PERFORMANCE_TEST */

#ifdef CONFIG_MTK_RAM_CONSOLE
#define MT_GPUFREQ_AEE_RR_REC
#endif

struct mt_gpufreq_power_table_info {
	unsigned int gpufreq_khz;
	unsigned int gpufreq_volt;
	unsigned int gpufreq_power;
};


extern u32 get_devinfo_with_index(u32 index);
#ifdef MT_GPUFREQ_AEE_RR_REC
extern void aee_rr_rec_gpu_dvfs_vgpu(u8 val);
extern void aee_rr_rec_gpu_dvfs_oppidx(u8 val);
extern void aee_rr_rec_gpu_dvfs_status(u8 val);
extern u8 aee_rr_curr_gpu_dvfs_status(void);
#endif

/* extern int mt_gpufreq_state_set(int enabled); */
extern unsigned int mt_gpufreq_get_cur_freq_index(void);
extern unsigned int mt_gpufreq_get_cur_freq(void);
extern unsigned int mt_gpufreq_get_cur_volt(void);
extern unsigned int mt_gpufreq_get_dvfs_table_num(void);
extern int mt_gpufreq_target(unsigned int idx);
extern int mt_gpufreq_voltage_enable_set(unsigned int enable);
extern unsigned int mt_gpufreq_get_freq_by_idx(unsigned int idx);
extern void mt_gpufreq_thermal_protect(unsigned int limited_power);
extern unsigned int mt_gpufreq_get_max_power(void);
extern unsigned int mt_gpufreq_get_min_power(void);
extern unsigned int mt_gpufreq_get_thermal_limit_index(void);
extern unsigned int mt_gpufreq_get_thermal_limit_freq(void);
extern void mt_gpufreq_set_power_limit_by_pbm(unsigned int limited_power);
extern unsigned int mt_gpufreq_get_leakage_mw(void);

typedef void (*gpufreq_power_limit_notify) (unsigned int);
extern void mt_gpufreq_power_limit_notify_registerCB(gpufreq_power_limit_notify pCB);

typedef void (*gpufreq_input_boost_notify) (unsigned int);
extern void mt_gpufreq_input_boost_notify_registerCB(gpufreq_input_boost_notify pCB);

typedef void (*sampler_func) (unsigned int);
extern void mt_gpufreq_setfreq_registerCB(sampler_func pCB);
extern void mt_gpufreq_setvolt_registerCB(sampler_func pCB);

#endif
