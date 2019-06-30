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
 
#include "CreateCSV.h"

extern MutualMpTest_t * ptMutualMpTest;
extern MutualMpTestResult_t * ptMutualMpTestResult;
extern MutualMpTest_t * ptMsg30xxMutualMpTest;
extern MutualMpTestResult_t * ptMsg30xxMutualMpTestResult;
extern struct _TP_INFO tpinfo;
#ifdef CONFIG_ENABLE_ITO_MP_TEST
extern u32 g_IsInMpTest;
int MpTestStatus = 0;

static void convert_csv_msg28xx(void)
{
	int i = 0, j = 0, max_channel, test_Interval = 3, testCount = 0, failCount = 0;
//	char *pbuf = NULL;
	char sResult[10];
	char *SetCsvData = NULL, CsvPATHName[256] = "";
	struct file *f=NULL;
	mm_segment_t fs;

	SetCsvData = kmalloc(1024,GFP_KERNEL);

	if (ptMutualMpTestResult->nShortResult == ITO_TEST_OK &&
			 ptMutualMpTestResult->nOpenResult == ITO_TEST_OK)
	{
		strcpy(sResult, "PASS");
		MpTestStatus = ITO_TEST_OK;
	} 
	else
	{
		if (testCount >= test_Interval) 
		{
			failCount++;
			if (failCount == test_Interval)
				strcpy(sResult, "FAIL");
			else 
				strcpy(sResult, "FAIL");
		}
		strcpy(sResult, "FAIL");
		MpTestStatus = ITO_TEST_FAIL;
	}
	
	sprintf(CsvPATHName,"/sdcard/%s.csv", sResult);
	printk("CSV:%s\n", CsvPATHName);
	if(f == NULL)
	{
		f = filp_open(CsvPATHName, O_CREAT | O_RDWR , 0644);		
	}

	if(IS_ERR(f) || f == NULL)
	{
		printk("Failed to open csv file %s\n", CsvPATHName);
		kfree(SetCsvData);
		return;
	}
	printk("Open csv:%s\n", CsvPATHName);

	fs = get_fs();
	set_fs(KERNEL_DS);
	
	//sprintf(SetCsvData, "Golden 0 Max,,");
	strcpy(SetCsvData, "Golden 0 Max,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);
	for(i = 0; i < MAX_MUTUAL_NUM; i++)
	{
		//sprintf(SetCsvData, "%1f,", ptMutualMpTestResult->pGolden_CH_Max[i]);
		sprintf(SetCsvData, "%d,", ptMutualMpTestResult->pGolden_CH_Max[i]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);
	
	//sprintf(SetCsvData, "Golden 0,,");
	strcpy(SetCsvData, "Golden 0,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	for(i = 0; i < MAX_MUTUAL_NUM; i++)
	{
		//sprintf(SetCsvData, "%1f,", ptMutualMpTestResult->pGolden_CH[i]);
		sprintf(SetCsvData, "%d,", ptMutualMpTestResult->pGolden_CH[i]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);
	
	//sprintf(SetCsvData, "Golden 0 Min,,");
	strcpy(SetCsvData, "Golden 0 Min,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);
	for(i = 0; i < MAX_MUTUAL_NUM; i++)
	{
		//sprintf(SetCsvData, "%1f,", ptMutualMpTestResult->pGolden_CH_Min[i]);
		sprintf(SetCsvData, "%1d,", ptMutualMpTestResult->pGolden_CH_Min[i]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");	
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	
	//sprintf(SetCsvData, "test_0_deltaC,,");
	strcpy(SetCsvData, "test_0_deltaC,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	for(i = 0; i < MAX_MUTUAL_NUM; i++)
	{
		sprintf(SetCsvData, "%d,", ptMutualMpTestResult->pOpenResultData[i]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	
	//sprintf(SetCsvData, "test_0_ratio,[%1$.2f~%2$.2f],", ptMutualMpTestResult->nRatioAvg_max, ptMutualMpTestResult->nRatioAvg_min);
	sprintf(SetCsvData, "test_0_ratio,[%1d~%2d],", ptMutualMpTestResult->nRatioAvg_max, ptMutualMpTestResult->nRatioAvg_min);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	for(i = 0; i < MAX_MUTUAL_NUM; i++)
	{
		//sprintf(SetCsvData, "%1f,", ptMutualMpTestResult->pGolden_CH_Max_Avg[i]);
		sprintf(SetCsvData, "%1d,", ptMutualMpTestResult->pGolden_CH_Max_Avg[i]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	
	//sprintf(SetCsvData, "test_border_ratio,[%1$.2f~%2$.2f] ", ptMutualMpTestResult->nBorder_RatioAvg_max, ptMutualMpTestResult->nBorder_RatioAvg_min);
	sprintf(SetCsvData, "test_border_ratio,[%1d~%2d] ", ptMutualMpTestResult->nBorder_RatioAvg_max, ptMutualMpTestResult->nBorder_RatioAvg_min);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	for(i = 0; i < MAX_MUTUAL_NUM; i++)
	{
		sprintf(SetCsvData, "%s", ",");
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "Platform Version :%s\n", tpinfo.PlatformVersion);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	sprintf(SetCsvData, "Device Driver Version : %s\n", DEVICE_DRIVER_RELEASE_VERSION);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	sprintf(SetCsvData, "FW Version : %s\n", tpinfo.FwVersion);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	sprintf(SetCsvData, "Main Block FW Version : %s\n", tpinfo.MainBlockFWVersion);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	sprintf(SetCsvData, "Info Block FW Version : %s\n", tpinfo.InfoBlockFWVersion);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	sprintf(SetCsvData, "ANA_Version : %s\n", ptMutualMpTest->ana_version);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	sprintf(SetCsvData, "SupportIC : %s\n", ptMutualMpTest->UIConfig.sSupportIC);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	sprintf(SetCsvData, "Project name : %s\n", ptMutualMpTest->project_name);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);
	sprintf(SetCsvData, "Mapping table name : %s\n", ptMutualMpTestResult->mapTbl_sec);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	//sprintf(SetCsvData, "DC_Range=%u\n", ptMutualMpTest->ToastInfo.persentDC_VA_Range);
	sprintf(SetCsvData, "DC_Range=%d\n", ptMutualMpTest->ToastInfo.persentDC_VA_Range);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	//sprintf(SetCsvData, "DC_Range_up=%hu\n", ptMutualMpTest->ToastInfo.persentDC_VA_Range_up);
	sprintf(SetCsvData, "DC_Range_up=%d\n", ptMutualMpTest->ToastInfo.persentDC_VA_Range_up);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	//sprintf(SetCsvData, "DC_Ratio=%u\n", ptMutualMpTest->ToastInfo.persentDC_VA_Ratio);
	sprintf(SetCsvData, "DC_Ratio=%d\n", ptMutualMpTest->ToastInfo.persentDC_VA_Ratio);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	//sprintf(SetCsvData, "DC_Border_Ratio=%u\n", ptMutualMpTest->ToastInfo.persentDC_Border_Ratio);
	sprintf(SetCsvData, "DC_Border_Ratio=%d\n", ptMutualMpTest->ToastInfo.persentDC_Border_Ratio);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	//sprintf(SetCsvData, "DC_Ratio_up=%hu\n\n", ptMutualMpTest->ToastInfo.persentDC_VA_Ratio_up);
	sprintf(SetCsvData, "DC_Ratio_up=%d\n\n", ptMutualMpTest->ToastInfo.persentDC_VA_Ratio_up);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
 
	//sprintf(SetCsvData, "Golden,,");
	strcpy(SetCsvData, "Golden,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	for(i = 0; i < ptMutualMpTest->sensorInfo.numDrv; i++)
	{
		sprintf(SetCsvData, "D%d,", i+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	for(j = 0; j < ptMutualMpTest->sensorInfo.numSen; j++)
	{
		sprintf(SetCsvData, ",S%d,", j+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		for(i = 0; i < ptMutualMpTest->sensorInfo.numDrv; i++)
		{
			if (ptMutualMpTestResult->pGolden_CH[j * ptMutualMpTest->sensorInfo.numDrv + i] == NULL_DATA) //for mutual key
			{
				sprintf(SetCsvData, "%s", ",");
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			}
			else
			{
				//sprintf(SetCsvData, "%1$.2f,", ptMutualMpTestResult->pGolden_CH[j * ptMutualMpTest->sensorInfo.numDrv + i]);
				sprintf(SetCsvData, "%.2d,", ptMutualMpTestResult->pGolden_CH[j * ptMutualMpTest->sensorInfo.numDrv + i]);
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			}
		}
		sprintf(SetCsvData, "\n");
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	
	//sprintf(SetCsvData, "Golden_Max,,");
	strcpy(SetCsvData, "Golden_Max,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	for(i = 0; i < ptMutualMpTest->sensorInfo.numDrv; i++)
	{
		sprintf(SetCsvData, "D%d,", i+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	for(j = 0; j < ptMutualMpTest->sensorInfo.numSen; j++)
	{
		sprintf(SetCsvData, ",S%d,", j+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		for(i = 0; i < ptMutualMpTest->sensorInfo.numDrv; i++)
		{
			if (ptMutualMpTestResult->pGolden_CH[j * ptMutualMpTest->sensorInfo.numDrv + i] == NULL_DATA) //for mutual key
			{
				sprintf(SetCsvData, "%s", ",");
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			}
			else
			{
				//sprintf(SetCsvData, "%1$.2f,", ptMutualMpTestResult->pGolden_CH_Max[j * ptMutualMpTest->sensorInfo.numDrv + i]);
				sprintf(SetCsvData, "%.2d,", ptMutualMpTestResult->pGolden_CH_Max[j * ptMutualMpTest->sensorInfo.numDrv + i]);
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			}
		}
		sprintf(SetCsvData, "\n");
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	
	//sprintf(SetCsvData, "Golden_Min,,");
	strcpy(SetCsvData, "Golden_Min,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	for(i = 0; i < ptMutualMpTest->sensorInfo.numDrv; i++)
	{
		sprintf(SetCsvData, "D%d,", i+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	for(j = 0; j < ptMutualMpTest->sensorInfo.numSen; j++)
	{
		sprintf(SetCsvData, ",S%d,", j+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		for(i = 0; i < ptMutualMpTest->sensorInfo.numDrv; i++)
		{
			if (ptMutualMpTestResult->pGolden_CH[j * ptMutualMpTest->sensorInfo.numDrv + i] == NULL_DATA) //for mutual key
			{
				sprintf(SetCsvData, "%s", ",");
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			}
			else
			{
				//sprintf(SetCsvData, "%1$.2f,", ptMutualMpTestResult->pGolden_CH_Min[j * ptMutualMpTest->sensorInfo.numDrv + i]);
				sprintf(SetCsvData, "%.2d,", ptMutualMpTestResult->pGolden_CH_Min[j * ptMutualMpTest->sensorInfo.numDrv + i]);
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			}
		}
		sprintf(SetCsvData, "\n");
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	
	//sprintf(SetCsvData, "DeltaC,,");
	strcpy(SetCsvData, "DeltaC,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	for(i = 0; i < ptMutualMpTest->sensorInfo.numDrv; i++)
	{
		sprintf(SetCsvData, "D%d,", i+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	for(j = 0; j < ptMutualMpTest->sensorInfo.numSen; j++)
	{
		sprintf(SetCsvData, ",S%d,", j+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		for(i = 0; i < ptMutualMpTest->sensorInfo.numDrv; i++)
		{
			if (ptMutualMpTestResult->pOpenResultData[j * ptMutualMpTest->sensorInfo.numDrv + i] == NULL_DATA) //for mutual key
			{
				sprintf(SetCsvData, "%s", ",");
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			}
			else
			{
				//sprintf(SetCsvData, "%1$d,", ptMutualMpTestResult->pOpenResultData[j * ptMutualMpTest->sensorInfo.numDrv + i]);
				sprintf(SetCsvData, "%1d,", ptMutualMpTestResult->pOpenResultData[j * ptMutualMpTest->sensorInfo.numDrv + i]);
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			}
		}
		sprintf(SetCsvData, "\n");
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	if(ptMutualMpTestResult->nOpenResult == 1)
	{
		sprintf(SetCsvData, "DeltaC_Result:PASS\n");
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	else
	{
		if(ptMutualMpTestResult->pCheck_Fail[0] == 1)
		{
			sprintf(SetCsvData, "DeltaC_Result:FAIL\nFail Channel:");
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);
			for (i = 0; i < MAX_MUTUAL_NUM; i++)
			{
				if (ptMutualMpTestResult->pOpenFailChannel[i] == PIN_NO_ERROR)
					continue;
					//sprintf(SetCsvData,"D%1$d.S%2$d", ptMutualMpTestResult->pOpenFailChannel[i] % 100, ptMutualMpTestResult->pOpenFailChannel[i] / 100);
					sprintf(SetCsvData,"D%1d.S%2d", ptMutualMpTestResult->pOpenFailChannel[i] % 100, ptMutualMpTestResult->pOpenFailChannel[i] / 100);
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);
			}
			sprintf(SetCsvData, "\n");
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
		else
		{
			sprintf(SetCsvData, "DeltaC_Result:PASS\n");
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
	}
	
	//sprintf(SetCsvData, "\nRatio,,");
	strcpy(SetCsvData, "\nRatio,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	for(i = 0; i < ptMutualMpTest->sensorInfo.numDrv; i++)
	{
		sprintf(SetCsvData, "D%d,", i+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);		
	for(j = 0; j < ptMutualMpTest->sensorInfo.numSen; j++)
	{
		sprintf(SetCsvData, ",S%d,", j+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		for(i = 0; i < ptMutualMpTest->sensorInfo.numDrv; i++)
		{
			if (ptMutualMpTestResult->pOpenResultData[j * ptMutualMpTest->sensorInfo.numDrv + i] == NULL_DATA) //for mutual key
			{
				sprintf(SetCsvData, "%s", ",");
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			}
			else
			{
				//sprintf(SetCsvData, "%1$f,", ptMutualMpTestResult->pGolden_CH_Max_Avg[j * ptMutualMpTest->sensorInfo.numDrv + i]);
				sprintf(SetCsvData, "%1d,", ptMutualMpTestResult->pGolden_CH_Max_Avg[j * ptMutualMpTest->sensorInfo.numDrv + i]);
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			}
		}
		sprintf(SetCsvData, "\n");
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	if(ptMutualMpTestResult->nOpenResult == 1)
	{
		sprintf(SetCsvData, "Ratio_Result:PASS\n");
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	else
	{
		if(ptMutualMpTestResult->pCheck_Fail[0] == 1)
		{
			sprintf(SetCsvData, "Ratio_Result:FAIL\nFail Channel:");
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);
			for (i = 0; i < MAX_MUTUAL_NUM; i++)
			{
				if (ptMutualMpTestResult->pOpenFailChannel[i] == PIN_NO_ERROR)
					continue;
					//sprintf(SetCsvData,"D%1$d.S%2$d", ptMutualMpTestResult->pOpenRatioFailChannel[i] % 100, ptMutualMpTestResult->pOpenRatioFailChannel[i] / 100);
					sprintf(SetCsvData,"D%1d.S%2d", ptMutualMpTestResult->pOpenRatioFailChannel[i] % 100, ptMutualMpTestResult->pOpenRatioFailChannel[i] / 100);
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);
			}
			sprintf(SetCsvData, "\n");
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
		else
		{
			sprintf(SetCsvData, "Ratio_Result:PASS\n");
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
	}
	sprintf(SetCsvData, "\n\nShortValue=%d\n\n", ptMutualMpTest->sensorInfo.thrsShort);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	
	sprintf(SetCsvData, "ICPinShort=%d\n\n", ptMutualMpTest->sensorInfo.thrsICpin);
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	

	//sprintf(SetCsvData, "Pin Number,,");
	strcpy(SetCsvData, "Pin Number,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	max_channel = MAX_CHANNEL_NUM_28XX;
	if (tpinfo.ChipType == CHIP_TYPE_MSG28XXA)
		max_channel = MAX_CHANNEL_NUM_30XX;

	for (i = 0; i < max_channel; i ++) {
		if (ptMutualMpTestResult->pICPinChannel[i] == 0) {
			continue;
		}
		//logHeader[i+1-j] = "P" + Integer.toString(ptMutualMpTestResult->pICPinChannel[i]);
		sprintf(SetCsvData, "P%d,",ptMutualMpTestResult->pICPinChannel[i]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	//sprintf(SetCsvData, "\ndeltaR,,");
	strcpy(SetCsvData, "\ndeltaR,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	for (i = 0; i < max_channel; i ++) {
		if (ptMutualMpTestResult->pICPinChannel[i] == 0) {
			continue;
		}
		//sprintf(SetCsvData, "%1$.1fM,",ptMutualMpTestResult->pICPinShortRData[i]);
		sprintf(SetCsvData, "%1dM,",ptMutualMpTestResult->pICPinShortRData[i]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}

	//sprintf(SetCsvData, "\nresultData,,");
	strcpy(SetCsvData, "\nresultData,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	for (i = 0; i < max_channel; i ++) {
		if (ptMutualMpTestResult->pICPinChannel[i] == 0) {
			continue;
		}
		//sprintf(SetCsvData, "%1$d,",ptMutualMpTestResult->pICPinShortResultData[i]);
		sprintf(SetCsvData, "%d,",ptMutualMpTestResult->pICPinShortResultData[i]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	
	if (ptMutualMpTestResult->nShortResult == ITO_TEST_OK) {
		sprintf(SetCsvData, "\nICPin Short Test:PASS\n");
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	else {
		if (ptMutualMpTestResult->pCheck_Fail[2] == 1) {
			sprintf(SetCsvData, "\nICPin Short Test:FAIL\nFail Channel:,,");
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			for (i = 0; i < max_channel; i++) {
				if (ptMutualMpTestResult->pICPinShortFailChannel[i] == 0) {
					continue;
				}
				//sprintf(SetCsvData, "P%1$d,",ptMutualMpTestResult->pICPinShortFailChannel[i]);
				sprintf(SetCsvData, "P%d,",ptMutualMpTestResult->pICPinShortFailChannel[i]);
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
			}
		} else {

			sprintf(SetCsvData, "\nICPin Short Test:PASS\n");
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
	}

	//sprintf(SetCsvData, "\ndeltaR,,");
	strcpy(SetCsvData, "\ndeltaR,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	for (i = 0; i < 10; i ++) {	    						
		sprintf(SetCsvData, "%d,", i+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}	
	for (i = 0; i < (ptMutualMpTest->sensorInfo.numSen); i ++) {
		if ((i % 10) == 0) {
			sprintf(SetCsvData, "\n,S%d,", i);
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
		//sprintf(SetCsvData, "%1$.1fM,", ptMutualMpTestResult->pShortRData[i]);
		sprintf(SetCsvData, "%dM,", ptMutualMpTestResult->pShortRData[i]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	for (i = 0; i < (ptMutualMpTest->sensorInfo.numDrv); i ++) {
		if ((i % 10) == 0) {
			sprintf(SetCsvData, "\n,D%d,", i);
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
		//sprintf(SetCsvData, "%1$.1fM,",  ptMutualMpTestResult->pShortRData[i + ptMutualMpTest->sensorInfo.numSen]);
		DBG(0," pshortdata = %d \n",  ptMutualMpTestResult->pShortRData[i + ptMutualMpTest->sensorInfo.numSen]);
		sprintf(SetCsvData, "%1dM,",  ptMutualMpTestResult->pShortRData[i + ptMutualMpTest->sensorInfo.numSen]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	
	sprintf(SetCsvData, "\n");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);				
	for (i = 0; i < ptMutualMpTest->sensorInfo.numGr; i ++) {
		if ((i % 10) == 0) {
			sprintf(SetCsvData, "\n,GR%d,", i);
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
		//sprintf(SetCsvData, "%1$d,",  ptMutualMpTestResult->pShortResultData[i + ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv]);
		sprintf(SetCsvData, "%d,",  ptMutualMpTestResult->pShortResultData[i + ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	
	if (ptMutualMpTestResult->nShortResult == ITO_TEST_OK) {
		//sprintf(SetCsvData, "\nITO Short Test:PASS,");
		strcpy(SetCsvData, "\nITO Short Test:PASS,");
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	else {
		if (ptMutualMpTestResult->pCheck_Fail[3] == 1) {
			if (testCount >= test_Interval) {
				if (failCount == test_Interval) {
					//sprintf(SetCsvData, "\nITO Short Test:FAIL\nFail Channel:,,");
					strcpy(SetCsvData, "\nITO Short Test:FAIL\nFail Channel:,,");
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
					j = 0;
					for (i = 0; i < ptMutualMpTest->sensorInfo.numSen; i++) {
						if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
							continue;
						//sprintf(SetCsvData, "S%1$d,", i + 1);
						sprintf(SetCsvData, "S%d,", i + 1);
						f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
					}
					for (; i < ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv; i++) {
						if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
							continue;
						//sprintf(SetCsvData, "D%1$d,", i + 1 - ptMutualMpTest->sensorInfo.numSen);
						sprintf(SetCsvData, "D%d,", i + 1 - ptMutualMpTest->sensorInfo.numSen);
						f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
					}
					for (; i < ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv + ptMutualMpTest->sensorInfo.numGr; i++) {
						if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
							continue;
						//sprintf(SetCsvData, "GR%1$d", i + 1 - ptMutualMpTest->sensorInfo.numSen - ptMutualMpTest->sensorInfo.numDrv);
						sprintf(SetCsvData, "GR%d", i + 1 - ptMutualMpTest->sensorInfo.numSen - ptMutualMpTest->sensorInfo.numDrv);
						f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
					}
				} else {
					//sprintf(SetCsvData, "\nITO Short Test:PASS,");
					strcpy(SetCsvData, "\nITO Short Test:PASS,");
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
				}
			} else {
				//sprintf(SetCsvData, "\nITO Short Test:FAIL\nFail Channel:,,");
				strcpy(SetCsvData, "\nITO Short Test:FAIL\nFail Channel:,,");
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
				j = 0;
				for (i = 0; i < ptMutualMpTest->sensorInfo.numSen; i++) {
					if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
						continue;
					//sprintf(SetCsvData, "S%1$d,", i + 1);
					sprintf(SetCsvData, "S%d,", i + 1);
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
				}
				for (; i < ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv; i++) {
					if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
						continue;
					//sprintf(SetCsvData, "D%1$d,", i + 1 - ptMutualMpTest->sensorInfo.numSen);
					sprintf(SetCsvData, "D%d,", i + 1 - ptMutualMpTest->sensorInfo.numSen);
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
				}
				for (; i < ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv + ptMutualMpTest->sensorInfo.numGr; i++) {
					if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
						continue;
					//sprintf(SetCsvData, "GR%1$d", i + 1 - ptMutualMpTest->sensorInfo.numSen - ptMutualMpTest->sensorInfo.numDrv);
					sprintf(SetCsvData, "GR%d", i + 1 - ptMutualMpTest->sensorInfo.numSen - ptMutualMpTest->sensorInfo.numDrv);
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
				}
			}
		}
		else
		{
			//sprintf(SetCsvData, "\nITO Short Test:PASS,");
			strcpy(SetCsvData, "\nITO Short Test:PASS,");
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
	}

	//sprintf(SetCsvData, "\nresultData,,");
	strcpy(SetCsvData, "\nresultData,,");
	f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	for (i = 0; i < 10; i ++) {	    						
		sprintf(SetCsvData, "%d,", i+1);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	for (i = 0; i < (ptMutualMpTest->sensorInfo.numSen); i ++) {
		if ((i % 10) == 0) {
			sprintf(SetCsvData, "\n,S%d,", i);
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
		//sprintf(SetCsvData, "%1$d,",  ptMutualMpTestResult->pShortResultData[i]);
		sprintf(SetCsvData, "%d,",  ptMutualMpTestResult->pShortResultData[i]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	
	for (i = 0; i < (ptMutualMpTest->sensorInfo.numDrv); i ++) {
		if ((i % 10) == 0) {
			sprintf(SetCsvData, "\n,D%d,", i);
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
		//sprintf(SetCsvData, "%1$d,",  ptMutualMpTestResult->pShortResultData[i + ptMutualMpTest->sensorInfo.numSen]);
		sprintf(SetCsvData, "%d,",  ptMutualMpTestResult->pShortResultData[i + ptMutualMpTest->sensorInfo.numSen]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	
	for (i = 0; i < (ptMutualMpTest->sensorInfo.numGr); i ++) {
		if ((i % 10) == 0) {
			sprintf(SetCsvData, "\n,GR%d,", i);
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
		//sprintf(SetCsvData, "%1$d,",  ptMutualMpTestResult->pShortResultData[i + ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv]);
		sprintf(SetCsvData, "%d,",  ptMutualMpTestResult->pShortResultData[i + ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv]);
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	
	if (ptMutualMpTestResult->nShortResult == ITO_TEST_OK) {
		//sprintf(SetCsvData, "\nITO Short Test:PASS\n");
		strcpy(SetCsvData, "\nITO Short Test:PASS\n");
		f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
	}
	else {
		if (ptMutualMpTestResult->pCheck_Fail[3] == 1) {
			if (testCount >= test_Interval) {
				if (failCount == test_Interval) {
					//sprintf(SetCsvData, "\nITO Short Test:FAIL\nFail Channel:,,");
					strcpy(SetCsvData, "\nITO Short Test:FAIL\nFail Channel:,,");
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
					j = 0;
					for (i = 0; i < ptMutualMpTest->sensorInfo.numSen; i++) {
						if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
							continue;
						//sprintf(SetCsvData, "S%1$d,", i + 1);
						sprintf(SetCsvData, "S%d,", i + 1);
						f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
					}
					for (; i < ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv; i++) {
						if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
							continue;
						//sprintf(SetCsvData, "D%1$d,", i + 1 - ptMutualMpTest->sensorInfo.numSen);
						sprintf(SetCsvData, "D%d,", i + 1 - ptMutualMpTest->sensorInfo.numSen);
						f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
					}
					for (; i < ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv + ptMutualMpTest->sensorInfo.numGr; i++) {
						if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
							continue;
						//sprintf(SetCsvData, "GR%1$d", i + 1 - ptMutualMpTest->sensorInfo.numSen - ptMutualMpTest->sensorInfo.numDrv);
						sprintf(SetCsvData, "GR%d", i + 1 - ptMutualMpTest->sensorInfo.numSen - ptMutualMpTest->sensorInfo.numDrv);
						f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
					}
				} else {
					//sprintf(SetCsvData, "\nITO Short Test:PASS\n");
					strcpy(SetCsvData, "\nITO Short Test:PASS\n");
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
				}
			} else {
				//sprintf(SetCsvData, "\nITO Short Test:FAIL\nFail Channel:,,");
				strcpy(SetCsvData, "\nITO Short Test:FAIL\nFail Channel:,,");
				f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
				j = 0;
				for (i = 0; i < ptMutualMpTest->sensorInfo.numSen; i++) {
					if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
						continue;
					//sprintf(SetCsvData, "S%1$d,", i + 1);
					sprintf(SetCsvData, "S%d,", i + 1);
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
				}
				for (; i < ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv; i++) {
					if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
						continue;
					//sprintf(SetCsvData, "D%1$d,", i + 1 - ptMutualMpTest->sensorInfo.numSen);
					sprintf(SetCsvData, "D%d,", i + 1 - ptMutualMpTest->sensorInfo.numSen);
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
				}
				for (; i < ptMutualMpTest->sensorInfo.numSen + ptMutualMpTest->sensorInfo.numDrv + ptMutualMpTest->sensorInfo.numGr; i++) {
					if (ptMutualMpTestResult->pShortFailChannel[i] == PIN_UN_USE)
						continue;
					//sprintf(SetCsvData, "GR%1$d", i + 1 - ptMutualMpTest->sensorInfo.numSen - ptMutualMpTest->sensorInfo.numDrv);
					sprintf(SetCsvData, "GR%d", i + 1 - ptMutualMpTest->sensorInfo.numSen - ptMutualMpTest->sensorInfo.numDrv);
					f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
				}
			}
		}
		else
		{
			//sprintf(SetCsvData, "\nITO Short Test:PASS,");
			strcpy(SetCsvData, "\nITO Short Test:PASS,");
			f->f_op->write(f, SetCsvData, strlen(SetCsvData) * sizeof(char), &f->f_pos);	
		}
	}

	set_fs(fs);
	filp_close(f, NULL);
	kfree(SetCsvData);
	return;
}

int get_mp_test_status(void)
{
	if(g_IsInMpTest)
	{
		MpTestStatus = ITO_TEST_PROCESSING;
	}
	else
	{
		 // if mp test was crashed
		if(MpTestStatus != ITO_TEST_OK && MpTestStatus != ITO_TEST_FAIL) 
			MpTestStatus = ITO_TEST_UNDEFINED_ERROR;
	}

	return MpTestStatus;
}

void save_test_data(int nChipType)
{
	if(nChipType == CHIP_TYPE_MSG28XX)
	{
		convert_csv_msg28xx();
	}
	else if(nChipType == CHIP_TYPE_ILI2117A || nChipType == CHIP_TYPE_ILI2118A)
	{
		ptMutualMpTest= (MutualMpTest_t *)kcalloc(1, sizeof(MutualMpTest_t), GFP_KERNEL);
   		ptMutualMpTestResult = (MutualMpTestResult_t *)kcalloc(1, sizeof(MutualMpTestResult_t), GFP_KERNEL);
		memcpy(ptMutualMpTest, ptMsg30xxMutualMpTest, sizeof(*ptMsg30xxMutualMpTest));
		memcpy(ptMutualMpTestResult, ptMsg30xxMutualMpTestResult, sizeof(*ptMsg30xxMutualMpTestResult));
		convert_csv_msg28xx();
		kfree(ptMutualMpTest);
		kfree(ptMutualMpTestResult);
	}
	else
	{
		printk("%s: It doesn't support this chip type: 0x%x \n", __func__, nChipType);
	}
	
	return;
}
#endif