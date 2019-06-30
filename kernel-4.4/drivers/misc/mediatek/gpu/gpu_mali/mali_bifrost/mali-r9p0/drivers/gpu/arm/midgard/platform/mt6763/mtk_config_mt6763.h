
#ifndef __MTK_COMMON_H__
#define __MTK_COMMON_H__

/* MTK device context driver private data */
struct mtk_config {
	volatile void *mfg_register;
	struct clk *clk_mfg0;
	struct clk *clk_mfg1;
	struct clk *clk_mfg2;
	struct clk *clk_mfg3;
	struct clk *clk_mfg4;
	struct clk *clk_mfg_cg;
};

#define MTKCLK_prepare_enable(clk) \
	do { \
		if (config->clk) { \
			if (clk_prepare_enable(config->clk)) { \
				pr_alert("MALI: clk_prepare_enable failed when enabling" #clk); \
			} \
		} \
	} while (0)

#define MTKCLK_disable_unprepare(clk) \
	do { \
		if (config->clk) \
			clk_disable_unprepare(config->clk); \
	} while (0)

#endif /* __MTK_COMMON_H__ */
