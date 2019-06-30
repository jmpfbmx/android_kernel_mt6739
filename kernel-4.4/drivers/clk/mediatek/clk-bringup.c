
#include <linux/clk.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>

static const struct of_device_id bring_up_id_table[] = {
	{ .compatible = "mediatek,clk-bring-up",},
	{ .compatible = "mediatek,mt8163-bring-up",},
	{ .compatible = "mediatek,mt8173-bring-up",},
	{ .compatible = "mediatek,elbrus-bring-up",},
	{ .compatible = "mediatek,mt6799-bring-up",},
	{ },
};
MODULE_DEVICE_TABLE(of, bring_up_id_table);

static int bring_up_probe(struct platform_device *pdev)
{
	const int NR_CLKS = 420;
	char clk_name_buf[16];
	struct clk *clk;
	int i;

	pr_debug("ccf bring up\r\n");
	for (i = 0; i < NR_CLKS; i++) {
		sprintf(clk_name_buf, "%d", i);

		clk = devm_clk_get(&pdev->dev, clk_name_buf);
		if (!IS_ERR(clk)) {
			clk_prepare_enable(clk);
			/*pr_debug("%d: %10ldhz, %d\r\n", i, clk_get_rate(clk), clk->enable_count);*/
		}
	}

	return 0;
}

static int bring_up_remove(struct platform_device *pdev)
{
	return 0;
}

static struct platform_driver bring_up = {
	.probe		= bring_up_probe,
	.remove		= bring_up_remove,
	.driver		= {
		.name	= "bring_up",
		.owner	= THIS_MODULE,
		.of_match_table = bring_up_id_table,
	},
};

module_platform_driver(bring_up);
