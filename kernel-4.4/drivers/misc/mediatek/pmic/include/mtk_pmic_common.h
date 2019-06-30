
#ifndef _MTK_PMIC_COMMON_H_
#define _MTK_PMIC_COMMON_H_

#include <linux/device.h>
#include <linux/wakelock.h>

#define PMIC_ENTRY(reg) {reg, reg##_ADDR, reg##_MASK, reg##_SHIFT}

#if !defined CONFIG_HAS_WAKELOCKS
#define pmic_init_wake_lock(lock, name)	wakeup_source_init(lock, name)
#define pmic_wake_lock(lock)	__pm_stay_awake(lock)
#define pmic_wake_unlock(lock)	__pm_relax(lock)
extern struct wakeup_source pmicThread_lock;
#else
#define pmic_init_wake_lock(lock, name)	wake_lock_init(lock, WAKE_LOCK_SUSPEND, name)
#define pmic_wake_lock(lock)	wake_lock(lock)
#define pmic_wake_unlock(lock)	wake_unlock(lock)
extern struct wake_lock pmicThread_lock;
#endif

#endif	/* _MTK_PMIC_COMMON_H_ */
