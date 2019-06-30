
#include <linux/kconfig.h>
#include <linux/module.h>
#include <linux/reboot.h>
#include <mrdump.h>
#include <asm/memory.h>
#include <mach/wd_api.h>
#include "mrdump_private.h"

static void mrdump_hw_enable(bool enabled)
{
	if (enabled) {
		mrdump_cblock->enabled = MRDUMP_ENABLE_COOKIE;
		pr_info("%s: mrdump enabled!\n", __func__);
	} else {
		mrdump_cblock->enabled = 0;
		pr_info("%s: mrdump disabled!\n", __func__);
	}
	__inner_flush_dcache_all();
}

static void mrdump_reboot(void)
{
	aee_exception_reboot();
}

const struct mrdump_platform mrdump_v1_platform = {
	.hw_enable = mrdump_hw_enable,
	.reboot = mrdump_reboot
};

int __init mrdump_init(void)
{
	mrdump_cblock_init();
	return mrdump_platform_init(&mrdump_v1_platform);
}
