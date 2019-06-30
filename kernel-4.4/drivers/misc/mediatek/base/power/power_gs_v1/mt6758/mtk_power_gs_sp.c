
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#include "mtk_power_gs.h"

void mt_power_gs_sp_dump(void)
{
	mt_power_gs_pmic_manual_dump();
}
