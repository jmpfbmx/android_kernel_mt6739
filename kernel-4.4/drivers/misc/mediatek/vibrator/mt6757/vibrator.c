

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <mt-plat/upmu_common.h>
#include "vibrator.h"

struct vibrator_hw *pvib_cust;

static int debug_enable_vib_hal = 1;
/* #define pr_fmt(fmt) "[vibrator]"fmt */
#define VIB_DEBUG(format, args...) do { \
	if (debug_enable_vib_hal) {\
		pr_debug(format, ##args);\
	} \
} while (0)

void vibr_Enable_HW(void)
{
#if defined(CONFIG_MTK_PMIC_CHIP_MT6355)
	pmic_set_register_value(PMIC_RG_LDO_VGP2_SW_OP_EN, 1);
	pmic_set_register_value(PMIC_RG_LDO_VGP2_EN, 1);
#else
	pmic_set_register_value(PMIC_RG_VIBR_EN, 1);	/* [bit 1]: VIBR_EN,  1=enable */
#endif
}

void vibr_Disable_HW(void)
{
#if defined(CONFIG_MTK_PMIC_CHIP_MT6355)
	pmic_set_register_value(PMIC_RG_LDO_VGP2_EN, 0);
	pmic_set_register_value(PMIC_RG_LDO_VGP2_SW_OP_EN, 0);
#else
	pmic_set_register_value(PMIC_RG_VIBR_EN, 0);	/* [bit 1]: VIBR_EN,  1=enable */
#endif
}

void init_vibr_oc_handler(void (*vibr_oc_func)(void))
{
	/* add corresponding interrupt registration */
	/* Ex: pmic_register_interrupt_callback(INT_VIBR_OC, vibr_oc_func); */
}

void init_cust_vibrator_dtsi(struct platform_device *pdev)
{
	int ret;

	if (pvib_cust == NULL) {
		pvib_cust = kmalloc(sizeof(struct vibrator_hw), GFP_KERNEL);
		if (pvib_cust == NULL) {
			VIB_DEBUG("%s kmalloc fail\n", __func__);
			return;
		}
		ret = of_property_read_u32(pdev->dev.of_node, "vib_timer", &(pvib_cust->vib_timer));
		if (!ret)
			VIB_DEBUG("The vibrator timer from dts is : %d\n", pvib_cust->vib_timer);
		else
			pvib_cust->vib_timer = 25;
#ifdef CUST_VIBR_LIMIT
		ret = of_property_read_u32(pdev->dev.of_node, "vib_limit", &(pvib_cust->vib_limit));
		if (!ret)
			VIB_DEBUG("The vibrator limit from dts is : %d\n", pvib_cust->vib_limit);
		else
			pvib_cust->vib_limit = 9;
#endif

#ifdef CUST_VIBR_VOL
		ret = of_property_read_u32(pdev->dev.of_node, "vib_vol", &(pvib_cust->vib_vol));
		if (!ret)
			VIB_DEBUG("The vibrator vol from dts is : %d\n", pvib_cust->vib_vol);
		else
			pvib_cust->vib_vol = 0x05;
#endif
		VIB_DEBUG("pvib_cust = %d, %d, %d\n", pvib_cust->vib_timer, pvib_cust->vib_limit, pvib_cust->vib_vol);
	}
}

struct vibrator_hw *get_cust_vibrator_dtsi(void)
{
	if (pvib_cust == NULL)
		VIB_DEBUG("get_cust_vibrator_dtsi fail, pvib_cust is NULL\n");
	return pvib_cust;
}

void vibr_power_set(void)
{
#ifdef CUST_VIBR_VOL
	struct vibrator_hw *hw = get_cust_vibrator_dtsi();

	if (hw != NULL) {
		VIB_DEBUG("vibr_init: vibrator set voltage = %d\n", hw->vib_vol);
#if defined(CONFIG_MTK_PMIC_CHIP_MT6355)
		pmic_set_register_value(PMIC_RG_VGP2_VOSEL, hw->vib_vol);
#else
		pmic_set_register_value(PMIC_RG_VIBR_VOSEL, hw->vib_vol);
#endif /* CONFIG_MTK_PMIC_CHIP_MT6355 */
	} else {
		VIB_DEBUG("vibr_init: can not get vibrator settings from dtsi!\n");
	}
#endif
}

struct vibrator_hw *mt_get_cust_vibrator_hw(void)
{
	struct vibrator_hw *hw = get_cust_vibrator_dtsi();
	return hw;
}
