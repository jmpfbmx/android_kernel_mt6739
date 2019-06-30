
#ifndef __ION_SEC_HEAP_H__
#define __ION_SEC_HEAP_H__

typedef struct {
	struct mutex lock;
	int eModuleID;
	unsigned int security;
	unsigned int coherent;
	void *pVA;
	unsigned int MVA;
	ion_phys_addr_t priv_phys;
	ion_mm_buf_debug_info_t dbg_info;
} ion_sec_buffer_info;

#endif
