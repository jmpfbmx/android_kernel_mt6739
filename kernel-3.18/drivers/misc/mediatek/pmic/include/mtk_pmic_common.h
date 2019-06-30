
#ifndef _MTK_PMIC_COMMON_H_
#define _MTK_PMIC_COMMON_H_

#if !defined CONFIG_HAS_WAKELOCKS
#define pmic_wake_lock(lock)	__pm_stay_awake(lock)
#define pmic_wake_unlock(lock)	__pm_relax(lock)
extern struct wakeup_source pmicThread_lock;
#else
#define pmic_wake_lock(lock)	wake_lock(lock)
#define pmic_wake_unlock(lock)	wake_unlock(lock)
extern struct wake_lock pmicThread_lock;
#endif

#endif	/* _MTK_PMIC_COMMON_H_ */
