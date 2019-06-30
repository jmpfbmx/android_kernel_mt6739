
#include <linux/kernel.h>
#include <linux/cpuidle.h>
#include <linux/pm_qos.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/tick.h>
#include <linux/sched.h>
#include <linux/math64.h>
#include <linux/module.h>
#include <linux/cpu.h>

struct mtk_idle_device {
	unsigned int        cpu;
	int                 last_state_idx;
};

static DEFINE_PER_CPU(struct mtk_idle_device, mtk_idle_devices);

int __attribute__((weak)) mt_idle_select(int cpu)
{
	/* Default: CPUidle state select failed */
	return -1;
}

void __attribute__((weak)) mt_cpuidle_framework_init(void)
{

}

static int mtk_governor_select(struct cpuidle_driver *drv, struct cpuidle_device *dev)
{
	struct mtk_idle_device *data = &__get_cpu_var(mtk_idle_devices);
	int state;

	state = mt_idle_select(data->cpu);

	return state;
}

static void mtk_governor_reflect(struct cpuidle_device *dev, int index)
{
	/* TODO: implement actions for MTK governor & replace it */
}

static int mtk_governor_enable_device(struct cpuidle_driver *drv,
				struct cpuidle_device *dev)
{
	struct mtk_idle_device *data = &per_cpu(mtk_idle_devices, dev->cpu);

	memset(data, 0, sizeof(struct mtk_idle_device));
	data->cpu = dev->cpu;

	return 0;
}

static struct cpuidle_governor mtk_governor = {
	.name =		"mtk_governor",
	.rating =	100,
	.enable =	mtk_governor_enable_device,
	.select =	mtk_governor_select,
	.reflect =	mtk_governor_reflect,
	.owner =	THIS_MODULE,
};

static int __init init_mtk_governor(void)
{
	/* TODO: check if debugfs_create_file() failed */
	mt_cpuidle_framework_init();
	return cpuidle_register_governor(&mtk_governor);
}

MODULE_LICENSE("GPL");
postcore_initcall(init_mtk_governor);

