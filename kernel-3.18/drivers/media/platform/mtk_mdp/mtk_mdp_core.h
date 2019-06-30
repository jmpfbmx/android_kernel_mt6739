

#ifndef __MTK_MDP_CORE_H__
#define __MTK_MDP_CORE_H__

#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/pm_runtime.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/videodev2.h>
#include <media/videobuf2-core.h>
#include <media/videobuf2-dma-contig.h>
#include <media/videobuf2-vmalloc.h>
#include <linux/module.h>

#include "mtk_mdp_vpu.h"
#ifdef CONFIG_ARM
#include <asm/dma-iommu.h>
#endif
#include <linux/iommu.h>

#define MTK_MDP_MODULE_NAME		"mtk-mdp"

#define MTK_MDP_SHUTDOWN_TIMEOUT	((100*HZ)/1000)
#define MTK_MDP_MAX_DEVS		4
#define MTK_MDP_MAX_CTRL_NUM		10
#define MTK_MDP_SC_ALIGN_4		4
#define MTK_MDP_SC_ALIGN_2		2
#define DEFAULT_CSC_EQ			1
#define DEFAULT_CSC_RANGE		1

#define MTK_MDP_PARAMS			(1 << 0)
#define MTK_MDP_SRC_FMT			(1 << 1)
#define MTK_MDP_DST_FMT			(1 << 2)
#define MTK_MDP_CTX_M2M			(1 << 3)
#define MTK_MDP_CTX_STOP_REQ		(1 << 6)
#define MTK_MDP_CTX_ABORT		(1 << 7)


enum mtk_mdp_color_fmt {
	MTK_MDP_RGB = 0x1,
	MTK_MDP_YUV420 = 0x2,
	MTK_MDP_YUV422 = 0x3,
	MTK_MDP_YUV444 = 0x4,
	MTK_MDP_YUV420_BLK = 0x5,
};

enum mtk_mdp_yuv_fmt {
	MTK_MDP_LSB_Y = 0x10,
	MTK_MDP_LSB_C,
	MTK_MDP_CBCR = 0x20,
	MTK_MDP_CRCB,
};

#define is_rgb(x) (!!((x) & 0x1))
#define is_yuv420(x) (!!((x) & 0x2))
#define is_yuv422(x) (!!((x) & 0x4))

struct mtk_mdp_fmt {
	char				*name;
	u32				pixelformat;
	u32				color;
	u32				yorder;
	u32				corder;
	u16				num_planes;
	u16				num_comp;
	u8				depth[VIDEO_MAX_PLANES];
	u32				flags;
};

struct mtk_mdp_addr {
	dma_addr_t y;
	dma_addr_t cb;
	dma_addr_t cr;
};

struct mdp_ctrl {
	u32 val;
};

struct mtk_mdp_ctrls {
	struct mdp_ctrl *rotate;
	struct mdp_ctrl *hflip;
	struct mdp_ctrl *vflip;
	struct mdp_ctrl *global_alpha;
};

struct mtk_mdp_rect {
	 u32 left;
	 u32 top;
	 u32 width;
	 u32 height;
};

struct mtk_mdp_frame {
	u32				f_width;
	u32				f_height;
	struct mtk_mdp_rect		crop;
	unsigned long			payload[VIDEO_MAX_PLANES];
	unsigned int			pitch[VIDEO_MAX_PLANES];
	struct mtk_mdp_addr		addr;
	struct mtk_mdp_fmt		*fmt;
	u32				colorspace;
	u8				alpha;
};

struct mtk_mdp_pix_max {
	u16 org_scaler_bypass_w;
	u16 org_scaler_bypass_h;
	u16 org_scaler_input_w;
	u16 org_scaler_input_h;
	u16 real_rot_dis_w;
	u16 real_rot_dis_h;
	u16 real_rot_en_w;
	u16 real_rot_en_h;
	u16 target_rot_dis_w;
	u16 target_rot_dis_h;
	u16 target_rot_en_w;
	u16 target_rot_en_h;
};

struct mtk_mdp_pix_min {
	u16 org_w;
	u16 org_h;
	u16 real_w;
	u16 real_h;
	u16 target_rot_dis_w;
	u16 target_rot_dis_h;
	u16 target_rot_en_w;
	u16 target_rot_en_h;
};

struct mtk_mdp_pix_align {
	u16 org_h;
	u16 org_w;
	u16 offset_h;
	u16 real_w;
	u16 real_h;
	u16 target_w;
	u16 target_h;
};

struct mtk_mdp_variant {
	const struct mtk_mdp_pix_max		*pix_max;
	const struct mtk_mdp_pix_min		*pix_min;
	const struct mtk_mdp_pix_align	*pix_align;
	u16				in_buf_cnt;
	u16				out_buf_cnt;
	u16				h_sc_up_max;
	u16				v_sc_up_max;
	u16				h_sc_down_max;
	u16				v_sc_down_max;
};

struct mtk_mdp_dev {
	struct mutex			vpulock;
	struct platform_device		*pdev;
	const struct mtk_mdp_variant		*variant;
	u16				id;
	unsigned long			state;
	struct platform_device		*vpu_dev;
	dev_t mtk_mdp_devno;
	struct cdev *mtk_mdp_cdev;
	struct class *mtk_mdp_class;
};

struct mtk_mdp_ctx {
	struct mtk_mdp_frame		s_frame;
	struct mtk_mdp_frame		d_frame;
	u32				flags;
	u32				state;
	int				rotation;
	u32				hflip:1;
	u32				vflip:1;
	struct mtk_mdp_dev		*mdp_dev;
	struct mtk_mdp_ctrls		ctrls;
	bool				ctrls_rdy;
	unsigned long			drv_handle;

	struct mtk_mdp_vpu		vpu;
};




void mtk_mdp_hw_set_input_addr(struct mtk_mdp_ctx *ctx,
			       struct mtk_mdp_addr *addr);
void mtk_mdp_hw_set_output_addr(struct mtk_mdp_ctx *ctx,
				struct mtk_mdp_addr *addr);
void mtk_mdp_hw_set_in_size(struct mtk_mdp_ctx *ctx);
void mtk_mdp_hw_set_in_image_format(struct mtk_mdp_ctx *ctx);
void mtk_mdp_hw_set_out_size(struct mtk_mdp_ctx *ctx);
void mtk_mdp_hw_set_out_image_format(struct mtk_mdp_ctx *ctx);
void mtk_mdp_hw_set_rotation(struct mtk_mdp_ctx *ctx);
void mtk_mdp_hw_set_global_alpha(struct mtk_mdp_ctx *ctx);
void mtk_mdp_hw_set_sfr_update(struct mtk_mdp_ctx *ctx);
#endif /* __MTK_MDP_CORE_H__ */
