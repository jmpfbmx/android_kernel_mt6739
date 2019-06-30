
#define CGROUP_ROOT 0
#define CGROUP_FG 1
#define CGROUP_BG 2
#define CGROUP_TA 3
#define CGROUP_RT 4
#define NR_CGROUP 5

#define EAS_KIR_PERF  0
#define EAS_KIR_FBC   1
#define EAS_KIR_BOOT  2
#define EAS_KIR_TOUCH 3
#define EAS_MAX_KIR   4

extern int boost_write_for_perf_idx(int, int);
int update_eas_boost_value(int kicker, int cgroup_idx, int value);
void init_perfmgr_eas_controller(void);
