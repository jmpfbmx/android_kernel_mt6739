
#ifndef __DRV_CLKCHK_H
#define __DRV_CLKCHK_H

#include <stdbool.h>
#include <stddef.h>

struct clkchk_cfg {
	bool aee_excp_on_fail;
	bool warn_on_fail;
	const char * const *compatible;
	const char * const *off_pll_names;
	const char * const *all_clk_names;
};

int clkchk_init(struct clkchk_cfg *cfg);

#endif /* __DRV_CLKCHK_H */
