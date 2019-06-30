
#ifndef _VENC_VP8_VPU_H_
#define _VENC_VP8_VPU_H_

int vp8_enc_vpu_init(struct venc_vp8_inst *inst);
int vp8_enc_vpu_set_param(struct venc_vp8_inst *inst, unsigned int id,
			  void *param);
int vp8_enc_vpu_encode(struct venc_vp8_inst *inst,
		       struct venc_frm_buf *frm_buf,
		       struct mtk_vcodec_mem *bs_buf);
int vp8_enc_vpu_deinit(struct venc_vp8_inst *inst);

#endif
