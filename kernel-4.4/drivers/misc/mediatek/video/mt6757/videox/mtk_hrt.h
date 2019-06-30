
#ifndef __MTK_HRT_H__
#define __MTK_HRT_H__

#include "disp_session.h"
#include "disp_lcm.h"
#include "disp_drv_log.h"
#include "primary_display.h"
#include "disp_drv_platform.h"
#include "display_recorder.h"

#define HRT_UNIT_BPP	4
#define HRT_UNIT_FPS	60

/* LP4 limiter simulation: mm_clk=302MHz, FHD */
#define EMI_LP4_FHD_ULPM_BOUND	(4 * HRT_UNIT_BPP)
#define EMI_LP4_FHD_LPM_BOUND	(8 * HRT_UNIT_BPP)
#define EMI_LP4_FHD_HPM_BOUND	(9 * HRT_UNIT_BPP)

#define EMI_LP4_FHDP_ULPM_BOUND	(4 * HRT_UNIT_BPP)
#define EMI_LP4_FHDP_LPM_BOUND	(7 * HRT_UNIT_BPP)
#define EMI_LP4_FHDP_HPM_BOUND	(9 * HRT_UNIT_BPP)

#define EMI_LP4_HD_ULPM_BOUND	(9 * HRT_UNIT_BPP)
#define EMI_LP4_HD_LPM_BOUND	(18 * HRT_UNIT_BPP)
#define EMI_LP4_HD_HPM_BOUND	(20 * HRT_UNIT_BPP)

#define EMI_LP4_HDP_ULPM_BOUND	(9 * HRT_UNIT_BPP)
#define EMI_LP4_HDP_LPM_BOUND	(15 * HRT_UNIT_BPP)
#define EMI_LP4_HDP_HPM_BOUND	(20 * HRT_UNIT_BPP)

/* LP3 limiter */
#define EMI_LP3_FHD_ULPM_BOUND	(4 * HRT_UNIT_BPP)
#define EMI_LP3_FHD_LPM_BOUND	(4 * HRT_UNIT_BPP)
#define EMI_LP3_FHD_HPM_BOUND	(6 * HRT_UNIT_BPP)

#define EMI_LP3_FHDP_ULPM_BOUND	(2 * HRT_UNIT_BPP)
#define EMI_LP3_FHDP_LPM_BOUND	(2 * HRT_UNIT_BPP)
#define EMI_LP3_FHDP_HPM_BOUND	(4 * HRT_UNIT_BPP)

#define EMI_LP3_HD_ULPM_BOUND	(9 * HRT_UNIT_BPP)
#define EMI_LP3_HD_LPM_BOUND	(9 * HRT_UNIT_BPP)
#define EMI_LP3_HD_HPM_BOUND	(13 * HRT_UNIT_BPP)

#define EMI_LP3_HDP_ULPM_BOUND	(4 * HRT_UNIT_BPP + HRT_UNIT_BPP/2)
#define EMI_LP3_HDP_LPM_BOUND	(4 * HRT_UNIT_BPP + HRT_UNIT_BPP/2)
#define EMI_LP3_HDP_HPM_BOUND	(9 * HRT_UNIT_BPP)



#define LARB_LOWER_BOUND	(3 * HRT_UNIT_BPP)
#define LARB_UPPER_BOUND	(4 * HRT_UNIT_BPP)
#define OD_EMI_LOWER_BOUND	(3 * HRT_UNIT_BPP)
#define OD_EMI_UPPER_BOUND	(4 * HRT_UNIT_BPP)
#define OD_LARB_LOWER_BOUND	(2 * HRT_UNIT_BPP)
#define OD_LARB_UPPER_BOUND	(3 * HRT_UNIT_BPP)




#define PRIMARY_HW_OVL_LAYER_NUM			(4)
#define EXTERNAL_HW_OVL_LAYER_NUM			(2)
#define EXTERNAL_HW_OVL_2L_LAYER_NUM			(2)
#define DISP_HW_OVL0_EXT_LAYER_NUM			(3)
#ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
#define PRIMARY_HW_OVL_2L_LAYER_NUM			(1)
#define DISP_HW_OVL0_2L_EXT_LAYER_NUM			(2)
#else
#define PRIMARY_HW_OVL_2L_LAYER_NUM			(2)
#define DISP_HW_OVL0_2L_EXT_LAYER_NUM			(3)
#endif


#ifdef CONFIG_MTK_DISPLAY_120HZ_SUPPORT
#define HRT_LEVEL(id) ((id)&0xff)
#define HRT_FPS(id) (((id)>>8)&0xff)
#define MAKE_HRT_NUM(fps, level) (unsigned int)((fps)<<8 | (level))
#endif

/* #define HRT_DEBUG */

int dispsys_hrt_calc(struct disp_layer_info *disp_info);

struct hrt_sort_entry {
	struct hrt_sort_entry *head, *tail;
	struct layer_config *layer_info;
	int key;
	int overlap_w;
};

enum HRT_LEVEL {
	HRT_LEVEL_DEFAULT,
	HRT_LEVEL_EXTREME_LOW,
	HRT_LEVEL_LOW,
	HRT_LEVEL_HIGH,
	HRT_OVER_LIMIT,
};

enum HRT_DISP_TYPE {
	HRT_PRIMARY = 0,
	HRT_SECONDARY,
};

extern int hdmi_get_dev_info(int is_sf, void *info);

#endif
