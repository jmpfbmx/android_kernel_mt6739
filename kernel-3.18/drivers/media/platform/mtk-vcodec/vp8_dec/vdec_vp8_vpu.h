
#ifndef _VDEC_VP8_VPU_H_
#define _VDEC_VP8_VPU_H_

int vdec_vp8_vpu_init(struct vdec_vp8_inst *inst, unsigned int data);
int vdec_vp8_vpu_dec_start(struct vdec_vp8_inst *inst);
int vdec_vp8_vpu_dec_end(struct vdec_vp8_inst *inst);
int vdec_vp8_vpu_reset(struct vdec_vp8_inst *inst);
int vdec_vp8_vpu_deinit(struct vdec_vp8_inst *inst);

#endif
