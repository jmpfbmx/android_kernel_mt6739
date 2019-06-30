
#ifndef _MTK_VCODEC_PM_H_
#define _MTK_VCODEC_PM_H_

#include "mtk_vcodec_drv.h"

int mtk_vcodec_init_dec_pm(struct mtk_vcodec_dev *dev);
void mtk_vcodec_release_dec_pm(struct mtk_vcodec_dev *dev);
int mtk_vcodec_init_enc_pm(struct mtk_vcodec_dev *dev);
void mtk_vcodec_release_enc_pm(struct mtk_vcodec_dev *dev);

void mtk_vcodec_enc_pw_on(struct mtk_vcodec_pm *pm);
void mtk_vcodec_enc_pw_off(struct mtk_vcodec_pm *pm);
void mtk_vcodec_enc_clock_on(struct mtk_vcodec_pm *pm);
void mtk_vcodec_enc_clock_off(struct mtk_vcodec_pm *pm);
void mtk_vcodec_dec_pw_on(struct mtk_vcodec_pm *pm);
void mtk_vcodec_dec_pw_off(struct mtk_vcodec_pm *pm);
void mtk_vcodec_dec_clock_on(struct mtk_vcodec_pm *pm);
void mtk_vcodec_dec_clock_off(struct mtk_vcodec_pm *pm);

#endif /* _MTK_VCODEC_PM_H_ */
