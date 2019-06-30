
#ifndef __VPU_ALGO_H__
#define __VPU_ALGO_H__

#include "vpu_cmn.h"

enum {
	VPU_PROP_RESERVED,
	VPU_NUM_PROPS
};

extern const size_t g_vpu_prop_type_size[VPU_NUM_PROP_TYPES];

int vpu_create_algo(char *name, struct vpu_algo **ralgo);

#endif
