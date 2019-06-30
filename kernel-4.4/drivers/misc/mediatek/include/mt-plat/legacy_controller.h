
#include <mach/mtk_ppm_api.h>

#define PPM_KIR_PERF  0
#define PPM_KIR_FBC   1
#define PPM_KIR_WIFI  2
#define PPM_KIR_BOOT  3
#define PPM_KIR_TOUCH 4
#define PPM_MAX_KIR   5

extern unsigned int mt_cpufreq_get_freq_by_idx(int id, int idx);
int update_userlimit_cpu_freq(int kicker, int num_cluster, struct ppm_limit_data *freq_limit);
int update_userlimit_cpu_core(int kicker, int num_cluster, struct ppm_limit_data *core_limit);
