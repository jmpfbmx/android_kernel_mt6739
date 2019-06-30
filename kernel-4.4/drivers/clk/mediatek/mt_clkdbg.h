
#include <linux/clk-provider.h>

extern bool __clk_is_prepared(struct clk *clk);
extern unsigned int mt_get_abist_freq(unsigned int ID);
extern unsigned int mt_get_ckgen_freq(unsigned int ID);

