
#ifndef _VDEC_DRV_IF_H_
#define _VDEC_DRV_IF_H_

#include "mtk_vcodec_drv.h"
#include "mtk_vcodec_dec.h"
#include "mtk_vcodec_util.h"

enum vdec_fb_status {
	FB_ST_NORMAL		= 0,
	FB_ST_DISPLAY		= (1 << 0),
	FB_ST_FREE		= (1 << 1)
};

enum vdec_get_param_type {
	GET_PARAM_DISP_FRAME_BUFFER,
	GET_PARAM_FREE_FRAME_BUFFER,
	GET_PARAM_PIC_INFO,
	GET_PARAM_CROP_INFO,
	GET_PARAM_DPB_SIZE
};

struct vdec_fb_node {
	struct list_head list;
	void *fb;
};



int vdec_if_create(struct mtk_vcodec_ctx *ctx, unsigned int fourcc);

int vdec_if_release(struct mtk_vcodec_ctx *ctx);

int vdec_if_init(struct mtk_vcodec_ctx *ctx, struct mtk_vcodec_mem *bs,
		 struct vdec_pic_info *pic);

int vdec_if_get_param(struct mtk_vcodec_ctx *ctx, enum vdec_get_param_type type,
		      void *out);

int vdec_if_decode(struct mtk_vcodec_ctx *ctx, struct mtk_vcodec_mem *bs,
		   struct vdec_fb *fb, bool *res_chg);

#endif
