
#ifndef _VENC_VP8_IF_H_
#define _VENC_VP8_IF_H_

#include "venc_drv_base.h"

enum venc_vp8_vpu_work_buf {
	VENC_VP8_VPU_WORK_BUF_LUMA,
	VENC_VP8_VPU_WORK_BUF_LUMA2,
	VENC_VP8_VPU_WORK_BUF_LUMA3,
	VENC_VP8_VPU_WORK_BUF_CHROMA,
	VENC_VP8_VPU_WORK_BUF_CHROMA2,
	VENC_VP8_VPU_WORK_BUF_CHROMA3,
	VENC_VP8_VPU_WORK_BUF_MV_INFO,
	VENC_VP8_VPU_WORK_BUF_BS_HD,
	VENC_VP8_VPU_WORK_BUF_PROB_BUF,
	VENC_VP8_VPU_WORK_BUF_RC_INFO,
	VENC_VP8_VPU_WORK_BUF_RC_CODE,
	VENC_VP8_VPU_WORK_BUF_RC_CODE2,
	VENC_VP8_VPU_WORK_BUF_RC_CODE3,
	VENC_VP8_VPU_WORK_BUF_SRC_LUMA,
	VENC_VP8_VPU_WORK_BUF_SRC_CHROMA,
	VENC_VP8_VPU_WORK_BUF_SRC_CHROMA_CB,
	VENC_VP8_VPU_WORK_BUF_SRC_CHROMA_CR,
	VENC_VP8_VPU_WORK_BUF_MAX,
};

struct venc_vp8_vpu_config {
	u32 input_fourcc;
	u32 bitrate;
	u32 pic_w;
	u32 pic_h;
	u32 buf_w;
	u32 buf_h;
	u32 intra_period;
	u32 framerate;
	u32 ts_mode;
};

struct venc_vp8_vpu_buf {
	u32 align;
	u32 iova;
	u32 vpua;
	u32 size;
};

struct venc_vp8_vpu_drv {
	struct venc_vp8_vpu_config config;
	struct venc_vp8_vpu_buf work_bufs[VENC_VP8_VPU_WORK_BUF_MAX];
};

struct venc_vp8_vpu_inst {
	wait_queue_head_t wq_hd;
	int signaled;
	int failure;
	unsigned int id;
	struct venc_vp8_vpu_drv *drv;
};

struct venc_vp8_inst {
	void __iomem *hw_base;
	struct mtk_vcodec_mem work_bufs[VENC_VP8_VPU_WORK_BUF_MAX];
	bool work_buf_allocated;
	unsigned int frm_cnt;
	unsigned int ts_mode;
	struct venc_vp8_vpu_inst vpu_inst;
	void *ctx;
	struct platform_device *dev;
};

struct venc_common_if *get_vp8_enc_comm_if(void);

#endif
