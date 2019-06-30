
#include <linux/module.h>       /* needed by all modules */
#include <mt-plat/sync_write.h>
#include "sspm_define.h"


#if SSPM_EMI_PROTECTION_SUPPORT
#include <emi_mpu.h>

void sspm_set_emi_mpu(phys_addr_t base, phys_addr_t size)
{
	unsigned long long shr_mem_phy_start, shr_mem_phy_end;
	int shr_mem_mpu_id;
	unsigned long long shr_mem_mpu_attr;

	shr_mem_mpu_id = SSPM_MPU_REGION_ID;
	shr_mem_phy_start = base;
	shr_mem_phy_end = base + size - 1;
	shr_mem_mpu_attr = SET_ACCESS_PERMISSON(UNLOCK,
			FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN,
			FORBIDDEN, FORBIDDEN, FORBIDDEN, NO_PROTECTION,
			FORBIDDEN, FORBIDDEN, FORBIDDEN, FORBIDDEN,
			FORBIDDEN, FORBIDDEN, FORBIDDEN, NO_PROTECTION);
	pr_debug("[SSPM] MPU Start protect SSPM Share region<%d:%08llx:%08llx> %llx\n",
			shr_mem_mpu_id, shr_mem_phy_start, shr_mem_phy_end, shr_mem_mpu_attr);

	emi_mpu_set_region_protection(
			shr_mem_phy_start,  /*START_ADDR */
			shr_mem_phy_end,    /*END_ADDR */
			shr_mem_mpu_id,     /*region */
			shr_mem_mpu_attr);

}
#endif
