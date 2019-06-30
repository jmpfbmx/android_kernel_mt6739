
#include <linux/seq_file.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>

static void __iomem *efuse_base;

void mtk_es_reg_dump(struct seq_file *m, void *v)
{
	seq_printf(m, "1. 0x%x\n", readl(efuse_base + 0x928) & 0x1);
}

int mtk_es_reg_init(void)
{
	struct device_node *node;

	node = of_find_compatible_node(NULL, NULL, "mediatek,efusec");
	if (unlikely(!node))
		return -ENODEV;

	efuse_base = of_iomap(node, 0);
	if (unlikely(!efuse_base))
		return -ENOMEM;

	return 0;
}

