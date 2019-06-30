
#ifndef __DFD_H__
#define __DFD_H__

#define DFD_SMC_MAGIC_SETUP (0x99716150)

struct dfd_drv {
	u64 base_addr;
	u32 base_addr_msb;
	unsigned long chain_length;
	unsigned long rg_dfd_timeout;
	unsigned int enabled;
};

extern unsigned int __attribute__((weak)) check_dfd_support(void);
extern unsigned int __attribute__((weak)) dfd_infra_base(void);

#endif
