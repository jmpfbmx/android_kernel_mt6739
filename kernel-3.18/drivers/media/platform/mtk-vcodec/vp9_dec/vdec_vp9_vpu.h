
#ifndef VDEC_VP9_VPU_H_
#define VDEC_VP9_VPU_H_

#include <linux/wait.h>

int vp9_dec_vpu_init(void *vdec_inst, unsigned int *data,
		     unsigned int items);
int vp9_dec_vpu_start(void *vdec_inst);
int vp9_dec_vpu_end(void *vdec_inst);
int vp9_dec_vpu_reset(void *vdec_inst);
int vp9_dec_vpu_deinit(void *vdec_inst);

#endif /* #ifndef VDEC_DRV_VP9_VPU_H_ */

