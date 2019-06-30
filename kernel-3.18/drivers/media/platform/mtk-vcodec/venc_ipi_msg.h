
#ifndef _VENC_IPI_MSG_H_
#define _VENC_IPI_MSG_H_

#define IPIMSG_H264_ENC_ID 0x100
#define IPIMSG_VP8_ENC_ID 0x200

#define AP_IPIMSG_VENC_BASE 0x20000
#define VPU_IPIMSG_VENC_BASE 0x30000

enum venc_ipi_msg_id {
	AP_IPIMSG_H264_ENC_INIT = AP_IPIMSG_VENC_BASE +
				  IPIMSG_H264_ENC_ID,
	AP_IPIMSG_H264_ENC_SET_PARAM,
	AP_IPIMSG_H264_ENC_ENCODE,
	AP_IPIMSG_H264_ENC_DEINIT,

	AP_IPIMSG_VP8_ENC_INIT = AP_IPIMSG_VENC_BASE +
				 IPIMSG_VP8_ENC_ID,
	AP_IPIMSG_VP8_ENC_SET_PARAM,
	AP_IPIMSG_VP8_ENC_ENCODE,
	AP_IPIMSG_VP8_ENC_DEINIT,

	VPU_IPIMSG_H264_ENC_INIT_DONE = VPU_IPIMSG_VENC_BASE +
					IPIMSG_H264_ENC_ID,
	VPU_IPIMSG_H264_ENC_SET_PARAM_DONE,
	VPU_IPIMSG_H264_ENC_ENCODE_DONE,
	VPU_IPIMSG_H264_ENC_DEINIT_DONE,

	VPU_IPIMSG_VP8_ENC_INIT_DONE = VPU_IPIMSG_VENC_BASE +
				       IPIMSG_VP8_ENC_ID,
	VPU_IPIMSG_VP8_ENC_SET_PARAM_DONE,
	VPU_IPIMSG_VP8_ENC_ENCODE_DONE,
	VPU_IPIMSG_VP8_ENC_DEINIT_DONE,
};

struct venc_ap_ipi_msg_init {
	uint32_t msg_id;
	uint32_t reserved;
	uint64_t venc_inst;
};

struct venc_ap_ipi_msg_set_param {
	uint32_t msg_id;
	uint32_t inst_id;
	uint32_t param_id;
	uint32_t data_item;
	uint32_t data[8];
};

struct venc_ap_ipi_msg_enc {
	uint32_t msg_id;
	uint32_t inst_id;
	uint32_t bs_mode;
	uint32_t input_addr[3];
	uint32_t bs_addr;
	uint32_t bs_size;
};

struct venc_ap_ipi_msg_deinit {
	uint32_t msg_id;
	uint32_t inst_id;
};

enum venc_ipi_msg_status {
	VENC_IPI_MSG_STATUS_OK,
	VENC_IPI_MSG_STATUS_FAIL,
};

struct venc_vpu_ipi_msg_common {
	uint32_t msg_id;
	uint32_t status;
	uint64_t venc_inst;
};

struct venc_vpu_ipi_msg_init {
	uint32_t msg_id;
	uint32_t status;
	uint64_t venc_inst;
	uint32_t inst_id;
	uint32_t reserved;
};

struct venc_vpu_ipi_msg_set_param {
	uint32_t msg_id;
	uint32_t status;
	uint64_t venc_inst;
	uint32_t param_id;
	uint32_t data_item;
	uint32_t data[6];
};

enum venc_ipi_msg_enc_state {
	VEN_IPI_MSG_ENC_STATE_FRAME,
	VEN_IPI_MSG_ENC_STATE_PART,
	VEN_IPI_MSG_ENC_STATE_SKIP,
	VEN_IPI_MSG_ENC_STATE_ERROR,
};

struct venc_vpu_ipi_msg_enc {
	uint32_t msg_id;
	uint32_t status;
	uint64_t venc_inst;
	uint32_t state;
	uint32_t key_frame;
	uint32_t bs_size;
	uint32_t reserved;
};

struct venc_vpu_ipi_msg_deinit {
	uint32_t msg_id;
	uint32_t status;
	uint64_t venc_inst;
};

#endif /* _VENC_IPI_MSG_H_ */
