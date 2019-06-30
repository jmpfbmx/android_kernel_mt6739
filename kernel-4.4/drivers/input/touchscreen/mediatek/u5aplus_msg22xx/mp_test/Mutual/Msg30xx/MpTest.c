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
 
#include "MpTest.h"
#include "../../my_parser/convert_file_op.h"
#include "../../my_parser/my_parser.h"

#define FILENAME_MAX 4096

MutualMpTest_t * ptMsg30xxMutualMpTest;
MutualMpTestResult_t * ptMsg30xxMutualMpTestResult;
int nMsg30xxCount = 0;
u8 nMsg30xxAna_version[100] = {0};
u16 szMapTable_msg30xx[1000][2] = {{0}};
char * iniMsg30xxFilePath = NULL;
u16 _gMsg30xxScanMode = 0;
u16 _gMsg30xxCodeType = 0;
u16 _gMsg30xxfout_base_addr = 0;
u16 _gMsg30xxTwoDACEnable = 0;
u16 _gMsg30xxsssqrt_en = 0;

int Msg30xxLoadIni(char * pFilePath)
{
	//u16 szMapTable[1000][2] = {{0}};
	int i, nSize = 0;
	int nMapSize = 0;
	char str[512]={0};
	long s_to_long = 0;
	int res;
	char *token;

	printk("*** %s() ***\n", __func__);

	iniMsg30xxFilePath = kmalloc(100, GFP_KERNEL);
	strcpy(iniMsg30xxFilePath, pFilePath);

	if(my_parser(iniMsg30xxFilePath) < 0)
	{
         printk("*** %s : failed to parse file = %s *** \n",
		  __func__, iniMsg30xxFilePath);
         return -1;		
	}

    printk("Open %s success! \n\n", iniMsg30xxFilePath);

    ptMsg30xxMutualMpTest= (MutualMpTest_t *)kcalloc(1, sizeof(MutualMpTest_t), GFP_KERNEL);
    ptMsg30xxMutualMpTestResult = (MutualMpTestResult_t *)kcalloc(1, sizeof(MutualMpTestResult_t), GFP_KERNEL);
	token = kmalloc(100, GFP_KERNEL);

	ms_getInidata("UI_CONFIG","OpenTest",str);
	ptMsg30xxMutualMpTest->UIConfig.bOpenTest = ms_atoi(str);
	ms_getInidata("UI_CONFIG","ShortTest",str);
	ptMsg30xxMutualMpTest->UIConfig.bShortTest = ms_atoi(str);
	ms_getInidata("UI_CONFIG","WpTest",str);
	ptMsg30xxMutualMpTest->UIConfig.bWpTest = ms_atoi(str);
	ms_getInidata("UI_CONFIG","ANAGEN_VER",str);
	ptMsg30xxMutualMpTest->ana_version = (char *)kmalloc(FILENAME_MAX * sizeof(char), GFP_KERNEL);
	strcpy(ptMsg30xxMutualMpTest->ana_version, str);
	nMsg30xxCount = ms_ini_split_u8_array(ptMsg30xxMutualMpTest->ana_version, nMsg30xxAna_version);
	DBG(0, "%s: nCount = %d \n", __func__,nMsg30xxCount);

	ms_getInidata("SENSOR","DrvNum",str);
	ptMsg30xxMutualMpTest->sensorInfo.numDrv = ms_atoi(str);
	ms_getInidata("SENSOR","SenNum",str);
	ptMsg30xxMutualMpTest->sensorInfo.numSen = ms_atoi(str);
	ms_getInidata("SENSOR","KeyNum",str);
	ptMsg30xxMutualMpTest->sensorInfo.numKey = ms_atoi(str);
	ms_getInidata("SENSOR","KeyLine",str);
	ptMsg30xxMutualMpTest->sensorInfo.numKeyLine = ms_atoi(str);
	ms_getInidata("SENSOR","GrNum",str);
	ptMsg30xxMutualMpTest->sensorInfo.numGr = ms_atoi(str);

	ms_getInidata("OPEN_TEST_N","CSUB_REF",str);
	ptMsg30xxMutualMpTest->Open_test_csub = ms_atoi(str);
	ms_getInidata("OPEN_TEST_N","CFB_REF",str);
	ptMsg30xxMutualMpTest->Open_test_cfb = ms_atoi(str);
	ms_getInidata("OPEN_TEST_N","OPEN_MODE",str);
	ptMsg30xxMutualMpTest->Open_mode = ms_atoi(str);
	ms_getInidata("OPEN_TEST_N","FIXED_CARRIER",str);
	ptMsg30xxMutualMpTest->Open_fixed_carrier = ms_atoi(str);
	ms_getInidata("OPEN_TEST_N","FIXED_CARRIER1",str);
	ptMsg30xxMutualMpTest->Open_fixed_carrier1 = ms_atoi(str);
	ms_getInidata("OPEN_TEST_N","CHARGE_PUMP",str);
	ptMsg30xxMutualMpTest->Open_test_chargepump = ms_atoi(str);

	ms_getInidata("INFOMATION","MutualKey",str);
	ptMsg30xxMutualMpTest->Mutual_Key = ms_atoi(str);
	ms_getInidata("INFOMATION","Pattern_type",str);
	ptMsg30xxMutualMpTest->Pattern_type = ms_atoi(str);
	ms_getInidata("INFOMATION","1T2R_MODEL",str);
	ptMsg30xxMutualMpTest->Pattern_model = ms_atoi(str);

	ms_getInidata("RULES","DC_Range",str);
	ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Range = ms_atoi(str);
	ms_getInidata("RULES","DC_Range_up",str);
	ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Range_up = ms_atoi(str);
	ms_getInidata("RULES","DC_Ratio",str);
	ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio = ms_atoi(str);
	ms_getInidata("RULES","DC_Ratio_up",str);
	ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio_up = ms_atoi(str);
	ms_getInidata("RULES","DC_Border_Ratio",str);
	ptMsg30xxMutualMpTest->ToastInfo.persentDC_Border_Ratio = ms_atoi(str);

	ms_getInidata("BASIC","DEEP_STANDBY",str);
	ptMsg30xxMutualMpTest->deep_standby = ms_atoi(str);
    DBG(0, "DEEP_STANDBY = %d \n", ptMsg30xxMutualMpTest->deep_standby);
	ms_getInidata("BASIC","DEEP_STANDBY_TIMEOUT",str);
	ptMsg30xxMutualMpTest->deep_standby_timeout = ms_atoi(str);

    if ((ptMsg30xxMutualMpTest->Mutual_Key == 1) && (ptMsg30xxMutualMpTest->Mutual_Key == 2))
	{
		ms_getInidata("SENSOR","KEY_CH",str);
		ptMsg30xxMutualMpTest->sensorInfo.KEY_CH = ms_atoi(str);
	}

	ms_getInidata("OPEN_TEST_N", "KEY_SETTING_BY_FW", str);
	ptMsg30xxMutualMpTest->Open_KeySettingByFW = ms_atoi(str);
	ms_getInidata("OPEN_TEST_N", "INVERT_MODE", str);
	ptMsg30xxMutualMpTest->inverter_mode = ms_atoi(str);

	ms_getInidata("CDTIME", "OPEN_CHARGE", str);
	ptMsg30xxMutualMpTest->OPEN_Charge = ms_atoi(str);
	strcpy(token, str);
	res = kstrtol((const char *)token, 0, &s_to_long);
	if(res == 0)
	 	ptMsg30xxMutualMpTest->OPEN_Charge = s_to_long;	

	ms_getInidata("CDTIME", "OPEN_DUMP", str);
	ptMsg30xxMutualMpTest->OPEN_Dump = ms_atoi(str);
	strcpy(token, str);
	res = kstrtol((const char *)token, 0, &s_to_long);
	if(res == 0)
	 	ptMsg30xxMutualMpTest->OPEN_Dump = s_to_long;


	ms_getInidata("CDTIME", "SHORT_Charge", str);
	ptMsg30xxMutualMpTest->SHORT_Charge = ms_atoi(str);
	strcpy(token, str);
	res = kstrtol((const char *)token, 0, &s_to_long);
	if(res == 0)
	 	ptMsg30xxMutualMpTest->SHORT_Charge = s_to_long;

	ms_getInidata("CDTIME", "SHORT_Dump1", str);
	ptMsg30xxMutualMpTest->SHORT_Dump1 = ms_atoi(str);
	
	strcpy(token, str);
	res = kstrtol((const char *)token, 0, &s_to_long);
	if(res == 0)
	 	ptMsg30xxMutualMpTest->SHORT_Dump1 = s_to_long;

    printk("ANAGEN_VER:    [%s]\n", ptMsg30xxMutualMpTest->ana_version);
    printk("OpenTest:      [%d]\n", ptMsg30xxMutualMpTest->UIConfig.bOpenTest);
    printk("ShortTest:      [%d]\n", ptMsg30xxMutualMpTest->UIConfig.bShortTest);
    printk("WPTest:      [%d]\n", ptMsg30xxMutualMpTest->UIConfig.bWpTest);
    printk("DrvNum:      [%d]\n", ptMsg30xxMutualMpTest->sensorInfo.numDrv);
    printk("SenNum:      [%d]\n", ptMsg30xxMutualMpTest->sensorInfo.numSen);
    printk("KeyNum:      [%d]\n", ptMsg30xxMutualMpTest->sensorInfo.numKey);
    printk("KeyLine:      [%d]\n", ptMsg30xxMutualMpTest->sensorInfo.numKeyLine);
    printk("GrNum:      [%d]\n", ptMsg30xxMutualMpTest->sensorInfo.numGr);
    printk("CSUB_REF:      [%d]\n", ptMsg30xxMutualMpTest->Open_test_csub);
    printk("CFB_REF:      [%d]\n", ptMsg30xxMutualMpTest->Open_test_cfb);
    printk("OPEN_MODE:      [%d]\n", ptMsg30xxMutualMpTest->Open_mode);
    printk("FIXED_CARRIER:      [%d]\n", ptMsg30xxMutualMpTest->Open_fixed_carrier);
    printk("FIXED_CARRIER1:      [%d]\n", ptMsg30xxMutualMpTest->Open_fixed_carrier1);
    printk("CHARGE_PUMP:      [%d]\n", ptMsg30xxMutualMpTest->Open_test_chargepump);
    printk("MutualKey:      [%d]\n", ptMsg30xxMutualMpTest->Mutual_Key);
    printk("KEY_CH:      [%d]\n", ptMsg30xxMutualMpTest->sensorInfo.KEY_CH);
    printk("Pattern_type:      [%d]\n", ptMsg30xxMutualMpTest->Pattern_type);
    printk("Pattern_model:      [%d]\n", ptMsg30xxMutualMpTest->Pattern_model);
    printk("DC_Range:      [%d]\n", ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Range);
    printk("DC_Ratio:      [%d]\n", ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio);
    printk("DC_Range_up:      [%d]\n", ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Range_up);
    printk("DC_Ratio_up:      [%d]\n", ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio_up);
    printk("KEY_SETTING_BY_FW:      [%d]\n", ptMsg30xxMutualMpTest->Open_KeySettingByFW);
    printk("INVERT_MODE:      [%d]\n", ptMsg30xxMutualMpTest->inverter_mode);
    printk("DEEP_STANDBY_TIMEOUT:      [%d]\n", ptMsg30xxMutualMpTest->deep_standby_timeout);
    printk("OPEN_CHARGE:      [%d]\n", ptMsg30xxMutualMpTest->OPEN_Charge);
    printk("OPEN_DUMP:      [%d]\n", ptMsg30xxMutualMpTest->OPEN_Dump);
    printk("SHORT_Charge:      [%d]\n", ptMsg30xxMutualMpTest->SHORT_Charge);
    printk("SHORT_Dump1:      [%d]\n", ptMsg30xxMutualMpTest->SHORT_Dump1);

    if(ptMsg30xxMutualMpTest->sensorInfo.numKey > 0){
		ptMsg30xxMutualMpTest->KeySen = (int *)kcalloc(ptMsg30xxMutualMpTest->sensorInfo.numKey, sizeof(int), GFP_KERNEL);
	    ms_getInidata("SENSOR","KEYSEN",str);
		strcpy((char *)ptMsg30xxMutualMpTest->KeySen, str);
		ptMsg30xxMutualMpTest->sensorInfo.numKey = ms_ini_split_int_array(str, ptMsg30xxMutualMpTest->KeySen);

		ms_getInidata("SENSOR","KEyDrv_o",str);
	    ptMsg30xxMutualMpTest->sensorInfo.KeyDrv_o = ms_atoi(str);
    }

    //nMapSize = IniSecKeysTo2DArray("MAPPING_TABLE_NORMAL", szMapTable);
    if ((nMsg30xxCount) && (nMsg30xxAna_version[2] <= 15)) {
    	// nMapSize = IniSecKeysTo2DArray2(iniMsg30xxFilePath, "MAPPING_TABLE_NORMAL", szMapTable);
    	// ptMsg30xxMutualMpTest->sensorInfo.mapping = (MutualMapping_t *)calloc(nMapSize, sizeof(MutualMapping_t));
		// DBG(0, "mapping number = %d", nMapSize);
		nMapSize = ms_ini_2d_array(iniMsg30xxFilePath, "MAPPING_TABLE_BARKER", szMapTable_msg30xx);
		printk("mapping number = %d\n", nMapSize);
		ptMsg30xxMutualMpTest->sensorInfo.mapping = (MutualMapping_t *)kcalloc(nMapSize, sizeof(MutualMapping_t), GFP_KERNEL);

    	for(i=0; i<nMapSize; i++){
    		ptMsg30xxMutualMpTest->sensorInfo.mapping[i].X = szMapTable_msg30xx[i][0];
    		ptMsg30xxMutualMpTest->sensorInfo.mapping[i].Y = szMapTable_msg30xx[i][1];
		}
	}
    // }

	ptMsg30xxMutualMpTest->UIConfig.sSupportIC = (char *)kmalloc(FILENAME_MAX * sizeof(char), GFP_KERNEL);
	memset(ptMsg30xxMutualMpTest->UIConfig.sSupportIC, 0, sizeof(*ptMsg30xxMutualMpTest->UIConfig.sSupportIC));
	if(ms_getInidata("UI_CONFIG","SupportIC",str) != 0)
		strcpy(ptMsg30xxMutualMpTest->UIConfig.sSupportIC, str);
	
	printk("SupportIC:      [%s]\n", ptMsg30xxMutualMpTest->UIConfig.sSupportIC);

 	ptMsg30xxMutualMpTest->project_name = (char *)kmalloc(FILENAME_MAX * sizeof(char), GFP_KERNEL);
	 memset(ptMsg30xxMutualMpTest->project_name, 0, sizeof(*ptMsg30xxMutualMpTest->project_name));
	if(ms_getInidata("INFOMATION", "PROJECT", str) != 0)
		strcpy(ptMsg30xxMutualMpTest->project_name, str);

	printk("PROJECT:      [%s]\n", ptMsg30xxMutualMpTest->project_name);

#ifdef DISABLE_DOUBLE
    ptMsg30xxMutualMpTest->Goldensample_CH_0 = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL);
	nSize = 0;
	nSize = ms_ini_split_golden(ptMsg30xxMutualMpTest->Goldensample_CH_0, 26);
	printk("%s: nSize = %d \n", __func__,nSize);
#else
	ptMsg30xxMutualMpTest->Goldensample_CH_0 = (double *)calloc(MAX_MUTUAL_NUM, sizeof(double)); //(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	nSize = 0;
	nSize = IniGetDoubleArray("RULES","Golden_CH_0",ptMsg30xxMutualMpTest->Goldensample_CH_0);
#endif

#ifdef DISABLE_DOUBLE
	ptMsg30xxMutualMpTest->Goldensample_CH_0_Max = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL); 
	ptMsg30xxMutualMpTest->Goldensample_CH_0_Max_Avg = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL);
	ptMsg30xxMutualMpTest->Goldensample_CH_0_Min = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL);
	ptMsg30xxMutualMpTest->Goldensample_CH_0_Min_Avg = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL);
#else
	ptMsg30xxMutualMpTest->Goldensample_CH_0_Max = (double *)calloc(MAX_MUTUAL_NUM, sizeof(double));
	ptMsg30xxMutualMpTest->Goldensample_CH_0_Max_Avg = (double *)calloc(MAX_MUTUAL_NUM, sizeof(double));
	ptMsg30xxMutualMpTest->Goldensample_CH_0_Min = (double *)calloc(MAX_MUTUAL_NUM, sizeof(double));
	ptMsg30xxMutualMpTest->Goldensample_CH_0_Min_Avg = (double *)calloc(MAX_MUTUAL_NUM, sizeof(double));
#endif

	if (ptMsg30xxMutualMpTest->sensorInfo.numDrv && ptMsg30xxMutualMpTest->sensorInfo.numSen)
	{
		ptMsg30xxMutualMpTest->PAD2Drive = (u16 *)kmalloc(ptMsg30xxMutualMpTest->sensorInfo.numDrv * sizeof(u16), GFP_KERNEL);
		ms_getInidata("PAD_TABLE","DRIVE",str);
		printk("PAD_TABLE(DRIVE):      [%s]\n", str);
		nSize = 0;
		nSize = ms_ini_split_u16_array(str, ptMsg30xxMutualMpTest->PAD2Drive);

		ptMsg30xxMutualMpTest->PAD2Sense = (u16 *)kmalloc(ptMsg30xxMutualMpTest->sensorInfo.numSen * sizeof(u16), GFP_KERNEL);
		ms_getInidata("PAD_TABLE","SENSE",str);
		printk("PAD_TABLE(SENSE):      [%s]\n", str);
		nSize = 0;
		nSize = ms_ini_split_u16_array(str, ptMsg30xxMutualMpTest->PAD2Sense);

		printk("PAD2Sense\n\n");
		DebugShowArray2(ptMsg30xxMutualMpTest->PAD2Sense, ptMsg30xxMutualMpTest->sensorInfo.numSen, 16, 16, ptMsg30xxMutualMpTest->sensorInfo.numSen);;
	}

	if (ptMsg30xxMutualMpTest->sensorInfo.numGr)
	{
		ptMsg30xxMutualMpTest->PAD2GR = (u16 *)kmalloc(ptMsg30xxMutualMpTest->sensorInfo.numGr * sizeof(u16), GFP_KERNEL);
		ms_getInidata("PAD_TABLE","GR",str);
		printk("PAD_TABLE(GR):      [%s]\n", str);
		nSize = 0;
		nSize = ms_ini_split_u16_array(str, ptMsg30xxMutualMpTest->PAD2GR);
	}

	ms_getInidata("RULES","SHORTVALUE",str);
	ptMsg30xxMutualMpTest->sensorInfo.thrsShort = ms_atoi(str);
	printk("SHORTVALUE:      [%d]\n", ptMsg30xxMutualMpTest->sensorInfo.thrsShort);

	ms_getInidata("RULES","ICPINSHORT",str);
	ptMsg30xxMutualMpTest->sensorInfo.thrsICpin = ms_atoi(str);
	printk("ICPINSHORT:      [%d]\n", ptMsg30xxMutualMpTest->sensorInfo.thrsICpin);

	kfree(token);

	return 0;
}

#ifdef DISABLE_DOUBLE
void Msg30xxcalGoldenRange(int *goldenTbl, u16 weight, u16 weight_up, int *maxTbl, int *minTbl, int length)
#else
void Msg30xxcalGoldenRange(double *goldenTbl, u16 weight, u16 weight_up, double *maxTbl, double *minTbl, int length)
#endif
{
#ifdef DISABLE_DOUBLE
    int value = 0, value_up = 0;
#else
	double value = 0, value_up = 0;
#endif
    int i;

    for (i = 0; i < length; i++)
    {
#ifdef DISABLE_DOUBLE
    	value = (int)weight * abs(goldenTbl[i]) / 100;
        value_up = (int)weight_up * abs(goldenTbl[i]) / 100;
#else
    	value = (double)weight * abs(goldenTbl[i]) / 100;
        value_up = (double)weight_up * abs(goldenTbl[i]) / 100;
#endif
        maxTbl[i] = goldenTbl[i] + value + value_up;
        minTbl[i] = goldenTbl[i] - value;
    }
}

int Msg30xxMPTestResult(int nResultType, int * pResult)
{
    int nResultSize = 0;

	switch(nResultType) {

		case  MPTEST_RESULT:
			pResult[OPEN_TEST_RESULT] = ptMsg30xxMutualMpTestResult->nOpenResult;
			pResult[SHORT_TEST_RESULT] = ptMsg30xxMutualMpTestResult->nShortResult;
			pResult[WATERPROOF_TEST_RESULT] = ptMsg30xxMutualMpTestResult->nWaterProofResult;
			nResultSize = 3;
		    break;

		case  MPTEST_SCOPE:
			pResult[0] = ptMsg30xxMutualMpTest->sensorInfo.numSen;
			pResult[1] = ptMsg30xxMutualMpTest->sensorInfo.numDrv;
			pResult[2] = ptMsg30xxMutualMpTest->sensorInfo.numKey;
			nResultSize = 3;
		    break;

		case  OPEN_TEST_DATA:
			pResult[0] = ptMsg30xxMutualMpTest->sensorInfo.numDrv * ptMsg30xxMutualMpTest->sensorInfo.numSen;
            memcpy(&pResult[1], ptMsg30xxMutualMpTestResult->pOpenResultData, sizeof(int) * pResult[0]);
			nResultSize = pResult[0]+1;
		    break;

		case  OPEN_TEST_FAIL_CHANNEL:
			pResult[0] = MAX_MUTUAL_NUM;
            memcpy(&pResult[1], ptMsg30xxMutualMpTestResult->pOpenFailChannel, sizeof(int) * pResult[0]);
			nResultSize = pResult[0]+1;
			break;

		case  SHORT_TEST_DATA:
		    break;

		case  SHORT_TEST_FAIL_CHANNEL:
			break;

		case  WATERPROOF_TEST_DATA:
		    break;

		case  WATERPROOF_TEST_FAIL_CHANNEL:
			break;

		default:
			break;
	}

	return nResultSize;
}

int Msg30xxMPTestResultD(int nResultType, double * pResult)
{
    int nResultSize = 0;

	switch(nResultType) {
		case  SHORT_TEST_DATA:
		    break;

		default:
			break;
	}

	return nResultSize;
}

void Msg30xxEndMPTest(void)
{
	printk("*** %s() ***\n", __func__);

	if(ptMsg30xxMutualMpTest->KeySen != NULL){
		DBG(0, "** free ptMsg30xxMutualMpTest->KeySen \n***");
		kfree(ptMsg30xxMutualMpTest->KeySen);
	}

	if(ptMsg30xxMutualMpTest->sensorInfo.mapping != NULL){
		DBG(0, "** free ptMsg30xxMutualMpTest->sensorInfo.mapping \n***");
		kfree(ptMsg30xxMutualMpTest->sensorInfo.mapping);
	}

	if(ptMsg30xxMutualMpTest->ana_version != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTest->ana_version \n***");
		kfree(ptMsg30xxMutualMpTest->ana_version);
	}

	if(ptMsg30xxMutualMpTest->project_name != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTest->project_name \n***");
		kfree(ptMsg30xxMutualMpTest->project_name);
	}

	if(ptMsg30xxMutualMpTest->Goldensample_CH_0 != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTest->Goldensample_CH_0 \n***");
		kfree(ptMsg30xxMutualMpTest->Goldensample_CH_0);
	}

	if(ptMsg30xxMutualMpTest->Goldensample_CH_0_Max != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTest->Goldensample_CH_0_Max \n***");
		kfree(ptMsg30xxMutualMpTest->Goldensample_CH_0_Max);
	}

	if(ptMsg30xxMutualMpTest->Goldensample_CH_0_Max_Avg != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTest->Goldensample_CH_0_Max_Avg \n***");
		kfree(ptMsg30xxMutualMpTest->Goldensample_CH_0_Max_Avg);
	}

	if(ptMsg30xxMutualMpTest->Goldensample_CH_0_Min != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTest->Goldensample_CH_0_Min \n***");
		kfree(ptMsg30xxMutualMpTest->Goldensample_CH_0_Min);
	}

	if(ptMsg30xxMutualMpTest->Goldensample_CH_0_Min_Avg != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTest->Goldensample_CH_0_Min_Avg \n***");
		kfree(ptMsg30xxMutualMpTest->Goldensample_CH_0_Min_Avg);
	}

	if (ptMsg30xxMutualMpTest->PAD2Drive != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTest->PAD2Drive \n***");
		kfree(ptMsg30xxMutualMpTest->PAD2Drive);
	}

	if (ptMsg30xxMutualMpTest->PAD2Sense != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTest->PAD2Sense \n***");
		kfree(ptMsg30xxMutualMpTest->PAD2Sense);
	}

	if (ptMsg30xxMutualMpTest->PAD2GR != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTest->PAD2GR \n***");
		kfree(ptMsg30xxMutualMpTest->PAD2GR);
	}

	if(ptMsg30xxMutualMpTestResult->pCheck_Fail != NULL){
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pCheck_Fail \n***");
		kfree(ptMsg30xxMutualMpTestResult->pCheck_Fail);
	}

	if(ptMsg30xxMutualMpTestResult->pOpenResultData != NULL){
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pOpenResultData \n***");
		kfree(ptMsg30xxMutualMpTestResult->pOpenResultData);
	}

	if(ptMsg30xxMutualMpTestResult->pOpenFailChannel != NULL){
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pOpenFailChannel \n***");
		kfree(ptMsg30xxMutualMpTestResult->pOpenFailChannel);
	}

	if(ptMsg30xxMutualMpTestResult->pOpenRatioFailChannel != NULL){
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pOpenRatioFailChannel \n***");
		kfree(ptMsg30xxMutualMpTestResult->pOpenRatioFailChannel);
	}

	if(ptMsg30xxMutualMpTestResult->pGolden_CH != NULL){
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pGolden_CH \n***");
		kfree(ptMsg30xxMutualMpTestResult->pGolden_CH);
	}

	if(ptMsg30xxMutualMpTestResult->pGolden_CH_Max != NULL){
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pGolden_CH_Max \n***");
		kfree(ptMsg30xxMutualMpTestResult->pGolden_CH_Max);
	}

	if(ptMsg30xxMutualMpTestResult->pGolden_CH_Max_Avg != NULL){
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pGolden_CH_Max_Avg \n***");
		kfree(ptMsg30xxMutualMpTestResult->pGolden_CH_Max_Avg);
	}

	if(ptMsg30xxMutualMpTestResult->pGolden_CH_Min != NULL){
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pGolden_CH_Min \n***");
		kfree(ptMsg30xxMutualMpTestResult->pGolden_CH_Min);
	}

	if(ptMsg30xxMutualMpTestResult->pGolden_CH_Min_Avg != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pGolden_CH_Min_Avg \n***");
		kfree(ptMsg30xxMutualMpTestResult->pGolden_CH_Min_Avg);
	}

	if (ptMsg30xxMutualMpTestResult->pICPinChannel != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pICPinChannel \n***");
		kfree(ptMsg30xxMutualMpTestResult->pICPinChannel);
	}

	if (ptMutualMpTestResult->pICPinChannel != NULL) {
		DBG(0,"** free ptMutualMpTestResult->pICPinChannel \n***");
		kfree(ptMutualMpTestResult->pICPinChannel);
	}

	if (ptMsg30xxMutualMpTestResult->pShortFailChannel != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pShortFailChannel \n***");
		kfree(ptMsg30xxMutualMpTestResult->pShortFailChannel);
	}

	if (ptMsg30xxMutualMpTestResult->pICPinShortFailChannel != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pICPinShortFailChannel \n***");
		kfree(ptMsg30xxMutualMpTestResult->pICPinShortFailChannel);
	}

	if (ptMsg30xxMutualMpTestResult->pShortResultData != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pShortResultData \n***");
		kfree(ptMsg30xxMutualMpTestResult->pShortResultData);
	}

	if (ptMsg30xxMutualMpTestResult->pShortRData) {
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pShortRData \n***");
		kfree(ptMsg30xxMutualMpTestResult->pShortRData);
	}

	if (ptMsg30xxMutualMpTestResult->pICPinShortResultData != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pICPinShortResultData \n***");
		kfree(ptMsg30xxMutualMpTestResult->pICPinShortResultData);
	}

	if (ptMsg30xxMutualMpTestResult->pICPinShortRData != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pICPinShortRData \n***");
		kfree(ptMsg30xxMutualMpTestResult->pICPinShortRData);
	}

	if (ptMsg30xxMutualMpTestResult->pWaterProofResultData != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTestResult->pWaterProofResultData \n***");
		kfree(ptMsg30xxMutualMpTestResult->pWaterProofResultData);
	}

	if (ptMsg30xxMutualMpTestResult->mapTbl_sec != NULL) {
		DBG(0, "** free ptMsg30xxMutualMpTestResult->mapTbl_sec \n***");
		kfree(ptMsg30xxMutualMpTestResult->mapTbl_sec);
	}

	if (ptMsg30xxMutualMpTest!= NULL)
	{
		kfree(ptMsg30xxMutualMpTest);
		ptMsg30xxMutualMpTest= NULL;
	}

	if (ptMsg30xxMutualMpTestResult != NULL)
	{
		kfree(ptMsg30xxMutualMpTestResult);
		ptMsg30xxMutualMpTestResult = NULL;
	}
	//IniFree();
	my_parser_exit();
}

u16 Msg30xxgetFirmwareOfficialVersionOnFlash(void)
{
	u8 *info_fw_ver;
	int official_ver = 0;
	u8 nChipType = 0;

	DBG(0, "*** %s() ***\n", __func__);

	info_fw_ver = (u8 *)kcalloc(1, 4, GFP_KERNEL);

	EnterDBBus();
    StopMCU();

	nChipType = RegGet16BitValue(0x1ECC) & 0xFF;
	ReadFlash(nChipType,0x7f8,EMEM_TYPE_INFO_BLOCK,4,info_fw_ver);
	DBG(0, "info_fw_ver[0] = 0x%02x, info_fw_ver[1] = 0x%02x, info_fw_ver[2] = 0x%02x, info_fw_ver[3] = 0x%02x\n"
	,info_fw_ver[0], info_fw_ver[1], info_fw_ver[2], info_fw_ver[3]);

	info_fw_ver[1] = 0;

	official_ver = str_to_hex((char *) info_fw_ver);
	kfree(info_fw_ver);
	StartMCU();
	return (u16)official_ver;
}

int Msg30xxInitVariables(void)
{
	int i;
	int nMapSize;

	if ((nMsg30xxCount) && ((nMsg30xxAna_version[0] == 1 && nMsg30xxAna_version[2] > 15) || (nMsg30xxAna_version[0] == 2 && nMsg30xxAna_version[2] >= 0)) ) {
		DrvDisableFingerTouchReport();
		DrvTouchDeviceHwReset();
		EnterDBBus();
		if (!Msg30xxEnterMpMode()) {	// to get key combine or key seperate scan mode
			mdelay(100);
			StopMCU();

			//mapTbl_sec = (char *)kmalloc(FILENAME_MAX * sizeof(char), GFP_KERNEL);
			ptMsg30xxMutualMpTestResult->mapTbl_sec = (char *)kmalloc(FILENAME_MAX * sizeof(char), GFP_KERNEL);

			if (_gMsg30xxTwoDACEnable == ENABLE)
			{
				if (_gMsg30xxScanMode == KEY_COMBINE)
					sprintf(ptMsg30xxMutualMpTestResult->mapTbl_sec, "MAPPING_TABLE_TWO_DAC_BARKER%d_KEY", Msg30xxGet_Drv_opening());
				else if (_gMsg30xxScanMode == KEY_SEPERATE)
					sprintf(ptMsg30xxMutualMpTestResult->mapTbl_sec, "MAPPING_TABLE_TWO_DAC_BARKER%d", Msg30xxGet_Drv_opening());
				else
					sprintf(ptMsg30xxMutualMpTestResult->mapTbl_sec, "MAPPING_TABLE_TWO_DAC_BARKER%d", Msg30xxGet_Drv_opening());
			}
			else
			{
				if (_gMsg30xxScanMode == KEY_COMBINE)
					sprintf(ptMsg30xxMutualMpTestResult->mapTbl_sec, "MAPPING_TABLE_BARKER%d_KEY", Msg30xxGet_Drv_opening());
				else if (_gMsg30xxScanMode == KEY_SEPERATE)
					sprintf(ptMsg30xxMutualMpTestResult->mapTbl_sec, "MAPPING_TABLE_BARKER%d", Msg30xxGet_Drv_opening());
				else
					sprintf(ptMsg30xxMutualMpTestResult->mapTbl_sec, "MAPPING_TABLE_BARKER%d", Msg30xxGet_Drv_opening());
			}

			DBG(0, "mapping table name = %s, code length = %d, scan mode = 0x%x\n", ptMsg30xxMutualMpTestResult->mapTbl_sec, Msg30xxGet_Drv_opening(), _gMsg30xxScanMode);

			// nMapSize = IniSecKeysTo2DArray2(iniMsg30xxFilePath, mapTbl_sec, szMapTable_msg30xx);
			// ptMsg30xxMutualMpTest->sensorInfo.mapping = (MutualMapping_t *)calloc(nMapSize, sizeof(MutualMapping_t));

			nMapSize = ms_ini_2d_array(iniMsg30xxFilePath, "MAPPING_TABLE_BARKER", szMapTable_msg30xx);
			DBG(0, "mapping number = %d\n", nMapSize);

			if(nMapSize != 0)
			{
				ptMsg30xxMutualMpTest->sensorInfo.mapping = (MutualMapping_t *)kcalloc(nMapSize, sizeof(MutualMapping_t), GFP_KERNEL);
				for(i=0; i<nMapSize; i++)
				{
					ptMsg30xxMutualMpTest->sensorInfo.mapping[i].X = szMapTable_msg30xx[i][0];
					ptMsg30xxMutualMpTest->sensorInfo.mapping[i].Y = szMapTable_msg30xx[i][1];
				}
			}
			else
			{
				return -1;
			}
		}
	}
	Msg30xxcalGoldenRange(ptMsg30xxMutualMpTest->Goldensample_CH_0,
		ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Range, ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Range_up,
		ptMsg30xxMutualMpTest->Goldensample_CH_0_Max, ptMsg30xxMutualMpTest->Goldensample_CH_0_Min, MAX_MUTUAL_NUM);

	ptMsg30xxMutualMpTestResult->pCheck_Fail = (int *)kmalloc(sizeof(int) * TEST_ITEM_NUM, GFP_KERNEL);
	ptMsg30xxMutualMpTestResult->pOpenResultData = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL);
	ptMsg30xxMutualMpTestResult->pOpenFailChannel = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL);
	ptMsg30xxMutualMpTestResult->pOpenRatioFailChannel = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );

	#ifdef DISABLE_DOUBLE
	ptMsg30xxMutualMpTestResult->pGolden_CH = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );
	ptMsg30xxMutualMpTestResult->pGolden_CH_Max = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );
	ptMsg30xxMutualMpTestResult->pGolden_CH_Max_Avg = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );
	ptMsg30xxMutualMpTestResult->pGolden_CH_Min = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );
	ptMsg30xxMutualMpTestResult->pGolden_CH_Min_Avg = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );

	ptMsg30xxMutualMpTestResult->nRatioAvg_max = (int) (100 + ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio + ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio_up) / 100;
	ptMsg30xxMutualMpTestResult->nRatioAvg_min =(int) (100 - ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio) / 100;
	ptMsg30xxMutualMpTestResult->nBorder_RatioAvg_max = (int) (100 + ptMsg30xxMutualMpTest->ToastInfo.persentDC_Border_Ratio + ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio_up) / 100;
	ptMsg30xxMutualMpTestResult->nBorder_RatioAvg_min = (int) (100 - ptMsg30xxMutualMpTest->ToastInfo.persentDC_Border_Ratio) / 100;
	#else
	ptMsg30xxMutualMpTestResult->pGolden_CH = (double *)calloc(MAX_MUTUAL_NUM, sizeof(double) );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double) );
	ptMsg30xxMutualMpTestResult->pGolden_CH_Max = (double *)calloc(MAX_MUTUAL_NUM, sizeof(double) );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMsg30xxMutualMpTestResult->pGolden_CH_Max_Avg = (double *)calloc(MAX_MUTUAL_NUM, sizeof(double) );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMsg30xxMutualMpTestResult->pGolden_CH_Min = (double *)calloc(MAX_MUTUAL_NUM, sizeof(double) );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMsg30xxMutualMpTestResult->pGolden_CH_Min_Avg = (double *)calloc(MAX_MUTUAL_NUM, sizeof(double) );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));

	ptMsg30xxMutualMpTestResult->nRatioAvg_max = (double) (100 + ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio + ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio_up) / 100;
	ptMsg30xxMutualMpTestResult->nRatioAvg_min =(double) (100 - ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio) / 100;
	ptMsg30xxMutualMpTestResult->nBorder_RatioAvg_max = (double) (100 + ptMsg30xxMutualMpTest->ToastInfo.persentDC_Border_Ratio + ptMsg30xxMutualMpTest->ToastInfo.persentDC_VA_Ratio_up) / 100;
	ptMsg30xxMutualMpTestResult->nBorder_RatioAvg_min = (double) (100 - ptMsg30xxMutualMpTest->ToastInfo.persentDC_Border_Ratio) / 100;
	#endif

	ptMsg30xxMutualMpTestResult->pShortFailChannel = (int *)kcalloc(MAX_CHANNEL_NUM_30XX, sizeof(int), GFP_KERNEL);
	ptMsg30xxMutualMpTestResult->pShortResultData = (int *)kcalloc(MAX_CHANNEL_NUM_30XX, sizeof(int), GFP_KERNEL);
	ptMsg30xxMutualMpTestResult->pICPinChannel = (int *)kcalloc(MAX_CHANNEL_NUM_30XX, sizeof(int), GFP_KERNEL);
	ptMsg30xxMutualMpTestResult->pICPinShortFailChannel = (int *)kcalloc(MAX_CHANNEL_NUM_30XX, sizeof(int), GFP_KERNEL);
	ptMsg30xxMutualMpTestResult->pICPinShortResultData = (int *)kcalloc(MAX_CHANNEL_NUM_30XX, sizeof(int), GFP_KERNEL);

#ifdef DISABLE_DOUBLE
	ptMsg30xxMutualMpTestResult->pICPinShortRData = (int *)kcalloc(MAX_CHANNEL_NUM_30XX, sizeof(int), GFP_KERNEL);
	ptMsg30xxMutualMpTestResult->pShortRData = (int *)kcalloc(MAX_CHANNEL_NUM_30XX, sizeof(int), GFP_KERNEL);
#else
	ptMsg30xxMutualMpTestResult->pICPinShortRData = (double *)calloc(MAX_CHANNEL_NUM_30XX, sizeof(double));//(double *)malloc(MAX_CHANNEL_NUM_30XX * sizeof(double));
	   ptMsg30xxMutualMpTestResult->pShortRData = (double *)calloc(MAX_CHANNEL_NUM_30XX, sizeof(double));//(double *)malloc(MAX_CHANNEL_NUM_30XX * sizeof(double));
#endif

	return 0;
}

void Msg30xxStartMPTest(void)
{
	int i;
//	u16 regData = 0;
	u16 fw_ver = 0;

	printk("*** %s() ***\n", __func__);

    DrvDisableFingerTouchReport();
    DrvTouchDeviceHwReset();
    EnterDBBus();
	fw_ver = Msg30xxgetFirmwareOfficialVersionOnFlash();
	if (fw_ver == 0xFF)
		fw_ver = 0;
	DBG(0, "info fw_ver = %x\n", fw_ver);

	if(Msg30xxInitVariables() < 0)
		goto out;

    if(ptMsg30xxMutualMpTest->UIConfig.bOpenTest == 1){ 	

    	ptMsg30xxMutualMpTestResult->nOpenResult = Msg30xxOpenTest(fw_ver);

    	for (i = 0; i < MAX_MUTUAL_NUM; i++)
    	{
    		ptMsg30xxMutualMpTestResult->pGolden_CH[i] = ptMsg30xxMutualMpTest->Goldensample_CH_0[i];
    		ptMsg30xxMutualMpTestResult->pGolden_CH_Max[i] = ptMsg30xxMutualMpTest->Goldensample_CH_0_Max[i];
    		ptMsg30xxMutualMpTestResult->pGolden_CH_Min[i] = ptMsg30xxMutualMpTest->Goldensample_CH_0_Min[i];
    	}
    }
    else {
    	ptMsg30xxMutualMpTestResult->nOpenResult = ITO_NO_TEST;
    }

    if(ptMsg30xxMutualMpTest->UIConfig.bShortTest == 1){

    	ptMsg30xxMutualMpTestResult->nShortResult = Msg30xxShortTest();
    }
    else {
    	ptMsg30xxMutualMpTestResult->nShortResult = ITO_NO_TEST;
    }

    if(ptMsg30xxMutualMpTest->UIConfig.bWpTest == 1){
    	ptMsg30xxMutualMpTestResult->pWaterProofResultData = (int *)kmalloc(sizeof(int) * 8, GFP_KERNEL);
    	//ptMsg30xxMutualMpTestResult->nWaterProofResult = Msg30xxWaterProofTest();
    }
    else {
    	ptMsg30xxMutualMpTestResult->nWaterProofResult = ITO_NO_TEST;
	}

out:
	DrvTouchDeviceHwReset();
	mdelay(300);
	DrvEnableFingerTouchReport();
	kfree(iniMsg30xxFilePath);
	return;
}