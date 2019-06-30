
#include <mt-plat/upmu_common.h>
#include <mt-plat/mtk_chip.h>

#include <linux/io.h>
#include <linux/io.h>
#include <linux/of_address.h>
#include "include/pmic.h"

void PMIC_INIT_SETTING_V1(void)
{
	unsigned int chip_version = 0;

	chip_version = pmic_get_register_value(PMIC_SWCID);

	PMIC_check_battery(); /*--update is_battery_remove--*/
	PMIC_check_wdt_status(); /*--update is_wdt_reboot_pmic/chk--*/
	/*--------------------------------------------------------*/
	if (!PMIC_check_pwrhold_status())
		PMIC_POWER_HOLD(1);

	PMICLOG("[PMIC] PMIC Chip = 0x%x,is_battery_remove =%d,is_wdt_reboot=%d\n",
		chip_version, is_battery_remove, is_wdt_reboot_pmic);

	PMIC_LP_INIT_SETTING();
#ifdef CONFIG_MTK_PMIC_CHIP_MT6355
	PMIC_MD_INIT_SETTING_V1();
#endif
	/*PMIC_PWROFF_SEQ_SETTING();*/
}
