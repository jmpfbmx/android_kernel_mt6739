#ifndef MTK_IOMMU_SMI_H
#define MTK_IOMMU_SMI_H

#include <linux/bitops.h>
#include <linux/device.h>

#define MTK_LARB_NR_MAX		8

#define MTK_SMI_MMU_EN(port)	BIT(port)

struct mtk_smi_larb_iommu {
	struct device *dev;
	unsigned int   mmu;
};

struct mtk_smi_iommu {
	unsigned int larb_nr;
	struct mtk_smi_larb_iommu larb_imu[MTK_LARB_NR_MAX];
};

#if (defined(CONFIG_MTK_SMI) && (!defined(CONFIG_MTK_SMI_EXT)))
int mtk_smi_larb_get(struct device *larbdev);
void mtk_smi_larb_put(struct device *larbdev);
int mtk_smi_larb_clock_on(int larbid, bool pm);
void mtk_smi_larb_clock_off(int larbid, bool pm);
int mtk_smi_larb_ready(int larbid);

#if defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT) && defined(CONFIG_MTK_SEC_VIDEO_PATH_SUPPORT)
#define M4U_TEE_SERVICE_ENABLE
int pseudo_config_port_tee(int kernelport);
#endif

#else

static inline int mtk_smi_larb_get(struct device *larbdev)
{
	return 0;
}

static inline void mtk_smi_larb_put(struct device *larbdev) { }

#endif

#endif
