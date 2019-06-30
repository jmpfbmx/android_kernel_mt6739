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
 
#ifndef __MSG28XX_TESTCOMMON_H__
#define __MSG28XX_TESTCOMMON_H__

#include "../MutualCommon.h"

extern s32 Msg28xxtGetWaterProofOneShotRawIIR(s16 * pRawDataWP, int *nSize);
extern s32 Msg28xxGetMutualOneShotRawIIR(s16 * nResultData, u16 * pSenNum, u16 * pDrvNum);
extern void Msg28xxEnableAdcOneShot(void);
extern void Msg28xxDBBusReEnter(void);
extern void Msg28xxDBBusReadDQMemEnd(void);
extern void Msg28xxDBBusReadDQMemStart(void);
extern void Msg28xxAnaChangeCDtime(u16 T1, u16 T2);
extern void Msg28xxAnaSwReset(void);
extern s32 Msg28xxGetMutualOneShotRawIIR(s16 * nResultData, u16 * pSenNum, u16 * pDrvNum);

#endif