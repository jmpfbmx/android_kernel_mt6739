
#include "mali_kbase_cpu_mt6775.h"

#define GPU_FREQ_KHZ_MAX kbase_get_platform_max_freq()
#define GPU_FREQ_KHZ_MIN kbase_get_platform_min_freq()

#define CPU_SPEED_FUNC (&kbase_get_vexpress_cpu_clock_speed)

#define GPU_SPEED_FUNC (NULL)

#define POWER_MANAGEMENT_CALLBACKS (&pm_callbacks)

#define PLATFORM_FUNCS (NULL)

#define PROTECTED_CALLBACKS (NULL)

extern struct kbase_pm_callback_conf pm_callbacks;
