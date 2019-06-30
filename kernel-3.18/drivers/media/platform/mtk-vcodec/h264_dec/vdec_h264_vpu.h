
#ifndef _VDEC_H264_VPU_H_
#define _VDEC_H264_VPU_H_

int vdec_h264_vpu_init(struct vdec_h264_inst *inst, uint64_t bs_dma,
		       uint32_t bs_sz);
int vdec_h264_vpu_dec_start(struct vdec_h264_inst *inst, uint32_t bs_sz,
			    uint32_t nal_start, uint64_t bs_dma,
			    uint64_t y_fb_dma, uint64_t c_fb_dma,
			    uint64_t vdec_fb_va);
int vdec_h264_vpu_dec_end(struct vdec_h264_inst *inst);
int vdec_h264_vpu_deinit(struct vdec_h264_inst *inst);
int vdec_h264_vpu_reset(struct vdec_h264_inst *inst);

#endif
