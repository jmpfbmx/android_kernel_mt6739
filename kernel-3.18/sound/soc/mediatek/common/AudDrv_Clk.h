
#ifndef _AUDDRV_CLK_H_
#define _AUDDRV_CLK_H_




#include "AudDrv_Common.h"
#include "AudDrv_Def.h"





#include <linux/clk.h>

extern int AudDrv_Clk_probe(void *dev);
extern void AudDrv_Clk_Deinit(void *dev);

void AudDrv_Clk_Global_Variable_Init(void);
void AudDrv_AUD_Sel(int parentidx);
void AudDrv_AUDINTBUS_Sel(int parentidx);

void AudDrv_Clk_AllOn(void);

void AudDrv_Bus_Init(void);

void AudDrv_Clk_Power_On(void);
void AudDrv_Clk_Power_Off(void);

void AudDrv_Clk_On(void);
void AudDrv_Clk_Off(void);

void AudDrv_ANA_Clk_On(void);
void AudDrv_ANA_Clk_Off(void);

void AudDrv_I2S_Clk_On(void);
void AudDrv_I2S_Clk_Off(void);

void AudDrv_TDM_Clk_On(void);
void AudDrv_TDM_Clk_Off(void);

void AudDrv_Core_Clk_On(void);
void AudDrv_Core_Clk_Off(void);

void AudDrv_ADC_Clk_On(void);
void AudDrv_ADC_Clk_Off(void);
void AudDrv_ADC2_Clk_On(void);
void AudDrv_ADC2_Clk_Off(void);
void AudDrv_ADC3_Clk_On(void);
void AudDrv_ADC3_Clk_Off(void);
void AudDrv_ADC_Hires_Clk_On(void);
void AudDrv_ADC_Hires_Clk_Off(void);


void AudDrv_HDMI_Clk_On(void);
void AudDrv_HDMI_Clk_Off(void);

void AudDrv_Suspend_Clk_On(void);
void AudDrv_Suspend_Clk_Off(void);

void AudDrv_APLL24M_Clk_On(void);
void AudDrv_APLL24M_Clk_Off(void);
void AudDrv_APLL22M_Clk_On(void);
void AudDrv_APLL22M_Clk_Off(void);

void AudDrv_APLL1Tuner_Clk_On(void);
void AudDrv_APLL1Tuner_Clk_Off(void);
void AudDrv_APLL2Tuner_Clk_On(void);
void AudDrv_APLL2Tuner_Clk_Off(void);

void AudDrv_Emi_Clk_On(void);
void AudDrv_Emi_Clk_Off(void);

void AudDrv_ANC_Clk_On(void);
void AudDrv_ANC_Clk_Off(void);

/* APLL , low jitter mode setting */
void EnableALLbySampleRate(uint32 SampleRate);
void DisableALLbySampleRate(uint32 SampleRate);
void EnableI2SDivPower(uint32 Diveder_name, bool bEnable);
void EnableApll1(bool bEnable);
void EnableApll2(bool bEnable);
void SetCLkBclk(uint32 MckDiv, uint32 SampleRate, uint32 Channels, uint32 Wlength);
uint32 SetCLkMclk(uint32 I2snum, uint32 SampleRate);
void EnableI2SCLKDiv(uint32 I2snum, bool bEnable);
void PowerDownAllI2SDiv(void);

#endif
