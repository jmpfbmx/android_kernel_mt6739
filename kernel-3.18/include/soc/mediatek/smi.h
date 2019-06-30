#ifndef MTK_IOMMU_SMI_H
#define MTK_IOMMU_SMI_H

#include <linux/device.h>

#ifdef CONFIG_MTK_SMI

int mtk_smi_config_port(struct device *larbdev, unsigned int larbportid,
			bool enable);
int mtk_smi_larb_get(struct device *larbdev);
void mtk_smi_larb_put(struct device *larbdev);

#else

static int
mtk_smi_config_port(struct device *larbdev, unsigned int larbportid,
		    bool enable)
{
	return 0;
}

static inline int mtk_smi_larb_get(struct device *larbdev)
{
	return 0;
}

static inline void mtk_smi_larb_put(struct device *larbdev) { }

#endif

#endif
