
#ifndef _UFS_MTK_PLATFORM_H
#define _UFS_MTK_PLATFORM_H

#include "ufs.h"
#include "ufshcd.h"


#define UFS_MTK_PLATFORM_UFS_HCI_PERF_HEURISTIC

enum {
	REG_UFS_PERICFG             = 0x12c,
	REG_UFS_PERICFG_RST_N_BIT   = 17,
};

enum {
	REG_UNIPRO_SW_RST_SET       = 0x140,
	REG_UNIPRO_SW_RST_SET_BIT   = 4,
	REG_UNIPRO_SW_RST_CLR       = 0x144,
	REG_UNIPRO_SW_RST_CLR_BIT   = 4,

	REG_UFSHCI_SW_RST_SET       = 0x120,
	REG_UFSHCI_SW_RST_SET_BIT   = 14,
	REG_UFSHCI_SW_RST_CLR       = 0x124,
	REG_UFSHCI_SW_RST_CLR_BIT   = 14,

	REG_UFSCPT_SW_RST_SET       = 0x130,
	REG_UFSCPT_SW_RST_SET_BIT   = 15,
	REG_UFSCPT_SW_RST_CLR       = 0x134,
	REG_UFSCPT_SW_RST_CLR_BIT   = 15,
};

enum {
	SW_RST_TARGET_UFSHCI        = 0x1,
	SW_RST_TARGET_UNIPRO        = 0x2,
	SW_RST_TARGET_UFSCPT        = 0x4,
	SW_RST_TARGET_MPHY          = 0x8,
	SW_RST_TARGET_ALL           = (SW_RST_TARGET_UFSHCI | SW_RST_TARGET_UNIPRO | SW_RST_TARGET_UFSCPT),
};

void ufs_mtk_pltfrm_pwr_change_final_gear(struct ufs_hba *hba, struct ufs_pa_layer_attr *final);
int  ufs_mtk_pltfrm_bootrom_deputy(struct ufs_hba *hba);
int  ufs_mtk_pltfrm_deepidle_check_h8(void);
void ufs_mtk_pltfrm_deepidle_leave(void);
void ufs_mtk_pltfrm_deepidle_lock(struct ufs_hba *hba, bool lock);
void ufs_mtk_pltfrm_deepidle_resource_req(struct ufs_hba *hba, unsigned int resource);
int  ufs_mtk_pltfrm_init(void);
int  ufs_mtk_pltfrm_parse_dt(struct ufs_hba *hba);
int  ufs_mtk_pltfrm_res_req(struct ufs_hba *hba, u32 option);
int  ufs_mtk_pltfrm_resume(struct ufs_hba *hba);
int  ufs_mtk_pltfrm_suspend(struct ufs_hba *hba);
void ufs_mtk_pltfrm_gpio_trigger_and_debugInfo_dump(struct ufs_hba *hba);

#ifdef MTK_UFS_HQA
void random_delay(struct ufs_hba *hba);
void wdt_pmic_full_reset(struct ufs_hba *hba);
#endif

#endif /* _UFS_MTK_PLATFORM_H */

