
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/i2c.h>
/* alsa sound header */
#include <sound/soc.h>
#include <sound/tlv.h>
#include <sound/pcm_params.h>
#include "mtk-soc-speaker-amp.h"
#if defined(CONFIG_SND_SOC_RT5509)
#include <rt5509.h>
#endif

#define AMP_DEVICE_NAME		"speaker_amp"

static int amp_index;
static struct amp_i2c_control amp_list[AMP_TYPE_NUM] = {
#if defined(CONFIG_SND_SOC_RT5509)
	[RICHTEK_RT5509] = {
		.i2c_probe = rt5509_i2c_probe,
		.i2c_remove = rt5509_i2c_remove,
		.i2c_shutdown = rt5509_i2c_shutdown,
	},
#endif
};

static int speaker_amp_i2c_probe(struct i2c_client *client,
				 const struct i2c_device_id *id)
{
	int i, ret = 0;

	pr_info("%s()\n", __func__);

	amp_index = NOT_SMARTPA;
	for (i = 0; i < AMP_TYPE_NUM; i++) {
		if (amp_list[i].i2c_probe == NULL)
			continue;

		ret = amp_list[i].i2c_probe(client, id);
		if (ret < 0)
			continue;

		amp_index = i;
		break;
	}
	return 0;
}

static int speaker_amp_i2c_remove(struct i2c_client *client)
{
	pr_info("%s()\n", __func__);

	if (amp_list[amp_index].i2c_remove != NULL)
		amp_list[amp_index].i2c_remove(client);

	return 0;
}

static void speaker_amp_i2c_shutdown(struct i2c_client *client)
{
	pr_info("%s()\n", __func__);

	if (amp_list[amp_index].i2c_shutdown != NULL)
		amp_list[amp_index].i2c_shutdown(client);
}

int get_amp_index(void)
{
	return amp_index;
}
EXPORT_SYMBOL(get_amp_index);

static const struct i2c_device_id speaker_amp_i2c_id[] = {
	{ "speaker_amp", 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, speaker_amp_i2c_id);

#ifdef CONFIG_OF
static const struct of_device_id speaker_amp_match_table[] = {
	{.compatible = "mediatek,speaker_amp",},
	{},
};
MODULE_DEVICE_TABLE(of, speaker_amp_match_table);
#endif /* #ifdef CONFIG_OF */

static struct i2c_driver speaker_amp_i2c_driver = {
	.driver = {
		.name = AMP_DEVICE_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(speaker_amp_match_table),
	},
	.probe = speaker_amp_i2c_probe,
	.remove = speaker_amp_i2c_remove,
	.shutdown = speaker_amp_i2c_shutdown,
	.id_table = speaker_amp_i2c_id,
};

module_i2c_driver(speaker_amp_i2c_driver);


MODULE_DESCRIPTION("speaker amp register driver");
MODULE_LICENSE("GPL");

