
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include "mtk_spm.h"
#include "mtk_sleep.h"
#include "mtk_cpuidle.h"
#include "mtk_spm_resource_req_internal.h"

static int __init mtk_spm_init(void)
{
#if defined(CONFIG_MACH_MT6757)
#if !defined(CONFIG_FPGA_EARLY_PORTING)
	/* cpu dormant driver init */
	mtk_cpuidle_init();

	spm_module_init();
	slp_module_init();
#endif
#elif defined(CONFIG_MACH_KIBOPLUS)
	/* cpu dormant driver init */
	/* mt_cpu_dormant_init(); */

	spm_module_init();
	/* slp_module_init(); */
#endif
	spm_resource_req_init();
	return 0;
}

arch_initcall(mtk_spm_init);
