
#ifndef __PASR_API_H__
#define __PASR_API_H__

#define MAX_RANKS	MAX_RK

struct basic_dram_setting {
	unsigned channel_nr;
	/* per-channel information */
	struct {
		/* per-rank information */
		struct {
			bool valid_rank;
			unsigned rank_size; /* unit: 1 Gb*/
			unsigned segment_nr;
		} rank[MAX_RK];
	} channel[MAX_CH];
};

void acquire_dram_setting(struct basic_dram_setting *pasrdpd);

#endif /* __PASR_API_H__ */
