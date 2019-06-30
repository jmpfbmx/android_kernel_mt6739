
#ifndef __MTK_BIF_INTF_H
#define __MTK_BIF_INTF_H

#ifdef CONFIG_MTK_BIF_SUPPORT
#include <linux/errno.h>

extern int mtk_bif_init(void);
extern int mtk_bif_get_vbat(unsigned int *vbat);
extern int mtk_bif_get_tbat(int *tbat);
extern bool mtk_bif_is_hw_exist(void);

#else /* Not CONFIG_MTK_BIF_SUPPORT */

static inline int mtk_bif_init(void)
{
	return -ENOTSUPP;
}

static inline int mtk_bif_get_vbat(unsigned int *vbat)
{
	*vbat = 0;
	return -ENOTSUPP;
}

static inline int mtk_bif_get_tbat(int *tbat)
{
	*tbat = 0;
	return -ENOTSUPP;
}

static inline bool mtk_bif_is_hw_exist(void)
{
	return false;
}

#endif /* CONFIG_MTK_BIF_SUPPORT */
#endif /* __MTK_BIF_INTF_H */
