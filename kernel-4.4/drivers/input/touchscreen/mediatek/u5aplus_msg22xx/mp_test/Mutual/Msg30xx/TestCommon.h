/*
 * Copyright (C) 2006-2017 ILITEK TECHNOLOGY CORP.
 *
 * Description: ILITEK I2C touchscreen driver for linux platform.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Dicky Chiang
 * Maintain: Luca Hsu, Tigers Huang
 */
 
#ifndef __MSG30XX_TESTCOMMON_H__
#define __MSG30XX_TESTCOMMON_H__

#include "../MutualCommon.h"
#include "../../MpCommon.h"

extern u16 _gMsg30xxfout_base_addr;
extern u16 _gMsg30xxScanMode;
extern u16 _gMsg30xxCodeType;
extern u16 _gMsg30xxTwoDACEnable;
extern u16 _gMsg30xxsssqrt_en;

extern void Msg30xxDBBusReEnter(void);
extern void Msg30xxDBBusReadDQMemStart(void);
extern void Msg30xxDBBusReadDQMemEnd(void);
extern void Msg30xxEnableAdcOneShot(void);
extern void Msg30xxAnaChangeCDtime(u16 T1, u16 T2);
extern s32 Msg30xxTriggerWaterProofOneShot(s16 * pResultData, int *nSize);
extern s32 Msg30xxTriggerMutualOneShot(s16 * pResultData, u16 * pSenNum, u16 * pDrvNum,u16 drv_mode);
extern s32 Msg30xxGetMutualOneShotRawIIR(s16 * nResultData, u16 * pSenNum, u16 * pDrvNum, u16 drv_mode);
extern s32 Msg30xxtGetWaterProofOneShotRawIIR(s16 * pRawDataWP, int *nSize);
extern s32 Msg30xxCheckSwitchStatus(void);
extern s32 Msg30xxSwitchFwMode(u16 *nFMode, u16 *deep_standby);
extern void Msg30xxAnaSwReset(void);
extern void Msg30xxAnaFixPrs(u16 option);
extern s32 Msg30xxEnterMpMode(void);
extern u16 Msg30xxGet_Drv_opening(void);
extern void Msg30xxScanDacSetting(u16 nTwoDAC);
extern void Msg30xxTgenOvwrDACEnable(u16 nEnable, u16 eDACSelect);
extern void Msg30xxTgenOvwrRegEn(u16 nEnable);
extern void Msg30xxTgenOvwrInverterMode(u16 nEnable, u16 nTwoDAC);
extern void Msg30xxTgenOvwrDRVLvBufCfbSetting(u16 nEnable, u16 nTwoDAC);
extern void Msg30xxTgenOvwrDRVLvBufGainSetting(u16 nEnable, u16 nTwoDAC);
extern void Msg30xxTgenOvwrDRVHvCompSetting(u16 nEnable);
extern void Msg30xxTgenOvwrROSSelect(u16 nEnable, u16 nTwoDAC);
extern void Msg30xxTgenOvwrDRVHvSetting(u16 nEnable);
extern void Msg30xxTgenOvwrChargePumpSetting(u16 nEnable_charge_pump);
extern void Msg30xxAnaEnableChargePump(u16 nEnable_charge_pump);
extern void Msg30xxGain_Setting(void);
extern void Msg30xxSetSensorPADState(u16 state);
extern void Msg30xxScanSetSensorNum(u16 nSensorNum);
extern void Msg30xxScanSetSubframeNum(u16 nSubframeNum);
extern void Msg30xxScanSetSampleNum(u16 nSampleNum);
extern void Msg30xxSetChipSwapEnable(u16 nSwapEnable);
extern void Msg30xxAdcDespInverterEnable(u16 nEnable);
extern void Msg30xxSetMutualCsubViaDBbus(s16 nCSub);
#endif