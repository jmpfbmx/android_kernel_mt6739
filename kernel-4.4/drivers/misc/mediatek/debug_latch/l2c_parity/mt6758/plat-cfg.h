
#ifndef __PLAT_CFG_H__
#define __PLAT_CFG_H__

const struct plat_cfg_l2_parity cfg_l2_parity = {
	.nr_little_cluster = 2,
	.cluster = {
		{
			.mp_id = 0,
			.l2_cache_parity1_rdata = 0x007c,
			.l2_cache_parity2_rdata = 0x0080,
		},
		{
			.mp_id = 1,
			.l2_cache_parity1_rdata = 0x027c,
			.l2_cache_parity2_rdata = 0x0280,
		},
	},
};

#endif
