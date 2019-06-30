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

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/kobject.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/string.h>
#include <linux/ctype.h>

#ifndef __MP_COMMON_H__
#define __MP_COMMON_H__

#include "../ilitek_drv_common.h"
#include "my_parser/my_parser.h"
#include "my_parser/CreateCSV.h"
#include "Mutual/Msg28xx/MpTest.h"
#include "Mutual/Msg30xx/MpTest.h"
//#include "../mstar_drv_platform_porting_layer.h"
//#include "../mstar_drv_utility_adaption.h"

// Chip Id
#define CHIP_TYPE_MSG21XX   (0x01) // EX. MSG2133
#define CHIP_TYPE_MSG21XXA  (0x02) // EX. MSG2133A/MSG2138A(Besides, use version to distinguish MSG2133A/MSG2138A, you may refer to _DrvFwCtrlUpdateFirmwareCash()) 
#define CHIP_TYPE_MSG26XXM  (0x03) // EX. MSG2633M
#define CHIP_TYPE_MSG22XX   (0x7A) // EX. MSG2238/MSG2256
#define CHIP_TYPE_MSG28XX   (0x85) // EX. MSG2833/MSG2835/MSG2836/MSG2840/MSG2856/MSG5846
#define CHIP_TYPE_MSG28XXA   (0xBF) // EX. MSG2856
#define CHIP_TYPE_MSG58XXA  (0xBF) // EX. MSG5846A
#define CHIP_TYPE_ILI2117A  (0x2117) // EX. ILI2117A
#define CHIP_TYPE_ILI2118A  (0x2118) // EX. ILI2118A
#define CHIP_TYPE_ILI2121   (0x2121) // EX. ILI2121
#define CHIP_TYPE_ILI2120   (0x2120) // (0) // EX. ILI2120

#define EMEM_SIZE_MSG28XX (1024*130)
#define EMEM_SIZE_MSG22XX ((1024*48) + 512 )
#define EMEM_TYPE_ALL_BLOCKS 	0x00
#define EMEM_TYPE_MAIN_BLOCK 	0x01
#define EMEM_TYPE_INFO_BLOCK 	0x02

//MPTest Result Items
#define MPTEST_RESULT                   0
#define MPTEST_SCOPE                    1
#define OPEN_TEST_DATA                  2
#define OPEN_TEST_FAIL_CHANNEL          3
#define SHORT_TEST_DATA                 4
#define SHORT_TEST_FAIL_CHANNEL         5
#define WATERPROOF_TEST_DATA            6
#define WATERPROOF_TEST_FAIL_CHANNEL    7

//MPTest Result
#define ITO_NO_TEST                   0
#define ITO_TEST_OK                   1
#define ITO_TEST_FAIL                 2
#define ITO_TEST_GET_TP_TYPE_ERROR    3
#define ITO_TEST_UNDEFINED_ERROR      4
#define ITO_TEST_PROCESSING		      5

//int[] location in Report(MPTEST_RESULT)
#define OPEN_TEST_RESULT          0
#define SHORT_TEST_RESULT         1
#define WATERPROOF_TEST_RESULT    2


struct _TP_INFO
{
	char DriverVersion[32];
	char FwVersion[32];
	char PlatformVersion[32];
	char MainBlockFWVersion[32];
	char InfoBlockFWVersion[32];
	char ChipType;
};

// declaired at MsgControl.c
extern void EnterDBBus(void);
extern void ExitDBBus(void);
extern void StartMCU(void);
extern void StopMCU(void);
extern s32 EnterMpMode(void);
extern s32 Msg28xxCheckSwitchStatus(void);
extern s32 Msg28xxSwitchFwMode(u16 *nFMode, u16 *deep_standby);
extern int ReadFlash(u8 nChipType,u32 nAddr,int nBlockType,int nLength,u8 *pFlashData);

// declaired at MsgLib.c
extern int getFirmwareVersionOnFlash(void);
extern int startMPTest(int nChipType, char *FilePath);
extern void EndMpTest(int nChipType);

// declaired at MsgUtility.c
extern void RegGet16BitByteValueBuf(u16 nAddr, u8 *pBuf, u16 nLen);
extern void DebugShowArray2(void *pBuf, u16 nLen, int nDataType, int nCarry, int nChangeLine);



#endif