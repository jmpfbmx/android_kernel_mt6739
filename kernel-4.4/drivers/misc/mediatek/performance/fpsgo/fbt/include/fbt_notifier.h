
#ifndef __FBT_NOTIFIER_H__
#define __FBT_NOTIFIER_H__

extern void fbt_notifier_exit(void);
extern int fbt_notifier_init(void);

/* for FBT CPU */
int fbt_notifier_push_cpu_frame_time(
	unsigned long long Q2Q_time,
	unsigned long long Runnging_time);

int fbt_notifier_push_cpu_capability(
	unsigned int curr_cap,
	unsigned int max_cap,
	unsigned int target_fps);

void fbt_notifier_push_game_mode(int is_game_mode);
void fbt_notifier_push_benchmark_hint(int is_benchmark);

/* for DFRC */
int fbt_notifier_push_dfrc_fps_limit(unsigned int fps_limit);

#endif
