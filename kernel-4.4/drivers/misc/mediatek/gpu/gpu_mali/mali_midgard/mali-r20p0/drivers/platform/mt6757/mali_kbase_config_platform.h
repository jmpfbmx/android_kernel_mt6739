



#define GPU_FREQ_KHZ_MAX (5000)
#define GPU_FREQ_KHZ_MIN (5000)

#define VE_VIRTEX6_GPU_FREQ_MIN 5000
#define VE_VIRTEX6_GPU_FREQ_MAX 5000
#define VE_VIRTEX7_GPU_FREQ_MIN 40000
#define VE_VIRTEX7_GPU_FREQ_MAX 40000

#define CPU_SPEED_FUNC (&kbase_get_vexpress_cpu_clock_speed)

#define GPU_SPEED_FUNC (NULL)

#define POWER_MANAGEMENT_CALLBACKS (&pm_callbacks)

#define PLATFORM_FUNCS (NULL)

#define POWER_MODEL_CALLBACKS (NULL)

#define SECURE_CALLBACKS (NULL)

extern struct kbase_pm_callback_conf pm_callbacks;
