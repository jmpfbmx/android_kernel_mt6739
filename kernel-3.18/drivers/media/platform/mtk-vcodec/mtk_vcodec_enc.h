

#ifndef _MTK_VCODEC_ENC_H_
#define _MTK_VCODEC_ENC_H_

#include <media/videobuf2-core.h>

struct mtk_video_enc_buf {
	struct vb2_buffer b;
	struct list_head list;

	enum mtk_encode_param param_change;
	struct mtk_enc_params enc_params;
};

int mtk_venc_unlock(struct mtk_vcodec_ctx *ctx);
int mtk_venc_lock(struct mtk_vcodec_ctx *ctx);
int m2mctx_venc_queue_init(void *priv, struct vb2_queue *src_vq,
	struct vb2_queue *dst_vq);
void mtk_vcodec_venc_release(struct mtk_vcodec_ctx *ctx);
int mtk_venc_ctrls_setup(struct mtk_vcodec_ctx *ctx);
void mtk_venc_ctrls_free(struct mtk_vcodec_ctx *ctx);

#endif /* _MTK_VCODEC_ENC_H_ */
