
#ifndef __DISP_DTS_GPIO_H__
#define __DISP_DTS_GPIO_H__


#include <linux/platform_device.h>	/* struct platform_device */

/* DTS state */
enum DTS_GPIO_STATE {
	DTS_GPIO_STATE_TE_MODE_GPIO = 0,	/* mode_te_gpio */
	DTS_GPIO_STATE_TE_MODE_TE,	/* mode_te_te */
	DTS_GPIO_STATE_TE1_MODE_TE,	/* mode_te1_te */
	DTS_GPIO_STATE_LCM_RST_OUT0,
	DTS_GPIO_STATE_LCM_RST_OUT1,
	DTS_GPIO_STATE_LCM1_RST_OUT0,
	DTS_GPIO_STATE_LCM1_RST_OUT1,
	DTS_GPIO_STATE_LCD_BIAS_ENN,
	DTS_GPIO_STATE_LCD_BIAS_ENP,
	DTS_GPIO_STATE_MAX,	/* for array size */
};

long disp_dts_gpio_init(struct platform_device *pdev);

long disp_dts_gpio_select_state(enum DTS_GPIO_STATE s);

/* repo of initialization */
#ifdef CONFIG_MTK_LEGACY
#define disp_dts_gpio_init_repo(x)  (0)
#else
#define disp_dts_gpio_init_repo(x)  (disp_dts_gpio_init(x))
#endif

#endif/*__DISP_DTS_GPIO_H__ */
