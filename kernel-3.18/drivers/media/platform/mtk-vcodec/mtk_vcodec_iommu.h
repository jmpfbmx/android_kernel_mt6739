
#ifndef MTK_VCODEC_IOMMU_H_
#define MTK_VCODEC_IOMMU_H_

#include "mtk_vcodec_drv.h"

int mtk_vcodec_iommu_init(struct device *dev);
void mtk_vcodec_iommu_deinit(struct device *dev);

#endif
