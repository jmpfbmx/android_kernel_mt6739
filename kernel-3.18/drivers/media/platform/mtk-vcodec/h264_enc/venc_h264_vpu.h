
#ifndef _VENC_H264_VPU_H_
#define _VENC_H264_VPU_H_

int h264_enc_vpu_init(struct venc_h264_inst *inst);
int h264_enc_vpu_set_param(struct venc_h264_inst *inst, unsigned int id,
			   void *param);
int h264_enc_vpu_encode(struct venc_h264_inst *inst, unsigned int bs_mode,
			struct venc_frm_buf *frm_buf,
			struct mtk_vcodec_mem *bs_buf,
			unsigned int *bs_size);
int h264_enc_vpu_deinit(struct venc_h264_inst *inst);

#endif
