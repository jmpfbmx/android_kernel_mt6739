
#ifndef __L2C_PARITY_H__
#define __L2C_PARITY_H__

struct cfg_l2_parity_little_cluster {
	unsigned int mp_id;
	unsigned int l2_cache_parity1_rdata;
	unsigned int l2_cache_parity2_rdata;
};

struct plat_cfg_l2_parity {
	unsigned int nr_little_cluster;
	struct cfg_l2_parity_little_cluster cluster[];
};

#endif
