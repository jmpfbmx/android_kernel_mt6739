
#include <linux/bitops.h>

#include <helio-dvfsrc.h>
#include <mt-plat/mtk_devinfo.h>
#include "mtk_dvfsrc_reg.h"
#include "mtk_spm_vcore_dvfs.h"

#include <ext_wd_drv.h>

#if 0
u32 vcore_to_vcore_dvfs_level[VCORE_OPP_NUM] = { BIT(0), BIT(3), BIT(5) };
u32 emi_to_vcore_dvfs_level[EMI_OPP_NUM]     = { BIT(0), BIT(2), BIT(4), BIT(6)};
#endif

__weak int mtk_rgu_cfg_dvfsrc(int enable) { return 0; }

static struct reg_config dvfsrc_init_configs[][128] = {
	/* HELIO_DVFSRC_DRAM_LP4X_2CH */
	{
		{ DVFSRC_LEVEL_LABEL_0_1, 0x00010000 },
		{ DVFSRC_LEVEL_LABEL_2_3, 0x00110010 },
		{ DVFSRC_LEVEL_LABEL_4_5, 0x00220021 },
		{ DVFSRC_LEVEL_LABEL_6_7, 0x00320032 },
		{ DVFSRC_LEVEL_LABEL_8_9, 0x00320032 },
		{ DVFSRC_LEVEL_LABEL_10_11, 0x00320032 },
		{ DVFSRC_LEVEL_LABEL_12_13, 0x00320032 },
		{ DVFSRC_LEVEL_LABEL_14_15, 0x00320032 },

		{ DVFSRC_TIMEOUT_NEXTREQ, 0x0000001E },

		{ DVFSRC_EMI_REQUEST, 0x00390339 },
		{ DVFSRC_EMI_REQUEST2, 0x00000000 },
		{ DVFSRC_EMI_REQUEST3, 0x39393939 },
		{ DVFSRC_VCORE_REQUEST, 0x00390000 },
		{ DVFSRC_VCORE_REQUEST2, 0x39393939 },

		/* ToDo: following initial setting */

		/* { DVFSRC_RSRV_1, 0x00000004 }, */

		{ DVFSRC_QOS_EN, 0x00000000 },
		{ DVFSRC_FORCE, 0x00400000 },
		{ DVFSRC_BASIC_CONTROL, 0x0000007B },
		{ DVFSRC_BASIC_CONTROL, 0x0000017B },

		{ -1, 0 },
	},
	/* HELIO_DVFSRC_DRAM_LP4X_1CH */
	{
		{ -1, 0 },
	},
	/* HELIO_DVFSRC_DRAM_LP3X_1CH */
	{
		{ -1, 0 },
	},
};

static struct reg_config dvfsrc_suspend_configs[][4] = {
	/* HELIO_DVFSRC_DRAM_LP4X_2CH */
	{
		{ DVFSRC_EMI_MD2SPM0, 0x00000000 },
		{ DVFSRC_EMI_MD2SPM1, 0x800080C0 },
		{ DVFSRC_VCORE_MD2SPM0, 0x800080C0 },
		{ -1, 0 },
	},
	/* HELIO_DVFSRC_DRAM_LP4X_1CH */
	{
		{ -1, 0 },
	},
	/* HELIO_DVFSRC_DRAM_LP3X_1CH */
	{
		{ DVFSRC_EMI_MD2SPM0, 0x000000C0 },
		{ DVFSRC_EMI_MD2SPM1, 0x80008000 },
		{ DVFSRC_VCORE_MD2SPM0, 0x800080C0 },
		{ -1, 0 },
	},
};
static struct reg_config dvfsrc_resume_configs[][4] = {
	/* HELIO_DVFSRC_DRAM_LP4X_2CH */
	{
		{ DVFSRC_EMI_MD2SPM0, 0x0000003E },
		{ DVFSRC_EMI_MD2SPM1, 0x800080C0 },
		{ DVFSRC_VCORE_MD2SPM0, 0x800080C0 },
		{ -1, 0 },
	},
	/* HELIO_DVFSRC_DRAM_LP4X_1CH */
	{
		{ -1, 0 },
	},
	/* HELIO_DVFSRC_DRAM_LP3X_1CH */
	{
		{ DVFSRC_EMI_MD2SPM0, 0x0000003E },
		{ DVFSRC_EMI_MD2SPM1, 0x800080C0 },
		{ DVFSRC_VCORE_MD2SPM0, 0x800080C0 },
		{ -1, 0 },
	},
};
void helio_dvfsrc_platform_init(struct helio_dvfsrc *dvfsrc)
{
	dvfsrc->flag = spm_dvfs_flag_init();
	dvfsrc->dram_type = 0; /* __spm_get_dram_type(); */
	/* dvfsrc->dram_issue = get_devinfo_with_index(138) & BIT(8); */
	dvfsrc->init_config = dvfsrc_init_configs[dvfsrc->dram_type];
	dvfsrc->suspend_config = dvfsrc_suspend_configs[dvfsrc->dram_type];
	dvfsrc->resume_config = dvfsrc_resume_configs[dvfsrc->dram_type];

	dvfsrc->vcore_dvs = SPM_VCORE_DVS_EN;
	dvfsrc->ddr_dfs = SPM_DDR_DFS_EN;
	dvfsrc->mm_clk = SPM_MM_CLK_EN;

	mtk_rgu_cfg_dvfsrc(1);
}

int dvfsrc_transfer_to_dram_level(int data)
{
	if (data >= 0x8000)
		return 2;
	else if (data >= 0x4000)
		return 1;
	return 0;
}

int dvfsrc_transfer_to_vcore_level(int data)
{
	if (data >= 800)
		return 1;
	return 0;
}

#if 0
void dvfsrc_set_scp_vcore_request(unsigned int vcore_level)
{
	dvfsrc_set_vcore_request((0x3 << 30), ((vcore_level & 0x3) << 30));
}
#endif

