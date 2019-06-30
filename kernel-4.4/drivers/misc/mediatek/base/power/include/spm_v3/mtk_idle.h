
#ifndef __MTK_IDLE_H__
#define __MTK_IDLE_H__

#include <linux/notifier.h>

enum idle_lock_spm_id {
	IDLE_SPM_LOCK_VCORE_DVFS = 0,
};
extern void idle_lock_spm(enum idle_lock_spm_id id);
extern void idle_unlock_spm(enum idle_lock_spm_id id);

extern void enable_dpidle_by_bit(int id);
extern void disable_dpidle_by_bit(int id);

extern void enable_soidle3_by_bit(int id);
extern void disable_soidle3_by_bit(int id);

extern void enable_soidle_by_bit(int id);
extern void disable_soidle_by_bit(int id);

extern void enable_mcsodi_by_bit(int id);
extern void disable_mcsodi_by_bit(int id);

#if defined(CONFIG_MACH_MT6759) \
	|| defined(CONFIG_MACH_MT6758) \
	|| defined(CONFIG_MACH_MT6775)
/* return 0: non-active, 1:active */
int dpidle_active_status(void);
#endif

enum spm_idle_notify_id {
	NOTIFY_DPIDLE_ENTER,
	NOTIFY_DPIDLE_LEAVE,
	NOTIFY_SOIDLE_ENTER,
	NOTIFY_SOIDLE_LEAVE,
	NOTIFY_SOIDLE3_ENTER,
	NOTIFY_SOIDLE3_LEAVE,
};

extern int mtk_idle_notifier_register(struct notifier_block *n);
extern void mtk_idle_notifier_unregister(struct notifier_block *n);

extern void idle_lock_by_ufs(unsigned int lock);
extern void idle_lock_by_gpu(unsigned int lock);

#endif
