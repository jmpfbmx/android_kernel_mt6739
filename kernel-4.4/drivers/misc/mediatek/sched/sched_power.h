


#define DEFAULT_TURNINGPOINT 65
#define DEFAULT_WATERSHED 236

#if defined(CONFIG_MACH_MT6799)
/* LL: CA35 */
#define DEFAULT_SODI_LIMIT 400
#else
/* LL: CA53 */
#define DEFAULT_SODI_LIMIT 200
#endif

extern int stune_task_threshold;
extern int sched_dvfs_type;

struct power_tuning_t {
	int turning_point; /* max=100, default: 65% capacity */
	int watershed; /* max=1023 */
};


extern struct power_tuning_t *get_eas_power_setting(void);

/* Game Hint */
extern void (*ged_kpi_set_game_hint_value_fp)(int is_game_mode);

/* Stune */
#ifdef CONFIG_CGROUP_SCHEDTUNE
extern int group_boost_read(int group_idx);
#else
static int group_boost_read(int group_idx) { return 0; }
#endif

#ifdef CONFIG_CPU_FREQ_SCHED_ASSIST
extern int dbg_id;
#endif

#ifdef CONFIG_MTK_SCHED_VIP_TASKS
extern void store_vip(int pid);
extern int get_vip_pid(int idx);
extern int get_vip_ref_count(void);
#endif
