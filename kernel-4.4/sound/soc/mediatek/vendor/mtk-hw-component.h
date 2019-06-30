


#ifndef _MTK_HW_COMPONENT_H_
#define _MTK_HW_COMPONENT_H_

#include <sound/core.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/pcm.h>
#include <linux/debugfs.h>

unsigned int get_hw_version(void);
int get_exthp_dai_codec_name(struct snd_soc_dai_link *mt_soc_exthp_dai);
int get_extspk_dai_codec_name(struct snd_soc_dai_link *mt_soc_extspk_dai);

extern int mt_set_gpio_mode(unsigned long pin, unsigned long mode);
extern int mt_set_gpio_dir(unsigned long pin, unsigned long dir);
extern int mt_set_gpio_pull_enable(unsigned long pin, unsigned long enable);
extern int mt_set_gpio_pull_select(unsigned long pin, unsigned long select);

#endif
