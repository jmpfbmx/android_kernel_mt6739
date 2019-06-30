#ifndef _SSPM_RESERVEDMEM_DEFINE_H_
#define _SSPM_RESERVEDMEM_DEFINE_H_
#include <sspm_reservedmem.h>

enum {
	SSPM_MEM_ID = 0,
	NUMS_MEM_ID,
};

#define SSPM_PLT_LOGGER_BUF_LEN        (511 * 1024)

#ifdef _SSPM_INTERNAL_
/* The total size of sspm_reserve_mblock should less equal than reserve-memory-sspm_share of device tree */
static struct sspm_reserve_mblock sspm_reserve_mblock[NUMS_MEM_ID] = {
	{
		.num = SSPM_MEM_ID,
		.size = SSPM_PLT_LOGGER_BUF_LEN + 1024,
	},
};
#endif
#endif
