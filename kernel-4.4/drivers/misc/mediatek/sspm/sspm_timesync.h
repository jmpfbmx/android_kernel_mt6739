#ifndef _SSPM_TIMESYNC_H_
#define _SSPM_TIMESTNC_H_

extern unsigned int sspm_timesync_init(phys_addr_t start, phys_addr_t limit);
extern int sspm_timesync_init_done(void);
extern void sspm_timesync_ts_get(unsigned int *ts_h, unsigned int *ts_l);
extern void sspm_timesync_clk_get(unsigned int *clk_h, unsigned int *clk_l);
extern u64 mtk_timer_get_cnt(u8 timer);
#endif
