
#ifndef _MT6759_VPU_HW_H_
#define _MT6759_VPU_HW_H_

#include "vpu_reg.h"
#include "vpu_drv.h"

#define VPU_MAX_NUM_CODE_SEGMENTS       (20)
#define VPU_MAX_NUM_ALGOS               (20)
#define VPU_MAX_NUM_STEPS               (4)
#define VPU_MAX_NUM_OPPS                (10)

/* MVA */
#define VPU_MVA_RESET_VECTOR            (0x50000000)
#define VPU_MVA_MAIN_PROGRAM            (0x60000000)

/* Size */
#define VPU_SIZE_RESET_VECTOR           (0x00080000)
#define VPU_SIZE_MAIN_PROGRAM           (0x00180000)
#define VPU_SIZE_ALGO_AREA              (0x00C80000)
#define VPU_SIZE_MAIN_PROGRAM_IMEM      (0x00040000)
#define VPU_SIZE_IMAGE_HEADERS          (0x00040000)
#define VPU_SIZE_RESERVED_INSTRUCT      (0x00500000)
#define VPU_NUMS_IMAGE_HEADER           (3)

/* Offset */
#define VPU_OFFSET_RESET_VECTOR         (0x00000000)
#define VPU_OFFSET_MAIN_PROGRAM         (0x00080000)
#define VPU_OFFSET_ALGO_AREA            (0x00200000)
#define VPU_OFFSET_MAIN_PROGRAM_IEME    (0x00E80000)
#define VPU_OFFSET_IMAGE_HEADERS        (0x00EC0000)
#define VPU_OFFSET_RESERVED_INSTRUCT    (0x00F00000)

/* Sum of all parts */
#define VPU_SIZE_BINARY_CODE            (0x00F00000)


struct vpu_code_segment {
	uint32_t offset;        /* offset from this partition */
	uint32_t dst_addr;      /* the DDR position is IPU can realize. */
	uint32_t length;        /* total size for this segment */
	uint32_t file_size;     /* file size to copy */
};

struct vpu_algo_info {
	uint32_t offset;
	uint32_t length;
	char name[VPU_NAME_SIZE];
};

struct vpu_dvfs_steps {
	uint32_t values[VPU_MAX_NUM_STEPS];
	uint8_t count;
	uint8_t index;
	uint8_t opp_map[VPU_MAX_NUM_OPPS];
};

struct vpu_dvfs_opps {
	struct vpu_dvfs_steps vcore;
	struct vpu_dvfs_steps dsp;
	struct vpu_dvfs_steps ipu_if;
	uint8_t index;
	uint8_t count;
};


struct vpu_image_header {
	uint32_t version;
	uint32_t build_date;
	uint32_t header_desc[8];
	uint32_t header_size;
	uint32_t image_size;
	uint32_t code_segment_count;
	struct vpu_code_segment code_segments[VPU_MAX_NUM_CODE_SEGMENTS];
	uint32_t algo_info_count;
	struct vpu_algo_info algo_infos[VPU_MAX_NUM_ALGOS];
	uint32_t reserved[32];
};




int vpu_hw_boot_sequence(void);

int vpu_hw_set_debug(void);

int vpu_hw_enable_jtag(bool enabled);

int vpu_hw_enque_request(struct vpu_request *req);
int32_t vpu_thermal_en_throttle_cb(uint8_t vcore_opp, uint8_t vpu_opp);

int32_t vpu_thermal_dis_throttle_cb(void);


#define VPU_OFFSET_COMMAND           (0x00000000)
#define VPU_OFFSET_LOG               (0x00002000)
#define VPU_SIZE_LOG_BUF             (0x00002000)
#define VPU_SIZE_LOG_HEADER          (0x00000010)
#define VPU_SIZE_WORK_BUF            (0x00004000)

#endif
