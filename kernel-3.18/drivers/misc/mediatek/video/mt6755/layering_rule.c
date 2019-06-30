
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/module.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

#include "layering_rule.h"

static struct layering_rule_ops l_rule_ops;
static struct layering_rule_info_t l_rule_info;

int emi_bound_table[HRT_BOUND_NUM][HRT_LEVEL_NUM] = {
	/* HRT_BOUND_TYPE_NORMAL */
	{4, 6},
	{6, 9},
	{3, 4},
};

int larb_bound_table[HRT_BOUND_NUM][HRT_LEVEL_NUM] = {
	/* HRT_BOUND_TYPE_NORMAL */
	{12, 12},
};

static int layer_mapping_table[HRT_TB_NUM][TOTAL_OVL_LAYER_NUM] = {
	/* HRT_TB_TYPE_GENERAL */
	{0x00010001, 0x00030003, 0x00070007, 0x000F000F, 0x000F001F, 0x000F007F,
	0x000F007F, 0x000F00FF, 0x000F00FF, 0x000F00FF, 0x000F00FF, 0x000F00FF}
};

static int larb_mapping_table[HRT_TB_NUM] = {
	0x00000000,
};

static int ovl_mapping_table[HRT_TB_NUM] = {
	0x000800A8,
};

#define GET_SYS_STATE(sys_state) ((l_rule_info.hrt_sys_state >> sys_state) & 0x1)

static void layering_rule_senario_decision(disp_layer_info *disp_info)
{
	MMProfileLogEx(ddp_mmp_get_events()->hrt, MMProfileFlagStart, l_rule_info.disp_path,
		l_rule_info.layer_tb_idx | (l_rule_info.bound_tb_idx << 16));

	l_rule_info.layer_tb_idx = HRT_TB_TYPE_GENERAL;
	l_rule_info.disp_path = HRT_PATH_GENERAL;

	l_rule_info.primary_fps = 60;
	if (primary_display_get_width() < 800)
		l_rule_info.bound_tb_idx = HRT_BOUND_TYPE_HD;
	else {
		if (primary_display_get_height() <= 1920)
			l_rule_info.bound_tb_idx = HRT_BOUND_TYPE_NORMAL;
		else
			l_rule_info.bound_tb_idx = HRT_BOUND_TYPE_FHD_PLUS;
	}
	MMProfileLogEx(ddp_mmp_get_events()->hrt, MMProfileFlagEnd, l_rule_info.disp_path,
		l_rule_info.layer_tb_idx | (l_rule_info.bound_tb_idx << 16));
}

static int get_hrt_bound(int is_larb, int hrt_level)
{
	if (is_larb)
		return larb_bound_table[l_rule_info.bound_tb_idx][hrt_level];
	else
		return emi_bound_table[l_rule_info.bound_tb_idx][hrt_level];
}

static int *get_bound_table(enum DISP_HW_MAPPING_TB_TYPE tb_type)
{
	switch (tb_type) {
	case DISP_HW_EMI_BOUND_TB:
		return emi_bound_table[l_rule_info.bound_tb_idx];
	case DISP_HW_LARB_BOUND_TB:
		return larb_bound_table[l_rule_info.bound_tb_idx];
	default:
		return NULL;
	}
}

static int get_mapping_table(enum DISP_HW_MAPPING_TB_TYPE tb_type, int param)
{
	switch (tb_type) {
	case DISP_HW_OVL_TB:
		return ovl_mapping_table[l_rule_info.layer_tb_idx];
	case DISP_HW_LARB_TB:
		return larb_mapping_table[l_rule_info.layer_tb_idx];
	case DISP_HW_LAYER_TB:
		return layer_mapping_table[l_rule_info.layer_tb_idx][param];
	default:
		return -1;
	}
}

void layering_rule_init(void)
{
	l_rule_info.primary_fps = 60;
	register_layering_rule_ops(&l_rule_ops, &l_rule_info);
}

static struct layering_rule_ops l_rule_ops = {
	.scenario_decision = layering_rule_senario_decision,
	.get_bound_table = get_bound_table,
	.get_hrt_bound = get_hrt_bound,
	.get_mapping_table = get_mapping_table,
};

