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
 
#ifndef __MUTUAL_COMMON_H__
#define __MUTUAL_COMMON_H__

#include "../MpCommon.h"

#define DISABLE_DOUBLE

// typedef enum
// {
// 	MUTUAL_MODE = 0x5705,
// 	MUTUAL_SINE = 0x5706,
// 	MUTUAL_KEY = 0x6734,         // latter FW v1007.
// 	MUTUAL_SINE_KEY = 0x6733,    // latter FW v1007.
// 	SELF_MODE = 0x6278,
// 	WATERPROOF_MODE = 0x7992,
// 	MUTUAL_SINGLE_DRIVE_MODE = 0x0158,
// 	SINE_PHASE = 0xECFA,
// 	SET_PHASE,
// 	DEEP_STANDBY = 0x6179,
// 	GET_BG_SUM = 0x7912,
// } MutualFwMode_e;

typedef enum
{
    ONE_DAC_ENABLE = 0,
    TWO_DAC_ENABLE = 1,
} ItoTestDACStatus;

typedef enum
{
    SINGLE = 0,
    MULTI = 1,
} ItoTestScreenType;

typedef enum
{
    eDAC_0,
    eDAC_1,
} ItoTestDAC;

// typedef enum
// {
// 	DISABLE = 0,
// 	ENABLE,
// };

// typedef enum
// {
// 	GND = 0x00,
//     POS_PULSE = 0x01,
//     HIGH_IMPEDENCE = 0x02,
//     NEG_PULSE = 0x03,
// };

// typedef enum
// {
// 	_50p,
// 	_40p,
// 	_30p,
// 	_20p,
// 	_10p
// };

typedef enum
{
	_Msg30xx50p,
	_Msg30xx45p,
	_Msg30xx35p,
	_Msg30xx30p,
	_Msg30xx25p
} ItoTestMsg30xxPin;

// #pragma pack(push)
// #pragma pack(4)

typedef struct
{
    u16 X;
    u16 Y;
} MutualMapping_t;

typedef struct
{
    char * sSupportIC;
    int bmDKVerify;
    int bCurrentTest;
    int bChipVerify;
    int bFWUpdate;
    int bFWTest;
    int bOpenTest;
    int bShortTest;
    int bWpTest;
    int bFunctionTest;
    int bAutoStart;
    int bAutoMation;
    int bTriggerMode;
    int bTSMode;
    int bTSEnable;
    int bPhaseKTest;
} MutualUIConfig_t;

typedef struct
{
    u16 persentDC_VA_Range;
    u16 persentDC_VA_Ratio;

    u16 persentDC_Border_Ratio;
    u16 persentDC_VA_Range_up;
    u16 persentDC_VA_Ratio_up;

    u16 persentDG_Range;
    u16 persentDG_Ratio;
    u16 persentDG_Range_up;
    u16 persentDG_Ratio_up;
    u16 persentWater_DG_Range;
} MutualToast_t;

typedef struct
{
    u16 numKey;
    u16 numKeyLine;
    u16 numDummy;
    u16 numTriangle;
    u16 KeyDrv;
    u16 KEY_CH;
    u16 KeyDrv_o;
    int thrsShort;
    int thrsICpin;
    int thrsOpen;
    int thrsWater;

    u16 numSen;
    u16 numDrv;
    u16 numGr;
    MutualMapping_t * mapping;
} MutualSensor_t;

typedef struct
{
	MutualUIConfig_t UIConfig;
    int logResult;
    int logFWResult;

    int Enable;

    char * ana_version;
    char * project_name;
    char * binname;
    char * versionFW;
    u16 slaveI2cID;
    char * stationNow;
    char * inipassword;
    u16 Mutual_Key;
    u16 Pattern_type;
    u16 Pattern_model;

    int Crc_check;

    MutualSensor_t sensorInfo;
    MutualToast_t ToastInfo;
    int FPC_threshold;
    int KeyDC_threshold;
    int KEY_Timer;
    int Open_test_csub;
    int Open_test_cfb;
    int Open_mode;
    int Open_fixed_carrier;
    int Open_fixed_carrier1;
    int Open_test_chargepump;
    int inverter_mode;
#ifndef DISABLE_DOUBLE
    double Current_threshold;
    double CurrentThreshold_Powerdown;
#else
    int Current_threshold;
    int CurrentThreshold_Powerdown;
#endif

    int OPEN_Charge;
    int OPEN_Dump;
    int SHORT_Charge;
    int SHORT_Dump1;
    int SHORT_Dump2;
    int Water_Charge;
    int Water_Dump;

    int * KeySen;
#ifndef DISABLE_DOUBLE
    double * Goldensample_CH_0_Max_Avg;///[ana26xxM.MAX_MUTUAL_NUM];
    double * Goldensample_CH_0_Max;    ///[ana26xxM.MAX_MUTUAL_NUM];
    double * Goldensample_CH_0;        ///[ana26xxM.MAX_MUTUAL_NUM];
    double * Goldensample_CH_0_Min;    ///[ana26xxM.MAX_MUTUAL_NUM];
    double * Goldensample_CH_0_Min_Avg;///[ana26xxM.MAX_MUTUAL_NUM];

    double * PhaseGolden_Max;    ///[ana26xxM.MAX_MUTUAL_NUM];
    double * PhaseGolden;        ///[ana26xxM.MAX_MUTUAL_NUM];
    double * PhaseGolden_Min;    ///[ana26xxM.MAX_MUTUAL_NUM];

    double * PhaseWaterGolden_Max;
    double * PhaseWaterGolden;
    double * PhaseWaterGolden_Min;
#else
    int * Goldensample_CH_0_Max_Avg;///[ana26xxM.MAX_MUTUAL_NUM];
    int * Goldensample_CH_0_Max;    ///[ana26xxM.MAX_MUTUAL_NUM];
    int * Goldensample_CH_0;        ///[ana26xxM.MAX_MUTUAL_NUM];
    int * Goldensample_CH_0_Min;    ///[ana26xxM.MAX_MUTUAL_NUM];
    int * Goldensample_CH_0_Min_Avg;///[ana26xxM.MAX_MUTUAL_NUM];

    int * PhaseGolden_Max;    ///[ana26xxM.MAX_MUTUAL_NUM];
    int * PhaseGolden;        ///[ana26xxM.MAX_MUTUAL_NUM];
    int * PhaseGolden_Min;    ///[ana26xxM.MAX_MUTUAL_NUM];

    int * PhaseWaterGolden_Max;
    int * PhaseWaterGolden;
    int * PhaseWaterGolden_Min;
#endif

    u16 * PAD2Sense;
    u16 * PAD2Drive;
    u16 * PAD2GR;

    u16 * phase_freq;
    u16 freq_num;
    u16 phase_time;
    u16 band_num;
    u16 * pgd;
    u16 * water_pgd;
    u16 * water_sense;
    u16 * water_drive;
    u16 charge_pump;
    u16 raw_type;
    u16 noise_thd;
    u16 sub_frame;
    u16 afe_num;
    u16 phase_sen_num;
    u16 * phase_sense;
    u16 water_sen_num;
    u16 water_drv_num;
    int update_bin;
    int force_phaseK;
    int update_info;
    int log_phasek;
    int border_drive_phase;
    int sw_calibration;
    u8 phase_version;
    u8 mapping_version;
    int deep_standby;
    int deep_standby_timeout;
    int Open_KeySettingByFW;
} MutualMpTest_t;

typedef struct
{
	int nOpenResult;
	int nShortResult;
	int nWaterProofResult;

	int nRatioAvg_max;
#ifndef DISABLE_DOUBLE
	double nRatioAvg_min;
	double nBorder_RatioAvg_max;
	double nBorder_RatioAvg_min;
#else
	int nRatioAvg_min;
	int nBorder_RatioAvg_max;
	int nBorder_RatioAvg_min;
#endif

    int * pOpenResultData;
    int * pOpenFailChannel;
    int * pOpenRatioFailChannel;
    int * pShortResultData;
#ifndef DISABLE_DOUBLE
    double * pShortRData;
#else
    int * pShortRData;
#endif
    int * pICPinShortResultData;
#ifndef DISABLE_DOUBLE
    double * pICPinShortRData;
#else
    int * pICPinShortRData;
#endif
    int * pICPinChannel;
    int * pShortFailChannel;
    int * pICPinShortFailChannel;
    int * pWaterProofResultData;
    int * pWaterProofFailChannel;

    int *pCheck_Fail;
#ifndef DISABLE_DOUBLE
    double *pResult_DeltaC;
    double *pGolden_CH_Max_Avg;
    double *pGolden_CH_Min_Avg;
    double *pGolden_CH_Max;
    double *pGolden_CH_Min;
    double *pGolden_CH;
#else
    int *pResult_DeltaC;
    int *pGolden_CH_Max_Avg;
    int *pGolden_CH_Min_Avg;
    int *pGolden_CH_Max;
    int *pGolden_CH_Min;
    int *pGolden_CH;
#endif
    char *mapTbl_sec;
} MutualMpTestResult_t;

#define MIN(a,b) (((a)<(b))?(a):(b))
#define	MAX(a,b) (((a)>(b))?(a):(b))

#define NULL_DATA -3240
#define PIN_NO_ERROR 0xFFFF
#define UN_USE_SENSOR 0x5AA5

#define IIR_MAX 32600
#define PIN_UN_USE 0xABCD

#define MAX_CHANNEL_DRV  30
#define MAX_CHANNEL_SEN  20
#define	MAX_CHANNEL_NUM_28XX	60
#define MAX_CHANNEL_NUM_30XX    49
#define MAX_AFE_NUM_30XX    14
#define MAX_AFE_NUM_28XX    13
#define	TEST_ITEM_NUM 8
#define MAX_MUTUAL_NUM  1904 // Camaro : 14AFE * 17DRI * 8SF, Cayenne: 13AFE * 15DRI * 6SF

#define KEY_SEPERATE	0x5566
#define	KEY_COMBINE		0x7788

extern MutualMpTest_t * ptMutualMpTest;
extern MutualMpTestResult_t * ptMutualMpTestResult;

extern MutualMpTest_t * ptMsg30xxMutualMpTest;
extern MutualMpTestResult_t * ptMsg30xxMutualMpTestResult;

#ifndef DISABLE_DOUBLE
extern u8 checkDoubleValueInRange(double nValue, double nMax, double nMin);
#else
extern u8 checkDoubleValueInRange(s32 nValue, s32 nMax, s32 nMin);
#endif
extern u8 CheckValueInRange(s32 nValue, s32 nMax, s32 nMin);
extern int str_to_hex(char *hex_str);

#endif