
#ifndef _AUDDRV_GPIO_H_
#define _AUDDRV_GPIO_H_



#include "AudDrv_Def.h"
#include "mt_soc_digital_type.h"





#include <linux/gpio.h>

void AudDrv_GPIO_probe(void *dev);

int AudDrv_GPIO_Request(bool _enable, Soc_Aud_Digital_Block _usage);

int AudDrv_GPIO_SMARTPA_Select(int mode);
int AudDrv_GPIO_TDM_Select(int mode);

int AudDrv_GPIO_I2S_Select(int bEnable);
int AudDrv_GPIO_EXTAMP_Select(int bEnable, int mode);
int AudDrv_GPIO_EXTAMP2_Select(int bEnable, int mode);
int AudDrv_GPIO_RCVSPK_Select(int bEnable);
int AudDrv_GPIO_HPDEPOP_Select(int bEnable);

#endif
