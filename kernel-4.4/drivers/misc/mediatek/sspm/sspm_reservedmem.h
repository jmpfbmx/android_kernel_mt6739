#ifndef __SSPM_RESERVED_H__
#define __SSPM_RESERVED_H__

#include <linux/types.h>

struct sspm_reserve_mblock {
	u32 num;
	u64 start_phys;
	u64 start_virt;
	u64 size;
};

phys_addr_t sspm_reserve_mem_get_phys(unsigned int id);
phys_addr_t sspm_reserve_mem_get_virt(unsigned int id);
phys_addr_t sspm_reserve_mem_get_size(unsigned int id);
int sspm_reserve_memory_init(void);
void sspm_set_emi_mpu(phys_addr_t base, phys_addr_t size);
void sspm_lock_emi_mpu(void);

#endif
