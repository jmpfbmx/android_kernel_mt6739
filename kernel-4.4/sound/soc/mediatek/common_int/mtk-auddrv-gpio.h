


#ifndef _AUDDRV_GPIO_H_
#define _AUDDRV_GPIO_H_



#include "mtk-auddrv-def.h"
#include "mtk-soc-digital-type.h"





#include <linux/gpio.h>

void AudDrv_GPIO_probe(void *dev);

int AudDrv_GPIO_Request(bool _enable, enum soc_aud_digital_block _usage);

int AudDrv_GPIO_SMARTPA_Select(int mode);
int AudDrv_GPIO_TDM_Select(int mode);

int AudDrv_GPIO_I2S_Select(int bEnable);
int AudDrv_GPIO_EXTAMP_Select(int bEnable, int mode);
int AudDrv_GPIO_EXTAMP2_Select(int bEnable, int mode);
int AudDrv_GPIO_RCVSPK_Select(int bEnable);
int AudDrv_GPIO_HPDEPOP_Select(int bEnable);

int audio_drv_gpio_aud_clk_pull(bool high);

/* Begin Modified by meng.zhang for task 5424730 on 2017/10/21 */
#ifdef CONFIG_JRD_HAC_EXTERNAL_PA_SUPPORT
int hac_hw_ctrl(bool bEnable, int mode);
#endif
/* End Modified by meng.zhang for task 5424730 on 2017/10/21 */

#endif
