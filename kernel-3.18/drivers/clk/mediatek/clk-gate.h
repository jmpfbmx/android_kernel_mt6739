
#ifndef __DRV_CLK_GATE_H
#define __DRV_CLK_GATE_H

#include <linux/regmap.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>

struct mtk_clk_gate {
	struct clk_hw	hw;
	struct regmap	*regmap;
	int		set_ofs;
	int		clr_ofs;
	int		sta_ofs;
	u8		bit;
};

static inline struct mtk_clk_gate *to_clk_gate(struct clk_hw *hw)
{
	return container_of(hw, struct mtk_clk_gate, hw);
}

extern const struct clk_ops mtk_clk_gate_ops_setclr;
extern const struct clk_ops mtk_clk_gate_ops_setclr_inv;

struct clk *mtk_clk_register_gate(
		const char *name,
		const char *parent_name,
		struct regmap *regmap,
		int set_ofs,
		int clr_ofs,
		int sta_ofs,
		u8 bit,
		const struct clk_ops *ops);

#endif /* __DRV_CLK_GATE_H */
