
#ifndef __DISP_DRV_PLATFORM_H__
#define __DISP_DRV_PLATFORM_H__

#include <linux/dma-mapping.h>
#include <linux/types.h>
#ifdef CONFIG_MTK_LEGACY
/* #include <mach/mt_gpio.h> */
#endif
#include "m4u.h"
/* #include <mach/mt_reg_base.h> */
#ifdef CONFIG_MTK_CLKMGR
#include <mach/mt_clkmgr.h>
#endif
/* #include <mach/mt_irq.h> */
/*#include <board-custom.h>*/
#include "disp_assert_layer.h"
#include <mt-plat/sync_write.h>
#include "ddp_hal.h"
/* #include "ddp_drv.h" */
#include "ddp_path.h"
#include "ddp_ovl.h"


#define ALIGN_TO(x, n)  \
	(((x) + ((n) - 1)) & ~((n) - 1))

/************************Feature options****************************/

#define MTK_NO_CAPTURE_SUPPORT

#define MTK_FB_SODI_SUPPORT

#define MTK_FB_SODI_DEFEATURE

#define MTK_FB_ESD_ENABLE

#define MTK_FB_ION_SUPPORT

#define MTK_DISP_IDLE_LP

/* #define OVL_MULTIPASS_SUPPORT */

/* #define OVL_TIME_SHARING */

#define CONFIG_ALL_IN_TRIGGER_STAGE

/* #define MTKFB_NO_M4U */

/* #define MTK_NO_DISP_IN_LK  // do not enable display in LK */

/* #define MTK_FB_CMDQ_DISABLE */

/* #define MTKFB_FB_BYPASS_PQ */

/* #define FPGA_DEBUG_PAN */

#define MTK_FB_DFO_DISABLE
/* #define DFO_USE_NEW_API */


/************************Display Capabilies****************************/
/* These configurations should not be changed. */

#if defined(CONFIG_FPGA_EARLY_PORTING) || !defined(CONFIG_MTK_GPU_SUPPORT)
#define MTK_FB_ALIGNMENT 16
#else
#define MTK_FB_ALIGNMENT 32
#endif

/* #define OVL_CASCADE_SUPPORT */

#define HW_OVERLAY_COUNT                 (OVL_LAYER_NUM)
#define RESERVED_LAYER_COUNT             (2)
#define VIDEO_LAYER_COUNT                (HW_OVERLAY_COUNT - RESERVED_LAYER_COUNT)
#define PRIMARY_DISPLAY_HW_OVERLAY_LAYER_NUMBER		(4)
#define PRIMARY_DISPLAY_HW_OVERLAY_ENGINE_COUNT		(2)
#ifdef OVL_CASCADE_SUPPORT
#define PRIMARY_DISPLAY_HW_OVERLAY_CASCADE_COUNT	(2)
#else
#define PRIMARY_DISPLAY_HW_OVERLAY_CASCADE_COUNT	(1)
#endif
#define PRIMARY_DISPLAY_SESSION_LAYER_COUNT	(PRIMARY_DISPLAY_HW_OVERLAY_LAYER_NUMBER *	\
							 PRIMARY_DISPLAY_HW_OVERLAY_CASCADE_COUNT)
#define EXTERNAL_DISPLAY_SESSION_LAYER_COUNT	(PRIMARY_DISPLAY_HW_OVERLAY_LAYER_NUMBER *	\
							 PRIMARY_DISPLAY_HW_OVERLAY_CASCADE_COUNT)
#define DISP_SESSION_OVL_TIMELINE_ID(x)		(x)
/* #define DISP_SESSION_OUTPUT_TIMELINE_ID       (PRIMARY_DISPLAY_SESSION_LAYER_COUNT) */
/* #define DISP_SESSION_PRESENT_TIMELINE_ID      (PRIMARY_DISPLAY_SESSION_LAYER_COUNT+1) */
/* #define DISP_SESSION_TIMELINE_COUNT                   (DISP_SESSION_PRESENT_TIMELINE_ID+1) */
typedef enum {
	DISP_SESSION_OUTPUT_TIMELINE_ID = PRIMARY_DISPLAY_SESSION_LAYER_COUNT,
	DISP_SESSION_PRESENT_TIMELINE_ID,
	DISP_SESSION_OUTPUT_INTERFACE_TIMELINE_ID,
	DISP_SESSION_TIMELINE_COUNT,
} DISP_SESSION_ENUM;

#define MAX_SESSION_COUNT 5

/* #define FORCE_SODI_BY_SW */

/* #define MTK_FB_RDMA1_SUPPORT */

#define DISP_HW_MAX_LAYER 4

#ifdef OVL_MULTIPASS_SUPPORT
#define WDMA_PATH_CLOCK_DYNAMIC_SWITCH
#endif

#ifdef CONFIG_MTK_GMO_RAM_OPTIMIZE
	#define DISP_HW_MODE_CAP DISP_OUTPUT_CAP_DIRECT_LINK
	#define DISP_HW_PASS_MODE DISP_OUTPUT_CAP_SINGLE_PASS

	#ifdef OVL_TIME_SHARING
	#undef OVL_TIME_SHARING
	#endif
	#define DISP_INTERNAL_BUFFER_COUNT 1
#else
	#define DISP_HW_MODE_CAP DISP_OUTPUT_CAP_SWITCHABLE
	#ifdef OVL_MULTIPASS_SUPPORT
		#define DISP_HW_PASS_MODE DISP_OUTPUT_CAP_MULTI_PASS
	#else
		#define DISP_HW_PASS_MODE DISP_OUTPUT_CAP_SINGLE_PASS
	#endif
	#define DISP_INTERNAL_BUFFER_COUNT 3
#endif
#ifndef CONFIG_SINGLE_PANEL_OUTPUT
#define DISP_NO_DPI
#endif
#define DISP_NO_MT_BOOT

#define DISP_NO_AEE

#endif /* __DISP_DRV_PLATFORM_H__ */
