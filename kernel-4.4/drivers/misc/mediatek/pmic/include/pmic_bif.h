
#ifndef __MTK_BIF_INTF_H
#define __MTK_BIF_INTF_H

extern int mtk_bif_init(void);
extern int mtk_bif_get_vbat(int *vbat);
extern int mtk_bif_get_tbat(int *tbat);
extern bool mtk_bif_is_hw_exist(void);
extern int pmic_bif_init(void);

#endif /* __MTK_BIF_INTF_H */
