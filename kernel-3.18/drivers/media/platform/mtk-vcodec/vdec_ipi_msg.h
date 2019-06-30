
#ifndef _VDEC_IPI_MSG_H_
#define _VDEC_IPI_MSG_H_

enum vdec_ipi_msgid {
	AP_IPIMSG_DEC_INIT			= 0xA000,
	AP_IPIMSG_DEC_START			= 0xA001,
	AP_IPIMSG_DEC_END			= 0xA002,
	AP_IPIMSG_DEC_DEINIT			= 0xA003,
	AP_IPIMSG_DEC_RESET			= 0xA004,

	VPU_IPIMSG_DEC_INIT_ACK			= 0xB000,
	VPU_IPIMSG_DEC_START_ACK		= 0xB001,
	VPU_IPIMSG_DEC_END_ACK			= 0xB002,
	VPU_IPIMSG_DEC_DEINIT_ACK		= 0xB003,
	VPU_IPIMSG_DEC_RESET_ACK		= 0xB004,
};

struct vdec_ap_ipi_cmd {
	uint32_t msg_id;
	uint32_t h_drv;
};

struct vdec_vpu_ipi_ack {
	uint32_t msg_id;
	int32_t status;
	uint64_t vdec_inst;
};

#endif
