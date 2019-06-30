
#ifndef _VENC_DRV_IF_H_
#define _VENC_DRV_IF_H_

#include "mtk_vcodec_drv.h"
#include "mtk_vcodec_util.h"

enum venc_yuv_fmt {
	VENC_YUV_FORMAT_420 = 3,
	VENC_YUV_FORMAT_YV12 = 5,
	VENC_YUV_FORMAT_NV12 = 6,
	VENC_YUV_FORMAT_NV21 = 7,
};

enum venc_start_opt {
	VENC_START_OPT_ENCODE_SEQUENCE_HEADER,
	VENC_START_OPT_ENCODE_FRAME,
};

enum venc_drv_msg {
	VENC_MESSAGE_OK,
	VENC_MESSAGE_ERR,
};

enum venc_set_param_type {
	VENC_SET_PARAM_ENC,
	VENC_SET_PARAM_FORCE_INTRA,
	VENC_SET_PARAM_ADJUST_BITRATE,
	VENC_SET_PARAM_ADJUST_FRAMERATE,
	VENC_SET_PARAM_I_FRAME_INTERVAL,
	VENC_SET_PARAM_SKIP_FRAME,
	VENC_SET_PARAM_PREPEND_HEADER,
	VENC_SET_PARAM_TS_MODE,
};

struct venc_enc_prm {
	enum venc_yuv_fmt input_fourcc;
	unsigned int h264_profile;
	unsigned int h264_level;
	unsigned int width;
	unsigned int height;
	unsigned int buf_width;
	unsigned int buf_height;
	unsigned int frm_rate;
	unsigned int intra_period;
	unsigned int bitrate;
	unsigned int sizeimage[MTK_VCODEC_MAX_PLANES];
};

struct venc_frm_buf {
	struct mtk_vcodec_mem fb_addr;
	struct mtk_vcodec_mem fb_addr1;
	struct mtk_vcodec_mem fb_addr2;
};

struct venc_done_result {
	enum venc_drv_msg msg;
	unsigned int bs_size;
	bool is_key_frm;
};

int venc_if_create(struct mtk_vcodec_ctx *ctx, unsigned int fourcc);

int venc_if_release(struct mtk_vcodec_ctx *ctx);

int venc_if_set_param(struct mtk_vcodec_ctx *ctx,
		      enum venc_set_param_type type,
		      void *in);

int venc_if_encode(struct mtk_vcodec_ctx *ctx,
		   enum venc_start_opt opt,
		   struct venc_frm_buf *frm_buf,
		   struct mtk_vcodec_mem *bs_buf,
		   struct venc_done_result *result);

#endif /* _VENC_DRV_IF_H_ */
