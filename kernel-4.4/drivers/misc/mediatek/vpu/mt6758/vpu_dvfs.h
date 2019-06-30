
#ifndef _VPU_DVFS_H_
#define _VPU_DVFS_H_

enum VPU_OPP_INDEX {
	VPU_OPP_0 = 0,
	VPU_OPP_1 = 1,
	VPU_OPP_NUM
};

struct VPU_OPP_INFO {
	enum VPU_OPP_INDEX opp_index;
	int power;	/*mW*/
};

extern struct VPU_OPP_INFO vpu_power_table[VPU_OPP_NUM];
extern int32_t vpu_thermal_en_throttle_cb(uint8_t vcore_opp, uint8_t vpu_opp);
extern int32_t vpu_thermal_dis_throttle_cb(void);

#endif
