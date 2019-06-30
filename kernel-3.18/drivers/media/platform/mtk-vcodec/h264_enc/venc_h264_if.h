
#ifndef _VENC_H264_IF_H_
#define _VENC_H264_IF_H_

#include "venc_drv_base.h"

enum venc_h264_vpu_work_buf {
	VENC_H264_VPU_WORK_BUF_RC_INFO,
	VENC_H264_VPU_WORK_BUF_RC_CODE,
	VENC_H264_VPU_WORK_BUF_REC_LUMA,
	VENC_H264_VPU_WORK_BUF_REC_CHROMA,
	VENC_H264_VPU_WORK_BUF_REF_LUMA,
	VENC_H264_VPU_WORK_BUF_REF_CHROMA,
	VENC_H264_VPU_WORK_BUF_MV_INFO_1,
	VENC_H264_VPU_WORK_BUF_MV_INFO_2,
	VENC_H264_VPU_WORK_BUF_SKIP_FRAME,
	VENC_H264_VPU_WORK_BUF_SRC_LUMA,
	VENC_H264_VPU_WORK_BUF_SRC_CHROMA,
	VENC_H264_VPU_WORK_BUF_SRC_CHROMA_CB,
	VENC_H264_VPU_WORK_BUF_SRC_CHROMA_CR,
	VENC_H264_VPU_WORK_BUF_MAX,
};

enum venc_h264_bs_mode {
	H264_BS_MODE_SPS,
	H264_BS_MODE_PPS,
	H264_BS_MODE_FRAME,
};

struct venc_h264_vpu_config {
	u32 input_fourcc;
	u32 bitrate;
	u32 pic_w;
	u32 pic_h;
	u32 buf_w;
	u32 buf_h;
	u32 intra_period;
	u32 framerate;
	u32 profile;
	u32 level;
	u32 wfd;
};

struct venc_h264_vpu_buf {
	u32 align;
	u32 iova;
	u32 vpua;
	u32 size;
};

struct venc_h264_vpu_drv {
	struct venc_h264_vpu_config config;
	struct venc_h264_vpu_buf work_bufs[VENC_H264_VPU_WORK_BUF_MAX];
};

struct venc_h264_vpu_inst {
	wait_queue_head_t wq_hd;
	int signaled;
	int failure;
	int state;
	int bs_size;
	int wait_int;
	unsigned int id;
	struct venc_h264_vpu_drv *drv;
};

struct venc_h264_inst {
	void __iomem *hw_base;
	struct mtk_vcodec_mem work_bufs[VENC_H264_VPU_WORK_BUF_MAX];
	struct mtk_vcodec_mem pps_buf;
	bool work_buf_allocated;
	unsigned int frm_cnt;
	unsigned int prepend_hdr;
	unsigned int is_key_frm;
	struct venc_h264_vpu_inst vpu_inst;
	void *ctx;
	struct platform_device *dev;
};

struct venc_common_if *get_h264_enc_comm_if(void);

#endif
