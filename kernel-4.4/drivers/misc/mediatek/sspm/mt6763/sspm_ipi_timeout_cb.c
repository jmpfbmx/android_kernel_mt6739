
#include <linux/module.h>       /* needed by all modules */
#include <mt-plat/sync_write.h>
#include "sspm_define.h"

/* debug API */
__weak void dump_emi_outstanding(void) {}
__weak void mtk_spm_dump_debug_info(void) {}
__weak void usb_dump_debug_register(void) {}

/* platform callback when ipi timeout */
void sspm_ipi_timeout_cb(int ipi_id)
{
	/* for debug EMI use */
	pr_info("%s: dump_emi_outstanding\n", __func__);
	dump_emi_outstanding();

	/* for debug SPM */
	pr_info("%s: mtk_spm_dump_debug_info\n", __func__);
	mtk_spm_dump_debug_info();

	/* for debug USB */
	pr_info("%s: usb_dump_debug_register\n", __func__);
	usb_dump_debug_register();

}

