
#ifndef _MTK_VCODEC_DEC_H_
#define _MTK_VCODEC_DEC_H_

#include <media/videobuf2-core.h>

struct vdec_fb {
	struct mtk_vcodec_mem base_y;
	struct mtk_vcodec_mem base_c;
	unsigned int status;
};

struct mtk_video_buf {
	struct vb2_buffer b;
	struct list_head list;

	bool used;
	bool ready_to_display;
	bool nonrealdisplay;
	bool queued_in_vb2;
	bool queued_in_v4l2;
	bool lastframe;
	struct vdec_fb frame_buffer;
	struct mutex lock;
};

int mtk_vdec_unlock(struct mtk_vcodec_ctx *ctx);
int mtk_vdec_lock(struct mtk_vcodec_ctx *ctx);
int m2mctx_vdec_queue_init(void *priv, struct vb2_queue *src_vq,
			   struct vb2_queue *dst_vq);
void mtk_vcodec_dec_set_default_params(struct mtk_vcodec_ctx *ctx);
void mtk_vcodec_vdec_release(struct mtk_vcodec_ctx *ctx);
int mtk_vdec_ctrls_setup(struct mtk_vcodec_ctx *ctx);
void mtk_vdec_ctrls_free(struct mtk_vcodec_ctx *ctx);

#endif /* _MTK_VCODEC_DEC_H_ */
