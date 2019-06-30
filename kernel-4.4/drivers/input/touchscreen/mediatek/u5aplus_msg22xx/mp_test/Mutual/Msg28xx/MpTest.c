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

MutualMpTest_t * ptMutualMpTest;
MutualMpTestResult_t * ptMutualMpTestResult;
int nCount = 0;
u8 nAna_version[100] = {0};
u16 szMapTable[1000][2] = {{0}};
char * iniFilePath = NULL;

extern struct i2c_client *g_I2cClient;

int Msg28xxLoadIni(char * pFilePath)
{
//    int nOpenTest;
//    int nShortTest;    
    int nMapSize = 0;
	int nSize;
    int i;
	char str[512]={0};

    DBG(&g_I2cClient->dev, "*** %s : iniPath = %s *** \n", __func__, pFilePath);

    // if (IniLoad(pFilePath) == 0)
    // {
    //     DBG(&g_I2cClient->dev, "*** %s : failed to parse file = %s *** \n", __func__, pFilePath);
    //     return -1;
    // }

	iniFilePath = kmalloc(100, GFP_KERNEL);
	strcpy(iniFilePath, pFilePath);

	if(my_parser(iniFilePath) < 0)
	{
         printk("*** %s : failed to parse file = %s *** \n", __func__, pFilePath);
         return -1;		
	}

    DBG(&g_I2cClient->dev,"Open %s success! \n\n", iniFilePath);

    ptMutualMpTest = (MutualMpTest_t *)kcalloc(1, sizeof(MutualMpTest_t), GFP_KERNEL);//(MutualMpTest_t *)malloc(sizeof(MutualMpTest_t));
    ptMutualMpTestResult = (MutualMpTestResult_t *)kcalloc(1, sizeof(MutualMpTestResult_t), GFP_KERNEL);//(MutualMpTestResult_t *)

	ms_getInidata("UI_CONFIG","OpenTest",str);
	ptMutualMpTest->UIConfig.bOpenTest = ms_atoi(str);
	ms_getInidata("UI_CONFIG","ShortTest",str);
	ptMutualMpTest->UIConfig.bShortTest = ms_atoi(str);
	ms_getInidata("UI_CONFIG","WpTest",str);
	ptMutualMpTest->UIConfig.bWpTest = ms_atoi(str);
	ms_getInidata("UI_CONFIG","ANAGEN_VER",str);
	ptMutualMpTest->ana_version = (char *)kmalloc(FILENAME_MAX * sizeof(char), GFP_KERNEL);
	strcpy(ptMutualMpTest->ana_version, str);
	nCount = ms_ini_split_u8_array(ptMutualMpTest->ana_version, nAna_version);
	DBG(&g_I2cClient->dev, "%s: nCount = %d \n", __func__,nCount);

	ms_getInidata("SENSOR","DrvNum",str);
	ptMutualMpTest->sensorInfo.numDrv = ms_atoi(str);
	ms_getInidata("SENSOR","SenNum",str);
	ptMutualMpTest->sensorInfo.numSen = ms_atoi(str);
	ms_getInidata("SENSOR","KeyNum",str);
	ptMutualMpTest->sensorInfo.numKey = ms_atoi(str);
	ms_getInidata("SENSOR","KeyLine",str);
	ptMutualMpTest->sensorInfo.numKeyLine = ms_atoi(str);
	ms_getInidata("SENSOR","GrNum",str);
	ptMutualMpTest->sensorInfo.numGr = ms_atoi(str);

	ms_getInidata("OPEN_TEST_N","CSUB_REF",str);
	ptMutualMpTest->Open_test_csub = ms_atoi(str);
	ms_getInidata("OPEN_TEST_N","CFB_REF",str);
	ptMutualMpTest->Open_test_cfb = ms_atoi(str);
	ms_getInidata("OPEN_TEST_N","OPEN_MODE",str);
	ptMutualMpTest->Open_mode = ms_atoi(str);
	ms_getInidata("OPEN_TEST_N","FIXED_CARRIER",str);
	ptMutualMpTest->Open_fixed_carrier = ms_atoi(str);
	ms_getInidata("OPEN_TEST_N","CHARGE_PUMP",str);
	ptMutualMpTest->Open_test_chargepump = ms_atoi(str);

	ms_getInidata("INFOMATION","MutualKey",str);
	ptMutualMpTest->Mutual_Key = ms_atoi(str);
	ms_getInidata("INFOMATION","Pattern_type",str);
	ptMutualMpTest->Pattern_type = ms_atoi(str);
	ms_getInidata("INFOMATION","1T2R_MODEL",str);
	ptMutualMpTest->Pattern_model = ms_atoi(str);

	ms_getInidata("RULES","DC_Range",str);
	ptMutualMpTest->ToastInfo.persentDC_VA_Range = ms_atoi(str);
	ms_getInidata("RULES","DC_Range_up",str);
	ptMutualMpTest->ToastInfo.persentDC_VA_Range_up = ms_atoi(str);
	ms_getInidata("RULES","DC_Ratio",str);
	ptMutualMpTest->ToastInfo.persentDC_VA_Ratio = ms_atoi(str);
	ms_getInidata("RULES","DC_Ratio_up",str);
	ptMutualMpTest->ToastInfo.persentDC_VA_Ratio_up = ms_atoi(str);
	ms_getInidata("RULES","DC_Border_Ratio",str);
	ptMutualMpTest->ToastInfo.persentDC_Border_Ratio = ms_atoi(str);

	ms_getInidata("BASIC","DEEP_STANDBY",str);
	ptMutualMpTest->deep_standby = ms_atoi(str);
    DBG(&g_I2cClient->dev, "DEEP_STANDBY = %d \n", ptMutualMpTest->deep_standby);

	ms_getInidata("BASIC","DEEP_STANDBY_TIMEOUT",str);
	ptMutualMpTest->deep_standby_timeout = ms_atoi(str);
	
    if ((ptMutualMpTest->Mutual_Key == 1) || (ptMutualMpTest->Mutual_Key == 2))
	{
		ms_getInidata("SENSOR","KEY_CH",str);
		ptMutualMpTest->sensorInfo.KEY_CH = ms_atoi(str);
	}

	ms_getInidata("PHASE_CALIBRATION","SUB_FRAME",str);
	ptMutualMpTest->sub_frame = ms_atoi(str);

    // ptMutualMpTest->UIConfig.bOpenTest = IniGetInt("[UI_CONFIG]", "OpenTest");
    // ptMutualMpTest->UIConfig.bShortTest = IniGetInt("[UI_CONFIG]", "ShortTest");
    // ptMutualMpTest->UIConfig.bWpTest = IniGetInt("[UI_CONFIG]", "WpTest");
    // ptMutualMpTest->ana_version = (char *)kmalloc(FILENAME_MAX * sizeof(char), GFP_KERNEL);
    // strcpy(ptMutualMpTest->ana_version, IniGetString("[UI_CONFIG]", "ANAGEN_VER"));
    // nCount = IniSplitU8Array("[UI_CONFIG]", "ANAGEN_VER", nAna_version);

    // ptMutualMpTest->sensorInfo.numDrv = (u16)IniGetInt("[SENSOR]", "DrvNum");
    // ptMutualMpTest->sensorInfo.numSen = (u16)IniGetInt("[SENSOR]", "SenNum");
    // ptMutualMpTest->sensorInfo.numKey = (u16)IniGetInt("[SENSOR]", "KeyNum");
    // ptMutualMpTest->sensorInfo.numKeyLine = (u16)IniGetInt("[SENSOR]", "KeyLine");
    // ptMutualMpTest->sensorInfo.numGr = (u16)IniGetInt("[SENSOR]", "GrNum");
    // ptMutualMpTest->Open_test_csub = IniGetInt("[OPEN_TEST_N]", "CSUB_REF");
    // ptMutualMpTest->Open_test_cfb = IniGetInt("[OPEN_TEST_N]", "CFB_REF");
    // ptMutualMpTest->Open_mode = IniGetInt("[OPEN_TEST_N]", "OPEN_MODE");
    // ptMutualMpTest->Open_fixed_carrier = IniGetInt("[OPEN_TEST_N]", "FIXED_CARRIER");
    // ptMutualMpTest->Open_test_chargepump = IniGetInt("[OPEN_TEST_N]", "CHARGE_PUMP");
    // ptMutualMpTest->Mutual_Key = IniGetInt("[INFOMATION]", "MutualKey");
    // ptMutualMpTest->Pattern_type = IniGetInt("[INFOMATION]", "Pattern_type");
    // ptMutualMpTest->Pattern_model = IniGetInt("[INFOMATION]", "1T2R_MODEL");
    // ptMutualMpTest->ToastInfo.persentDC_VA_Range = (u16)IniGetInt("[RULES]", "DC_Range");
    // ptMutualMpTest->ToastInfo.persentDC_VA_Range_up = (u16)IniGetInt("[RULES]", "DC_Range_up");
    // ptMutualMpTest->ToastInfo.persentDC_VA_Ratio = (u16)IniGetInt("[RULES]", "DC_Ratio");
    // ptMutualMpTest->ToastInfo.persentDC_VA_Ratio_up = (u16)IniGetInt("[RULES]", "DC_Ratio_up");
    // ptMutualMpTest->ToastInfo.persentDC_Border_Ratio = (u16)IniGetInt("[RULES[", "DC_Border_Ratio");
    // ptMutualMpTest->deep_standby = (u16)IniGetInt("[BASIC]", "DEEP_STANDBY");
    // printk("DEEP_STANDBY = %d \n", ptMutualMpTest->deep_standby);
    // ptMutualMpTest->deep_standby_timeout = (u16)IniGetInt("[BASIC]", "DEEP_STANDBY_TIMEOUT");
    // if ((ptMutualMpTest->Mutual_Key == 1) && (ptMutualMpTest->Mutual_Key == 2))
    // 	ptMutualMpTest->sensorInfo.KEY_CH = (u16)IniGetInt("[SENSOR]", "KEY_CH");

    // ptMutualMpTest->sub_frame = (u16)IniGetInt("[PHASE_CALIBRATION]", "SUB_FRAME");

    printk("ANAGEN_VER:    [%s]\n", ptMutualMpTest->ana_version);
    printk("OpenTest:      [%d]\n", ptMutualMpTest->UIConfig.bOpenTest);
    printk("ShortTest:      [%d]\n", ptMutualMpTest->UIConfig.bShortTest);
    printk("WPTest:      [%d]\n", ptMutualMpTest->UIConfig.bWpTest);
    printk("DrvNum:      [%d]\n", ptMutualMpTest->sensorInfo.numDrv);
    printk("SenNum:      [%d]\n", ptMutualMpTest->sensorInfo.numSen);
    printk("KeyNum:      [%d]\n", ptMutualMpTest->sensorInfo.numKey);
    printk("KeyLine:      [%d]\n", ptMutualMpTest->sensorInfo.numKeyLine);
    printk("GrNum:      [%d]\n", ptMutualMpTest->sensorInfo.numGr);
    printk("CSUB_REF:      [%d]\n", ptMutualMpTest->Open_test_csub);
    printk("CFB_REF:      [%d]\n", ptMutualMpTest->Open_test_cfb);
    printk("OPEN_MODE:      [%d]\n", ptMutualMpTest->Open_mode);
    printk("FIXED_CARRIER:      [%d]\n", ptMutualMpTest->Open_fixed_carrier);
    printk("CHARGE_PUMP:      [%d]\n", ptMutualMpTest->Open_test_chargepump);
    printk("MutualKey:      [%d]\n", ptMutualMpTest->Mutual_Key);
    printk("KEY_CH:      [%d]\n", ptMutualMpTest->sensorInfo.KEY_CH);
    printk("Pattern_type:      [%d]\n", ptMutualMpTest->Pattern_type);
    printk("Pattern_model:      [%d]\n", ptMutualMpTest->Pattern_model);
    printk("DC_Range:      [%d]\n", ptMutualMpTest->ToastInfo.persentDC_VA_Range);
    printk("DC_Ratio:      [%d]\n", ptMutualMpTest->ToastInfo.persentDC_VA_Ratio);
    printk("DC_Range_up:      [%d]\n", ptMutualMpTest->ToastInfo.persentDC_VA_Range_up);
    printk("DC_Ratio_up:      [%d]\n", ptMutualMpTest->ToastInfo.persentDC_VA_Ratio_up);
    printk("SUB_FRAME:      [%d]\n", ptMutualMpTest->sub_frame);

    if(ptMutualMpTest->sensorInfo.numKey > 0){
		ptMutualMpTest->KeySen = (int *)kcalloc(ptMutualMpTest->sensorInfo.numKey, sizeof(int), GFP_KERNEL);
	    ms_getInidata("SENSOR","KEYSEN",str);
		strcpy((char *)ptMutualMpTest->KeySen, str);
		ptMutualMpTest->sensorInfo.numKey = ms_ini_split_int_array(str, ptMutualMpTest->KeySen);

		ms_getInidata("SENSOR","KEyDrv_o",str);
	    ptMutualMpTest->sensorInfo.KeyDrv_o = ms_atoi(str);
    }

     if ((nCount) && (nAna_version[2] <= 15)) {
     	//nMapSize = IniSecKeysTo2DArray2(iniFilePath, "MAPPING_TABLE_NORMAL", szMapTable);
     	//ptMutualMpTest->sensorInfo.mapping = (MutualMapping_t *)kcalloc(nMapSize, sizeof(MutualMapping_t), GFP_KERNEL);
     	//printk("mapping number = %d\n", nMapSize);
		nMapSize = ms_ini_2d_array(iniFilePath, "MAPPING_TABLE_BARKER", szMapTable);
		printk("mapping number = %d\n", nMapSize);
		ptMutualMpTest->sensorInfo.mapping = (MutualMapping_t *)kcalloc(nMapSize, sizeof(MutualMapping_t), GFP_KERNEL);
		 
		for(i=0; i<nMapSize; i++){
     		ptMutualMpTest->sensorInfo.mapping[i].X = szMapTable[i][0];
     		ptMutualMpTest->sensorInfo.mapping[i].Y = szMapTable[i][1];
     	}
     }

	ptMutualMpTest->UIConfig.sSupportIC = (char *)kmalloc(FILENAME_MAX * sizeof(char), GFP_KERNEL);
	memset(ptMutualMpTest->UIConfig.sSupportIC, 0, sizeof(*ptMutualMpTest->UIConfig.sSupportIC));
	if(ms_getInidata("UI_CONFIG","SupportIC",str) != 0)
		strcpy(ptMutualMpTest->UIConfig.sSupportIC, str);
	
	printk("SupportIC:      [%s]\n", ptMutualMpTest->UIConfig.sSupportIC);

 	ptMutualMpTest->project_name = (char *)kmalloc(FILENAME_MAX * sizeof(char), GFP_KERNEL);
	 memset(ptMutualMpTest->project_name, 0, sizeof(*ptMutualMpTest->project_name));
	if(ms_getInidata("INFOMATION", "PROJECT", str) != 0)
		strcpy(ptMutualMpTest->project_name, str);

	printk("PROJECT:      [%s]\n", ptMutualMpTest->project_name);
    
#ifndef DISABLE_DOUBLE
	ptMutualMpTest->Goldensample_CH_0 = (double *)kcalloc(MAX_MUTUAL_NUM, sizeof(double), GFP_KERNEL); //(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	nSize = 0;
	nSize = IniGetDoubleArray("[RULES]","Golden_CH_0",ptMutualMpTest->Goldensample_CH_0);
#else
    ptMutualMpTest->Goldensample_CH_0 = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL); //(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	nSize = 0;
	//nSize = IniGetDoubleArray("[RULES]","Golden_CH_0",ptMutualMpTest->Goldensample_CH_0);	
	//ms_getInidata("RULES","Golden_CH_0",str);
	//printk("%s: s = %s \n", __func__,str);
	nSize = ms_ini_split_golden(ptMutualMpTest->Goldensample_CH_0, 24);
	DBG(0,"%s: nSize = %d \n", __func__,nSize);
#endif

#ifndef DISABLE_DOUBLE
	ptMutualMpTest->Goldensample_CH_0_Max = (double *)kcalloc(MAX_MUTUAL_NUM, sizeof(double), GFP_KERNEL); //(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTest->Goldensample_CH_0_Max_Avg = (double *)kcalloc(MAX_MUTUAL_NUM, sizeof(double), GFP_KERNEL);//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTest->Goldensample_CH_0_Min = (double *)kcalloc(MAX_MUTUAL_NUM, sizeof(double), GFP_KERNEL);//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTest->Goldensample_CH_0_Min_Avg = (double *)kcalloc(MAX_MUTUAL_NUM, sizeof(double), GFP_KERNEL);//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
#else
	ptMutualMpTest->Goldensample_CH_0_Max = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL); //(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTest->Goldensample_CH_0_Max_Avg = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL);//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTest->Goldensample_CH_0_Min = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL);//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTest->Goldensample_CH_0_Min_Avg = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL);//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
#endif

	if (ptMutualMpTest->sensorInfo.numDrv && ptMutualMpTest->sensorInfo.numSen)
	{
		ptMutualMpTest->PAD2Drive = (u16 *)kmalloc(ptMutualMpTest->sensorInfo.numDrv * sizeof(u16), GFP_KERNEL);
		ms_getInidata("PAD_TABLE","DRIVE",str);
		DBG(0,"PAD_TABLE(DRIVE):      [%s]\n", str);
		nSize = 0;
		nSize = ms_ini_split_u16_array(str, ptMutualMpTest->PAD2Drive);

		ptMutualMpTest->PAD2Sense = (u16 *)kmalloc(ptMutualMpTest->sensorInfo.numSen * sizeof(u16), GFP_KERNEL);
		ms_getInidata("PAD_TABLE","SENSE",str);
		DBG(0,"PAD_TABLE(SENSE):      [%s]\n", str);
		nSize = 0;
		nSize = ms_ini_split_u16_array(str, ptMutualMpTest->PAD2Sense);

		DBG(0,"PAD2Sense\n\n");
		DebugShowArray2(ptMutualMpTest->PAD2Sense, ptMutualMpTest->sensorInfo.numSen, 16, 16, ptMutualMpTest->sensorInfo.numSen);;
	}

	if (ptMutualMpTest->sensorInfo.numGr)
	{
		ptMutualMpTest->PAD2GR = (u16 *)kmalloc(ptMutualMpTest->sensorInfo.numGr * sizeof(u16), GFP_KERNEL);
		ms_getInidata("PAD_TABLE","GR",str);
		DBG(0,"PAD_TABLE(GR):      [%s]\n", str);
		nSize = 0;
		nSize = ms_ini_split_u16_array(str, ptMutualMpTest->PAD2GR);
	}

	ms_getInidata("RULES","SHORTVALUE",str);
	ptMutualMpTest->sensorInfo.thrsShort = ms_atoi(str);
	DBG(0,"SHORTVALUE:      [%d]\n", ptMutualMpTest->sensorInfo.thrsShort);

	ms_getInidata("RULES","ICPINSHORT",str);
	ptMutualMpTest->sensorInfo.thrsICpin = ms_atoi(str);
	DBG(0,"ICPINSHORT:      [%d]\n", ptMutualMpTest->sensorInfo.thrsICpin);

	return 0;
}

#ifndef DISABLE_DOUBLE
void calGoldenRange(double *goldenTbl, u16 weight, u16 weight_up, double *maxTbl, double *minTbl, int length)
#else
void calGoldenRange(int *goldenTbl, u16 weight, u16 weight_up, int *maxTbl, int *minTbl, int length)
#endif
{
#ifndef DISABLE_DOUBLE
    double value = 0, value_up = 0;
#else
    int value = 0, value_up = 0;
#endif
    int i;

    DBG(0," %s \n", __func__);

    for (i = 0; i < length; i++)
    {
#ifndef DISABLE_DOUBLE
    	value = (double)weight * abs(goldenTbl[i]) / 100;
        value_up = (double)weight_up * abs(goldenTbl[i]) / 100;
#else
    	value = (int)weight * abs(goldenTbl[i]) / 100;
        value_up = (int)weight_up * abs(goldenTbl[i]) / 100;
#endif
        maxTbl[i] = goldenTbl[i] + value + value_up;
        minTbl[i] = goldenTbl[i] - value;
    }
}

int Msg28xxMPTestResult(int nResultType, int * pResult)
{
    int nResultSize = 0;

    DBG(0,"*** %s() ***\n", __func__);

	switch(nResultType) {

		case  MPTEST_RESULT:
			pResult[OPEN_TEST_RESULT] = ptMutualMpTestResult->nOpenResult;
			pResult[SHORT_TEST_RESULT] = ptMutualMpTestResult->nShortResult;
			pResult[WATERPROOF_TEST_RESULT] = ptMutualMpTestResult->nWaterProofResult;
			nResultSize = 3;
		    break;

		case  MPTEST_SCOPE:
			pResult[0] = ptMutualMpTest->sensorInfo.numSen;
			pResult[1] = ptMutualMpTest->sensorInfo.numDrv;
			pResult[2] = ptMutualMpTest->sensorInfo.numKey;
			nResultSize = 3;
		    break;

		case  OPEN_TEST_DATA:
			pResult[0] = ptMutualMpTest->sensorInfo.numDrv * ptMutualMpTest->sensorInfo.numSen;
            memcpy(&pResult[1], ptMutualMpTestResult->pOpenResultData, sizeof(int) * pResult[0]);
			nResultSize = pResult[0]+1;
		    break;

		case  OPEN_TEST_FAIL_CHANNEL:
			pResult[0] = MAX_MUTUAL_NUM;
            memcpy(&pResult[1], ptMutualMpTestResult->pOpenFailChannel, sizeof(int) * pResult[0]);
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

int Msg28xxMPTestResultD(int nResultType, double * pResult)
{
	int nResultSize = 0;

    DBG(0,"*** %s() ***\n", __func__);

	switch(nResultType) {
		case  SHORT_TEST_DATA:
		    break;

		default:
			break;
	}

	return nResultSize;
}

void Msg28xxEndMPTest(void)
{
	DBG(0,"*** %s() ***\n", __func__);

	if(ptMutualMpTest->KeySen != NULL){
		DBG(0,"** free ptMutualMpTest->KeySen \n***");
	    kfree(ptMutualMpTest->KeySen);
	}

	if(ptMutualMpTest->sensorInfo.mapping != NULL){
		DBG(0,"** free ptMutualMpTest->sensorInfo.mapping \n***");
		kfree(ptMutualMpTest->sensorInfo.mapping);
	}

	if(ptMutualMpTest->ana_version != NULL) {
		DBG(0,"** free ptMutualMpTest->ana_version \n***");
		kfree(ptMutualMpTest->ana_version);
	}

	if(ptMutualMpTest->project_name != NULL) {
		DBG(0,"** free ptMutualMpTest->project_name \n***");
		kfree(ptMutualMpTest->project_name);
	}

	if(ptMutualMpTest->Goldensample_CH_0 != NULL) {
		DBG(0,"** free ptMutualMpTest->Goldensample_CH_0 \n***");
		kfree(ptMutualMpTest->Goldensample_CH_0);
	}

	if(ptMutualMpTest->Goldensample_CH_0_Max != NULL) {
		DBG(0,"** free ptMutualMpTest->Goldensample_CH_0_Max \n***");
		kfree(ptMutualMpTest->Goldensample_CH_0_Max);
	}

	if(ptMutualMpTest->Goldensample_CH_0_Max_Avg != NULL) {
		DBG(0,"** free ptMutualMpTest->Goldensample_CH_0_Max_Avg \n***");
		kfree(ptMutualMpTest->Goldensample_CH_0_Max_Avg);
	}

	if(ptMutualMpTest->Goldensample_CH_0_Min != NULL) {
		DBG(0,"** free ptMutualMpTest->Goldensample_CH_0_Min \n***");
		kfree(ptMutualMpTest->Goldensample_CH_0_Min);
	}

	if(ptMutualMpTest->Goldensample_CH_0_Min_Avg != NULL) {
		DBG(0,"** free ptMutualMpTest->Goldensample_CH_0_Min_Avg \n***");
		kfree(ptMutualMpTest->Goldensample_CH_0_Min_Avg);
	}

	if (ptMutualMpTest->PAD2Drive != NULL) {
		DBG(0,"** free ptMutualMpTest->PAD2Drive \n***");
		kfree(ptMutualMpTest->PAD2Drive);
	}

	if (ptMutualMpTest->PAD2Sense != NULL) {
		DBG(0,"** free ptMutualMpTest->PAD2Sense \n***");
		kfree(ptMutualMpTest->PAD2Sense);
	}

	if (ptMutualMpTest->PAD2GR != NULL) {
		DBG(0,"** free ptMutualMpTest->PAD2GR \n***");
		kfree(ptMutualMpTest->PAD2GR);
	}

	if(ptMutualMpTestResult->pCheck_Fail != NULL){
		DBG(0,"** free ptMutualMpTestResult->pCheck_Fail \n***");
		kfree(ptMutualMpTestResult->pCheck_Fail);
	}

	if(ptMutualMpTestResult->pOpenResultData != NULL){
		DBG(0,"** free ptMutualMpTestResult->pOpenResultData \n***");
		kfree(ptMutualMpTestResult->pOpenResultData);
	}

	if(ptMutualMpTestResult->pOpenFailChannel != NULL){
		DBG(0,"** free ptMutualMpTestResult->pOpenFailChannel \n***");
		kfree(ptMutualMpTestResult->pOpenFailChannel);
	}

	if(ptMutualMpTestResult->pOpenRatioFailChannel != NULL){
		DBG(0,"** free ptMutualMpTestResult->pOpenRatioFailChannel \n***");
		kfree(ptMutualMpTestResult->pOpenRatioFailChannel);
	}

	if(ptMutualMpTestResult->pGolden_CH != NULL){
		DBG(0,"** free ptMutualMpTestResult->pGolden_CH \n***");
		kfree(ptMutualMpTestResult->pGolden_CH);
	}

	if(ptMutualMpTestResult->pGolden_CH_Max != NULL){
		DBG(0,"** free ptMutualMpTestResult->pGolden_CH_Max \n***");
		kfree(ptMutualMpTestResult->pGolden_CH_Max);
	}

	if(ptMutualMpTestResult->pGolden_CH_Max_Avg != NULL){
		DBG(0,"** free ptMutualMpTestResult->pGolden_CH_Max_Avg \n***");
		kfree(ptMutualMpTestResult->pGolden_CH_Max_Avg);
	}

	if(ptMutualMpTestResult->pGolden_CH_Min != NULL){
		DBG(0,"** free ptMutualMpTestResult->pGolden_CH_Min \n***");
		kfree(ptMutualMpTestResult->pGolden_CH_Min);
	}

	if (ptMutualMpTestResult->pICPinChannel != NULL) {
		DBG(0,"** free ptMutualMpTestResult->pICPinChannel \n***");
		kfree(ptMutualMpTestResult->pICPinChannel);
	}

	if(ptMutualMpTestResult->pGolden_CH_Min_Avg != NULL) {
		DBG(0,"** free ptMutualMpTestResult->pGolden_CH_Min_Avg \n***");
		kfree(ptMutualMpTestResult->pGolden_CH_Min_Avg);
	}

	if (ptMutualMpTestResult->pICPinChannel != NULL) {
		DBG(0,"** free ptMutualMpTestResult->pICPinChannel \n***");
		kfree(ptMutualMpTestResult->pICPinChannel);
	}

	if (ptMutualMpTestResult->pShortFailChannel != NULL) {
		DBG(0,"** free ptMutualMpTestResult->pShortFailChannel \n***");
		kfree(ptMutualMpTestResult->pShortFailChannel);
	}

	if (ptMutualMpTestResult->pICPinShortFailChannel != NULL) {
		DBG(0,"** free ptMutualMpTestResult->pICPinShortFailChannel \n***");
		kfree(ptMutualMpTestResult->pICPinShortFailChannel);
	}

	if (ptMutualMpTestResult->pShortResultData != NULL) {
		DBG(0,"** free ptMutualMpTestResult->pShortResultData \n***");
		kfree(ptMutualMpTestResult->pShortResultData);
	}

	if (ptMutualMpTestResult->pShortRData) {
		DBG(0,"** free ptMutualMpTestResult->pShortRData \n***");
		kfree(ptMutualMpTestResult->pShortRData);
	}

	if (ptMutualMpTestResult->pICPinShortResultData != NULL) {
		DBG(0,"** free ptMutualMpTestResult->pICPinShortResultData \n***");
		kfree(ptMutualMpTestResult->pICPinShortResultData);
	}

	if (ptMutualMpTestResult->pICPinShortRData != NULL) {
		DBG(0,"** free ptMutualMpTestResult->pICPinShortRData \n***");
		kfree(ptMutualMpTestResult->pICPinShortRData);
	}

	if (ptMutualMpTestResult->pWaterProofResultData != NULL) {
		DBG(0,"** free ptMutualMpTestResult->pWaterProofResultData \n***");
		kfree(ptMutualMpTestResult->pWaterProofResultData);
	}

	if (ptMutualMpTestResult->mapTbl_sec != NULL) {
		DBG(0,"** free ptMutualMpTestResult->mapTbl_sec \n***");
		kfree(ptMutualMpTestResult->mapTbl_sec);
	}

	if (ptMutualMpTest != NULL)
	{
		kfree(ptMutualMpTest);
		ptMutualMpTest = NULL;
	}

	if (ptMutualMpTestResult != NULL)
	{
		kfree(ptMutualMpTestResult);
		ptMutualMpTestResult = NULL;
	}
	//IniFree();
	my_parser_exit();
}

u16 getFirmwareOfficialVersionOnFlash(void)
{
	u8 *info_fw_ver;
	int official_ver = 0;
	u8 nChipType = 0;

	DBG(0,"*** %s() ***\n", __func__);

    info_fw_ver = (u8 *)kcalloc(1, 4, GFP_KERNEL);

	EnterDBBus();
    StopMCU();

	nChipType = RegGet16BitValue(0x1ECC) & 0xFF;
	ReadFlash(nChipType,0x7f8,EMEM_TYPE_INFO_BLOCK,4,info_fw_ver);
	DBG(0,"info_fw_ver[0] = %x, info_fw_ver[1] = %x, info_fw_ver[2] = %x, info_fw_ver[3] = %x \n",info_fw_ver[0], info_fw_ver[1], info_fw_ver[2], info_fw_ver[3]);
	info_fw_ver[1] = 0;

	official_ver = str_to_hex((char *) info_fw_ver);
	kfree(info_fw_ver);
	StartMCU();
	return (u16)official_ver;
}

int Msg28xxInitVariables(void)
{
	int i, nMapSize = 0;
	u16 regData = 0;

	if ((nCount) && (nAna_version[2] > 15)) {
		//DisableFingerTouch();
		DrvDisableFingerTouchReport();
		//TouchDeviceResetHw();
		DrvTouchDeviceHwReset();
		EnterDBBus();
		if (!EnterMpMode()) {	// to get key combine or key seperate scan mode
			//udelay(100000);
			mdelay(100);
			StopMCU();
			regData = RegGet16BitValueByAddressMode(0x136E, ADDRESS_MODE_16BIT);

			ptMutualMpTestResult->mapTbl_sec = (char *)kmalloc(FILENAME_MAX * sizeof(char), GFP_KERNEL);

			if (regData == KEY_COMBINE)
				sprintf(ptMutualMpTestResult->mapTbl_sec, "MAPPING_TABLE_BARKER%d_KEY", RegGetLByteValue(0x1312));
			else if (regData == KEY_SEPERATE)
				sprintf(ptMutualMpTestResult->mapTbl_sec, "MAPPING_TABLE_BARKER%d", RegGetLByteValue(0x1312));
			else
				sprintf(ptMutualMpTestResult->mapTbl_sec, "MAPPING_TABLE_BARKER%d", RegGetLByteValue(0x1312));

			DBG(0,"mapping table name = %s, code length = %d, scan mode = 0x%x\n", 
							ptMutualMpTestResult->mapTbl_sec, RegGetLByteValue(0x1312), regData);
			
			nMapSize = ms_ini_2d_array(iniFilePath, "MAPPING_TABLE_BARKER", szMapTable);
			   // nMapSize = ms_ini_2d_array(iniFilePath, mapTbl_sec, szMapTable);
		
			DBG(0,"mapping number = %d\n", nMapSize);
			ptMutualMpTest->sensorInfo.mapping = (MutualMapping_t *)kcalloc(nMapSize, sizeof(MutualMapping_t), GFP_KERNEL);

			if(nMapSize == 0)
			{
				return -1;				
			}
			else
			{
				for(i=0; i<nMapSize; i++)
				{	
					ptMutualMpTest->sensorInfo.mapping[i].X = szMapTable[i][0];
					ptMutualMpTest->sensorInfo.mapping[i].Y = szMapTable[i][1];
				}
			}
		}
	}

	calGoldenRange(ptMutualMpTest->Goldensample_CH_0,
		ptMutualMpTest->ToastInfo.persentDC_VA_Range, ptMutualMpTest->ToastInfo.persentDC_VA_Range_up,
		ptMutualMpTest->Goldensample_CH_0_Max, ptMutualMpTest->Goldensample_CH_0_Min, MAX_MUTUAL_NUM);

	ptMutualMpTestResult->pCheck_Fail = (int *)kmalloc(sizeof(int) * TEST_ITEM_NUM, GFP_KERNEL);
	ptMutualMpTestResult->pOpenResultData = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL);//(int *)malloc(MAX_MUTUAL_NUM * sizeof(int));
	ptMutualMpTestResult->pOpenFailChannel = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL);//(int *)malloc(MAX_MUTUAL_NUM * sizeof(int));
	ptMutualMpTestResult->pOpenRatioFailChannel = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );

	#ifndef DISABLE_DOUBLE
	ptMutualMpTestResult->pGolden_CH = (double *)kcalloc(MAX_MUTUAL_NUM, sizeof(double), GFP_KERNEL );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double) );
	ptMutualMpTestResult->pGolden_CH_Max = (double *)kcalloc(MAX_MUTUAL_NUM, sizeof(double), GFP_KERNEL );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTestResult->pGolden_CH_Max_Avg = (double *)kcalloc(MAX_MUTUAL_NUM, sizeof(double), GFP_KERNEL );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTestResult->pGolden_CH_Min = (double *)kcalloc(MAX_MUTUAL_NUM, sizeof(double), GFP_KERNEL );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTestResult->pGolden_CH_Min_Avg = (double *)kcalloc(MAX_MUTUAL_NUM, sizeof(double), GFP_KERNEL );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));

	ptMutualMpTestResult->nRatioAvg_max = (double) (100 + ptMutualMpTest->ToastInfo.persentDC_VA_Ratio + ptMutualMpTest->ToastInfo.persentDC_VA_Ratio_up) / 100;
	ptMutualMpTestResult->nRatioAvg_min =(double) (100 - ptMutualMpTest->ToastInfo.persentDC_VA_Ratio) / 100;
	ptMutualMpTestResult->nBorder_RatioAvg_max = (double) (100 + ptMutualMpTest->ToastInfo.persentDC_Border_Ratio + ptMutualMpTest->ToastInfo.persentDC_VA_Ratio_up) / 100;
	ptMutualMpTestResult->nBorder_RatioAvg_min = (double) (100 - ptMutualMpTest->ToastInfo.persentDC_Border_Ratio) / 100;
	#else
	ptMutualMpTestResult->pGolden_CH = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double) );
	ptMutualMpTestResult->pGolden_CH_Max = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTestResult->pGolden_CH_Max_Avg = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTestResult->pGolden_CH_Min = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));
	ptMutualMpTestResult->pGolden_CH_Min_Avg = (int *)kcalloc(MAX_MUTUAL_NUM, sizeof(int), GFP_KERNEL );//(double *)malloc(MAX_MUTUAL_NUM * sizeof(double));

	ptMutualMpTestResult->nRatioAvg_max = (int) (100 + ptMutualMpTest->ToastInfo.persentDC_VA_Ratio + ptMutualMpTest->ToastInfo.persentDC_VA_Ratio_up) / 100;
	ptMutualMpTestResult->nRatioAvg_min =(int) (100 - ptMutualMpTest->ToastInfo.persentDC_VA_Ratio) / 100;
	ptMutualMpTestResult->nBorder_RatioAvg_max = (int) (100 + ptMutualMpTest->ToastInfo.persentDC_Border_Ratio + ptMutualMpTest->ToastInfo.persentDC_VA_Ratio_up) / 100;
	ptMutualMpTestResult->nBorder_RatioAvg_min = (int) (100 - ptMutualMpTest->ToastInfo.persentDC_Border_Ratio) / 100;
	#endif

	ptMutualMpTestResult->pShortFailChannel = (int *)kcalloc(MAX_CHANNEL_NUM_28XX, sizeof(int), GFP_KERNEL);//(int *)malloc(MAX_CHANNEL_NUM_28XX * sizeof(int));
	ptMutualMpTestResult->pShortResultData = (int *)kcalloc(MAX_CHANNEL_NUM_28XX, sizeof(int), GFP_KERNEL);//(int *)malloc(MAX_CHANNEL_NUM_28XX * sizeof(int));
	ptMutualMpTestResult->pICPinChannel = (int *)kcalloc(MAX_CHANNEL_NUM_28XX, sizeof(int), GFP_KERNEL);//(int *)malloc(MAX_CHANNEL_NUM_28XX * sizeof(int));
	ptMutualMpTestResult->pICPinShortFailChannel = (int *)kcalloc(MAX_CHANNEL_NUM_28XX, sizeof(int), GFP_KERNEL);//(int *)malloc(MAX_CHANNEL_NUM_28XX * sizeof(int)); 
	ptMutualMpTestResult->pICPinShortResultData = (int *)kcalloc(MAX_CHANNEL_NUM_28XX, sizeof(int), GFP_KERNEL);//(int *)malloc(MAX_CHANNEL_NUM_28XX * sizeof(int));
#ifndef DISABLE_DOUBLE
	ptMutualMpTestResult->pICPinShortRData = (double *)kcalloc(MAX_CHANNEL_NUM_28XX, sizeof(double), GFP_KERNEL);//(double *)malloc(MAX_CHANNEL_NUM_28XX * sizeof(double));
	ptMutualMpTestResult->pShortRData = (double *)kcalloc(MAX_CHANNEL_NUM_28XX, sizeof(double), GFP_KERNEL);//(double *)malloc(MAX_CHANNEL_NUM_28XX * sizeof(double));
#else
	ptMutualMpTestResult->pICPinShortRData = (int *)kcalloc(MAX_CHANNEL_NUM_28XX, sizeof(int), GFP_KERNEL);//(double *)malloc(MAX_CHANNEL_NUM_28XX * sizeof(double));
	ptMutualMpTestResult->pShortRData = (int *)kcalloc(MAX_CHANNEL_NUM_28XX, sizeof(int), GFP_KERNEL);//(double *)malloc(MAX_CHANNEL_NUM_28XX * sizeof(double));
#endif

	return 0;
}


int Msg28xxStartMPTest(void)
{
	int i, res = 0;
    u16 fw_ver = 0xFF;

    printk("*** %s() ***\n", __func__);

	fw_ver = getFirmwareOfficialVersionOnFlash();
	if (fw_ver == 0xFF)
	     	fw_ver = 0;
	
	DBG(0,"info fw_ver = %x \n", fw_ver);

	if(Msg28xxInitVariables() < 0)
	{
		res = -1;
		goto out;
	}

    if(ptMutualMpTest->UIConfig.bOpenTest == 1){
 
    	ptMutualMpTestResult->nOpenResult = Msg28xxOpenTest(fw_ver);

    	for (i = 0; i < MAX_MUTUAL_NUM; i++)
    	{
    		ptMutualMpTestResult->pGolden_CH[i] = ptMutualMpTest->Goldensample_CH_0[i];
    		ptMutualMpTestResult->pGolden_CH_Max[i] = ptMutualMpTest->Goldensample_CH_0_Max[i];
    		ptMutualMpTestResult->pGolden_CH_Min[i] = ptMutualMpTest->Goldensample_CH_0_Min[i];
    	}
    }
    else {
    	ptMutualMpTestResult->nOpenResult = ITO_NO_TEST;
    }

    if(ptMutualMpTest->UIConfig.bShortTest == 1){

    	ptMutualMpTestResult->nShortResult = Msg28xxShortTest();
    }
    else {
    	ptMutualMpTestResult->nShortResult = ITO_NO_TEST;
    }

    if(ptMutualMpTest->UIConfig.bWpTest == 1){
    	ptMutualMpTestResult->pWaterProofResultData = (int *)kmalloc(sizeof(int) * 8, GFP_KERNEL);
    	ptMutualMpTestResult->nWaterProofResult = Msg28xxWaterProofTest();
    }
    else {
    	ptMutualMpTestResult->nWaterProofResult = ITO_NO_TEST;
	}
	
out:
	DrvTouchDeviceHwReset();
	mdelay(300);
	DrvEnableFingerTouchReport();
	kfree(iniFilePath);
	return res;
}