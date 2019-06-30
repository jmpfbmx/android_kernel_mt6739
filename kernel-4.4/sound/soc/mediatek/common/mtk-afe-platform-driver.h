
#ifndef _MTK_AFE_PLATFORM_DRIVER_H_
#define _MTK_AFE_PLATFORM_DRIVER_H_

extern const struct snd_soc_platform_driver mtk_afe_pcm_platform;

struct mtk_base_afe;

int mtk_afe_combine_sub_dai(struct mtk_base_afe *afe);
#endif

