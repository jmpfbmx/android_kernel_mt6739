
#ifndef _MT6769_VPU_HW_H_
#define _MT6769_VPU_HW_H_

#include "vpu_reg.h"
#include "vpu_drv.h"

#define VPU_MAX_NUM_CODE_SEGMENTS       (50)
#define VPU_MAX_NUM_ALGOS               (50)
#define VPU_MAX_NUM_STEPS               (8)
#define VPU_MAX_NUM_OPPS                (8)

/* MVA */
#define VPU_MVA_RESET_VECTOR            (0x7DA00000)
#define VPU2_MVA_RESET_VECTOR           (0x7E300000)
#define VPU3_MVA_RESET_VECTOR           (0x7EC00000)
#define VPU_MVA_MAIN_PROGRAM            (0x7DB00000)
#define VPU2_MVA_MAIN_PROGRAM           (0x7E400000)
#define VPU3_MVA_MAIN_PROGRAM           (0x7ED00000)
#define VPU_MVA_KERNEL_LIB				(0x7DE00000)
#define VPU2_MVA_KERNEL_LIB				(0x7E700000)
#define VPU3_MVA_KERNEL_LIB				(0x7F000000)
#define VPU_MVA_SHARED_DATA				(0x7F500000)
#define VPU_MVA_RESERVED_END			(0x82600000)

/* Sum of all parts */
#define VPU_SIZE_BINARY_CODE            (0x02A10000)
/* Size */
#define VPU_SIZE_RESET_VECTOR           (0x00100000)
#define VPU_SIZE_MAIN_PROGRAM           (0x00300000)
#define VPU_SIZE_ALGO_KERNEL_LIB        (0x00500000)
#define VPU_SIZE_ALGO_AREA              (0x00500000)
#define VPU_SIZE_MAIN_PROGRAM_IMEM      (0x00030000)
#define VPU_SIZE_SHARED_DATA            (0x00600000)
#define VPU_NUMS_IMAGE_HEADER           (3)

/* Offset */
#define VPU_OFFSET_RESET_VECTOR         (0x00000000)
#define VPU_OFFSET_MAIN_PROGRAM         (0x00100000)
#define VPU_OFFSET_ALGO_AREA            (0x00C00000)
#define VPU_OFFSET_MAIN_PROGRAM_IMEM    (VPU_SIZE_BINARY_CODE - 0xC0000)
#define VPU_OFFSET_IMAGE_HEADERS        (VPU_SIZE_BINARY_CODE - 0x30000)
#define VPU_DDR_SHIFT_RESET_VECTOR		(0x00400000)
#define VPU_DDR_SHIFT_IRAM_DATA			(0x00030000)


struct vpu_code_segment {
	uint32_t vpu_core;		/* core index*/
	uint32_t offset;        /* offset from this partition */
	uint32_t dst_addr;      /* the DDR position is IPU can realize. */
	uint32_t length;        /* total size for this segment */
	uint32_t file_size;     /* file size to copy */
};

struct vpu_algo_info {
	uint32_t vpu_core;       /* core index*/
	uint32_t offset;
	uint32_t length;
	vpu_name_t name;
};

struct vpu_dvfs_steps {
	uint32_t values[VPU_MAX_NUM_STEPS];
	uint8_t count;
	uint8_t index;
	uint8_t opp_map[VPU_MAX_NUM_OPPS];
};

struct vpu_dvfs_opps {
	struct vpu_dvfs_steps vcore;
	struct vpu_dvfs_steps dsp;	/* ipu_conn */
	struct vpu_dvfs_steps dspcore[MTK_VPU_CORE];	/* ipu_core# */
	struct vpu_dvfs_steps ipu_if;	/* ipusys_vcore, interface */
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




int vpu_hw_boot_sequence(int core);

int vpu_hw_set_debug(int core);

int vpu_hw_enable_jtag(bool enabled);

int vpu_hw_enque_request(int core, struct vpu_request *req);

int vpu_hw_processing_request(int core, struct vpu_request *req);


int32_t vpu_thermal_en_throttle_cb(uint8_t vcore_opp, uint8_t vpu_opp);

int32_t vpu_thermal_dis_throttle_cb(void);

void vpu_dump_debug_stack(int core, int size);

void vpu_dump_code_segment(int core);

void vpu_dump_algo_segment(int core, int algo_id, int size);


#define VPU_OFFSET_COMMAND           (0x00000000)
#define VPU_OFFSET_LOG               (0x00002000)
#define VPU_SIZE_LOG_BUF             (0x00010000)
#define VPU_SIZE_LOG_SHIFT           (0x00000300)
#define VPU_SIZE_LOG_HEADER          (0x00000010)
#define VPU_SIZE_WORK_BUF            (0x00012000)

#endif
