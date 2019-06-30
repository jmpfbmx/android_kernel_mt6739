
#ifndef _VDEC_VP8_IF_H_
#define _VDEC_VP8_IF_H_

#include "vdec_drv_base.h"

#define VP8_MAX_FRM_BUFF_NUM				5
#define VP8_HW_DATA_SZ					272
#define VP8_DEC_DATA_SZ					1200

struct vdec_vp8_dec_info {
	uint64_t vp_wrapper_dma;
	uint64_t prev_y_dma;
	uint64_t cur_y_fb_dma;
	uint64_t cur_c_fb_dma;
	uint64_t bs_dma;
	uint32_t bs_sz;
	uint32_t resolution_changed;
	uint32_t show_frame;
	uint32_t reserved;
};

struct vdec_vp8_vsi {
	struct vdec_vp8_dec_info dec;
	struct vdec_pic_info pic;
	unsigned char dec_data[VP8_DEC_DATA_SZ];
	unsigned int segid_wrapper_work[VP8_HW_DATA_SZ][4];
	unsigned int load_data;
};

struct vdec_vp8_hw_reg_base {
	void __iomem *top;
	void __iomem *cm;
	void __iomem *hwd;
};

struct vdec_vp8_vpu_inst {
	wait_queue_head_t wq_hd;
	int signaled;
	int failure;
	unsigned int h_drv;
};


struct vdec_vp8_inst {
	struct vdec_fb *cur_fb;
	struct vdec_fb_node dec_fb[VP8_MAX_FRM_BUFF_NUM];
	struct vdec_fb_node disp_fb[VP8_MAX_FRM_BUFF_NUM];
	struct list_head dec_fb_list;
	struct list_head dec_use_list;
	struct list_head dec_free_list;
	struct list_head disp_fb_list;
	struct list_head disp_rdy_list;
	struct mtk_vcodec_mem vp_wrapper_buf;
	struct vdec_vp8_hw_reg_base reg_base;
	unsigned int frm_cnt;
	void *ctx;
	struct platform_device *dev;
	struct vdec_vp8_vsi *vsi;
	struct vdec_vp8_vpu_inst vpu;
};

struct vdec_common_if *get_vp8_dec_comm_if(void);

#endif
