
#ifndef __MTK_CHARGER_INTF_H
#define __MTK_CHARGER_INTF_H

#include <linux/i2c.h>
#include <linux/types.h>
#include <mt-plat/charging.h>

struct mtk_charger_info;

/* MTK charger interface */
typedef int (*mtk_charger_intf)(struct mtk_charger_info *mchr_info, void *data);

struct mtk_charger_info {
	const mtk_charger_intf *mchr_intf;
	const char *name;
	u8 device_id;
};

extern void mtk_charger_set_info(struct mtk_charger_info *info);

extern int mtk_charger_sw_init(struct mtk_charger_info *info, void *data);
extern int mtk_charger_set_hv_threshold(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_hv_status(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_battery_status(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_charger_det_status(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_charger_type(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_is_pcm_timer_trigger(struct mtk_charger_info *info, void *data);
extern int mtk_charger_set_platform_reset(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_platform_boot_mode(struct mtk_charger_info *info, void *data);
extern int mtk_charger_set_power_off(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_power_source(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_csdac_full_flag(struct mtk_charger_info *info, void *data);
extern int mtk_charger_diso_init(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_diso_state(struct mtk_charger_info *info, void *data);
extern int mtk_charger_set_vbus_ovp_en(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_bif_vbat(struct mtk_charger_info *info, void *data);
extern int mtk_charger_set_chrind_ck_pdn(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_bif_tbat(struct mtk_charger_info *info, void *data);
extern int mtk_charger_set_dp(struct mtk_charger_info *info, void *data);
extern int mtk_charger_get_bif_is_exist(struct mtk_charger_info *info, void *data);

#endif /* __MTK_CHARGER_INTF_H */
