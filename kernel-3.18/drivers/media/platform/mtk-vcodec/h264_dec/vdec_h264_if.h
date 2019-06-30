
#ifndef _VDEC_H264_IF_H_
#define _VDEC_H264_IF_H_

#include "vdec_drv_base.h"

#define H264_MAX_FB_NUM				17
#define NAL_TYPE(value)				((value) & 0x1F)

struct h264_fb {
	uint64_t vdec_fb_va;
	uint64_t y_fb_dma;
	uint64_t c_fb_dma;
	int32_t poc;
	uint32_t reserved;
};

struct h264_ring_fb_list {
	struct h264_fb fb_list[H264_MAX_FB_NUM];
	unsigned int read_idx;
	unsigned int write_idx;
	unsigned int count;
	unsigned int reserved;
};

struct vdec_h264_dec_info {
	uint32_t dpb_sz;
	uint32_t resolution_changed;
	uint32_t realloc_mv_buf;
	uint32_t reserved;
};

struct vdec_h264_vsi {
	uint64_t ppl_buf_dma;
	uint64_t mv_buf_dma[H264_MAX_FB_NUM];
	struct h264_ring_fb_list list_free;
	struct h264_ring_fb_list list_disp;
	struct vdec_h264_dec_info dec;
	struct vdec_pic_info pic;
	struct v4l2_rect crop;
};

struct vdec_h264_vpu_inst {
	unsigned char *hdr_bs_buf;
	unsigned int h_drv;
	unsigned int signaled;
	int failure;
	wait_queue_head_t wq;
};

struct vdec_h264_inst {
	int ctx_id;
	void *ctx;
	unsigned int num_nalu;
	struct platform_device *dev;
	struct mtk_vcodec_mem ppl_buf;
	struct mtk_vcodec_mem mv_buf[H264_MAX_FB_NUM];
	struct vdec_h264_vsi *vsi;
	struct vdec_h264_vpu_inst vpu;
};

struct vdec_common_if *get_h264_dec_comm_if(void);

#endif
