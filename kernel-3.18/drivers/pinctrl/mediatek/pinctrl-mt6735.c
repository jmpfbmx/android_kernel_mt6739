
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/regmap.h>
#include <dt-bindings/pinctrl/mt65xx.h>
#include "pinctrl-mtk-common.h"
#include "pinctrl-mtk-mt6735.h"


struct mtk_pin_spec_pupd_set {
	unsigned int pin;
	unsigned int offset;
	unsigned char pupd_bit;
	unsigned char r1_bit;
	unsigned char r0_bit;
};

#define MTK_PIN_PUPD_SPEC(_pin, _offset, _pupd, _r1, _r0)	\
	{	\
		.pin = _pin,	\
		.offset = _offset,	\
		.pupd_bit = _pupd,	\
		.r1_bit = _r1,		\
		.r0_bit = _r0,		\
	}

static const struct mtk_pin_spec_pupd_set mt6735_spec_pupd[] = {

	MTK_PIN_PUPD_SPEC(45, 0x980, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(46, 0x980, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(81, 0xA80, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(82, 0xA80, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(83, 0xA80, 10, 9, 8),
	MTK_PIN_PUPD_SPEC(84, 0xA80, 18, 17, 16),
	MTK_PIN_PUPD_SPEC(85, 0xA80, 22, 21, 20),
	MTK_PIN_PUPD_SPEC(86, 0xA80, 26, 25, 24),
	MTK_PIN_PUPD_SPEC(160, 0xC90, 3, 2, 1),
	MTK_PIN_PUPD_SPEC(161, 0xC90, 7, 6, 5),
	MTK_PIN_PUPD_SPEC(162, 0xC90, 11, 10, 9),
	MTK_PIN_PUPD_SPEC(163, 0xC90, 19, 18, 17),
	MTK_PIN_PUPD_SPEC(164, 0xC90, 23, 22, 21),
	MTK_PIN_PUPD_SPEC(165, 0xC90, 27, 26, 25),
	MTK_PIN_PUPD_SPEC(166, 0xC80, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(167, 0xC80, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(168, 0xC80, 10, 9, 8),
	MTK_PIN_PUPD_SPEC(169, 0xC80, 14, 13, 12),
	MTK_PIN_PUPD_SPEC(170, 0xC80, 18, 17, 16),
	MTK_PIN_PUPD_SPEC(171, 0xC80, 22, 21, 20),
	MTK_PIN_PUPD_SPEC(172, 0xD80, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(173, 0xD80, 10, 9, 8),
	MTK_PIN_PUPD_SPEC(174, 0xD80, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(175, 0xD80, 14, 13, 12),
	MTK_PIN_PUPD_SPEC(176, 0xD80, 18, 17, 16),
	MTK_PIN_PUPD_SPEC(177, 0xD80, 22, 21, 20),
	MTK_PIN_PUPD_SPEC(178, 0xD80, 26, 25, 24),
	MTK_PIN_PUPD_SPEC(179, 0xD80, 30, 29, 28),
	MTK_PIN_PUPD_SPEC(180, 0xD90, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(181, 0xD90, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(182, 0xD90, 10, 9, 8),
	MTK_PIN_PUPD_SPEC(183, 0xD90, 14, 13, 12),
	MTK_PIN_PUPD_SPEC(198, 0x880, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(199, 0x880, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(200, 0x880, 10, 9, 8),
	MTK_PIN_PUPD_SPEC(201, 0x880, 14, 13, 12),
	MTK_PIN_PUPD_SPEC(202, 0x880, 18, 17, 16),
	MTK_PIN_PUPD_SPEC(203, 0x880, 22, 21, 20),
	MTK_PIN_PUPD_SPEC(45, 0x980, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(46, 0x980, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(81, 0xA80, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(82, 0xA80, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(83, 0xA80, 10, 9, 8),
	MTK_PIN_PUPD_SPEC(84, 0xA80, 18, 17, 16),
	MTK_PIN_PUPD_SPEC(85, 0xA80, 22, 21, 20),
	MTK_PIN_PUPD_SPEC(86, 0xA80, 26, 25, 24),
	/* MTK_PIN_PUPD_SPEC(160,0xC90,3,2,1), */
	/* MTK_PIN_PUPD_SPEC(161,0xC90,7,6,5), */
	/* MTK_PIN_PUPD_SPEC(162,0xC90,11,10,9), */
	/* MTK_PIN_PUPD_SPEC(163,0xC90,19,18,17), */
	/* MTK_PIN_PUPD_SPEC(164,0xC90,23,22,21), */
	/* MTK_PIN_PUPD_SPEC(165,0xC90,27,26,25), */
	MTK_PIN_PUPD_SPEC(166, 0xC80, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(167, 0xC80, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(168, 0xC80, 10, 9, 8),
	MTK_PIN_PUPD_SPEC(169, 0xC80, 14, 13, 12),
	MTK_PIN_PUPD_SPEC(170, 0xC80, 18, 17, 16),
	MTK_PIN_PUPD_SPEC(171, 0xC80, 22, 21, 20),
	MTK_PIN_PUPD_SPEC(172, 0xD80, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(173, 0xD80, 10, 9, 8),
	MTK_PIN_PUPD_SPEC(174, 0xD80, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(175, 0xD80, 14, 13, 12),
	MTK_PIN_PUPD_SPEC(176, 0xD80, 18, 17, 16),
	MTK_PIN_PUPD_SPEC(177, 0xD80, 22, 21, 20),
	MTK_PIN_PUPD_SPEC(178, 0xD80, 26, 25, 24),
	MTK_PIN_PUPD_SPEC(179, 0xD80, 30, 29, 28),
	MTK_PIN_PUPD_SPEC(180, 0xD90, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(181, 0xD90, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(182, 0xD90, 10, 9, 8),
	MTK_PIN_PUPD_SPEC(183, 0xD90, 14, 13, 12),
	MTK_PIN_PUPD_SPEC(198, 0x880, 2, 1, 0),
	MTK_PIN_PUPD_SPEC(199, 0x880, 6, 5, 4),
	MTK_PIN_PUPD_SPEC(200, 0x880, 10, 9, 8),
	MTK_PIN_PUPD_SPEC(201, 0x880, 14, 13, 12),
	MTK_PIN_PUPD_SPEC(202, 0x880, 18, 17, 16),
	MTK_PIN_PUPD_SPEC(203, 0x880, 22, 21, 20),
};

static int spec_pull_set(struct regmap *regmap, unsigned int pin,
		unsigned char align, bool isup, unsigned int r1r0)
{
	unsigned int i;
	unsigned int reg_pupd, reg_set, reg_rst;
	unsigned int bit_pupd, bit_r0, bit_r1;
	const struct mtk_pin_spec_pupd_set *spec_pupd_pin;
	bool find = false;

	for (i = 0; i < ARRAY_SIZE(mt6735_spec_pupd); i++) {
		if (pin == mt6735_spec_pupd[i].pin) {
			find = true;
			break;
		}
	}

	if (!find)
		return -EINVAL;

	spec_pupd_pin = mt6735_spec_pupd + i;
	reg_set = spec_pupd_pin->offset + align;
	reg_rst = spec_pupd_pin->offset + (align << 1);

	if (isup)
		reg_pupd = reg_rst;
	else
		reg_pupd = reg_set;

	bit_pupd = BIT(spec_pupd_pin->pupd_bit);
	regmap_write(regmap, reg_pupd, bit_pupd);

	bit_r0 = BIT(spec_pupd_pin->r0_bit);
	bit_r1 = BIT(spec_pupd_pin->r1_bit);

	switch (r1r0) {
	case MTK_PUPD_SET_R1R0_00:
		regmap_write(regmap, reg_rst, bit_r0);
		regmap_write(regmap, reg_rst, bit_r1);
		break;
	case MTK_PUPD_SET_R1R0_01:
		regmap_write(regmap, reg_set, bit_r0);
		regmap_write(regmap, reg_rst, bit_r1);
		break;
	case MTK_PUPD_SET_R1R0_10:
		regmap_write(regmap, reg_rst, bit_r0);
		regmap_write(regmap, reg_set, bit_r1);
		break;
	case MTK_PUPD_SET_R1R0_11:
		regmap_write(regmap, reg_set, bit_r0);
		regmap_write(regmap, reg_set, bit_r1);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
static const struct mtk_pinctrl_devdata mt6735_pinctrl_data = {
	.pins = mtk_pins_mt6735,
	.npins = ARRAY_SIZE(mtk_pins_mt6735),
	.grp_desc = NULL,
	.n_grp_cls = 0,
	.pin_drv_grp = NULL,
	.n_pin_drv_grps = 0,
	.spec_pull_set = spec_pull_set,
	/*.spec_ies_smt_set = spec_ies_smt_set,*/
	.spec_set_gpio_mode = mt_set_gpio_mode,
	.mt_set_gpio_dir = mt_set_gpio_dir,
	.mt_get_gpio_dir = mt_get_gpio_dir,
	.mt_get_gpio_out = mt_get_gpio_out,
	.mt_set_gpio_out = mt_set_gpio_out,
	.mt_set_gpio_driving = mt_set_gpio_driving,
	.mt_set_gpio_ies = mt_set_gpio_ies,
	.mt_set_gpio_smt = mt_set_gpio_smt,
	.mt_set_gpio_pull_enable =  mt_set_gpio_pull_enable,
	.mt_set_gpio_pull_select = mt_set_gpio_pull_select,
	.mt_get_gpio_in = mt_get_gpio_in,
	/* .dir_offset = 0x0000, */
	/* .pullen_offset = 0x0100, */
	/* .pullsel_offset = 0x0200, */
	/* .dout_offset = 0x0100, */
	/* .din_offset = 0x0200, */
	/* .pinmux_offset = 0x300, */
	.type1_start = 204,
	.type1_end = 204,
	.port_shf = 4,
	.port_mask = 0xf,
	.port_align = 4,
	.chip_type = MTK_CHIP_TYPE_BASE,
	/*
	.eint_offsets = {
		.name = "mt6735_eint",
		.stat      = 0x000,
		.ack       = 0x040,
		.mask      = 0x080,
		.mask_set  = 0x0c0,
		.mask_clr  = 0x100,
		.sens      = 0x140,
		.sens_set  = 0x180,
		.sens_clr  = 0x1c0,
		.soft      = 0x200,
		.soft_set  = 0x240,
		.soft_clr  = 0x280,
		.pol       = 0x300,
		.pol_set   = 0x340,
		.pol_clr   = 0x380,
		.dom_en    = 0x400,
		.dbnc_ctrl = 0x500,
		.dbnc_set  = 0x600,
		.dbnc_clr  = 0x700,
		.port_mask = 7,
		.ports     = 6,
	},
	.ap_num = 224,
	.db_cnt = 16,
	*/
};

static int mt6735_pinctrl_probe(struct platform_device *pdev)
{
	pr_warn("mt6735 pinctrl probe\n");
	return mtk_pctrl_init(pdev, &mt6735_pinctrl_data, NULL);
}

static struct of_device_id mt6735_pctrl_match[] = {
	{
		.compatible = "mediatek,mt6735-pinctrl",
	}, {
	}
};
MODULE_DEVICE_TABLE(of, mt6735_pctrl_match);

static struct platform_driver mtk_pinctrl_driver = {
	.probe = mt6735_pinctrl_probe,
	.driver = {
		.name = "mediatek-mt6735-pinctrl",
		.owner = THIS_MODULE,
		.of_match_table = mt6735_pctrl_match,
	},
};

static int __init mtk_pinctrl_init(void)
{
	return platform_driver_register(&mtk_pinctrl_driver);
}

/* module_init(mtk_pinctrl_init); */

postcore_initcall(mtk_pinctrl_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MediaTek Pinctrl Driver");
MODULE_AUTHOR("Hongzhou Yang <hongzhou.yang@mediatek.com>");
