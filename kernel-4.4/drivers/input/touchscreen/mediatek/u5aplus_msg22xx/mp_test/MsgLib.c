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

#include "MpCommon.h"

extern struct i2c_client *g_I2cClient;

extern u8 *_gPlatformFwVersion; // internal use firmware version for MStar
extern u8 *_gFwVersion; // customer firmware version
extern struct mutex g_Mutex;

struct _TP_INFO tpinfo;
extern int MpTestStatus;
#ifdef CONFIG_ENABLE_ITO_MP_TEST
extern u32 g_IsInMpTest;
int getFirmwareVersionOnFlash(void)
{
	uint8_t nChipType = 0;
//	uint8_t *pVer;
	//uint8_t *unknow_version="unknow";
	uint8_t *main_fw_ver = kmalloc(4, GFP_KERNEL);
	uint8_t *info_fw_ver = kmalloc(4, GFP_KERNEL);
	uint8_t *ret = kmalloc(21, GFP_KERNEL); //0001.0001 / 0001.0001
	uint16_t main_major,main_minor;
	uint16_t info_major,info_minor;
#ifdef BUILD_FUNCTION_APK 
    jbyteArray result;
#else
	uint8_t result = 0;
#endif
	memset(main_fw_ver,0,4);
	memset(info_fw_ver,0,4);
	memset(ret,0,21);

    DBG(&g_I2cClient->dev, "*** %s *** \n", __func__);

    EnterDBBus();
    StopMCU();

	nChipType = RegGet16BitValue(0x1ECC) & 0xFF;
	printk(":%s: ==== nChipType = 0x%x \n", __func__, nChipType);

	if (nChipType != CHIP_TYPE_MSG28XX &&   // (0x85)
		nChipType != CHIP_TYPE_MSG28XXA &&
		nChipType != CHIP_TYPE_ILI2117A &&
		nChipType != CHIP_TYPE_ILI2118A)     // (0xBF)
		{
			nChipType = 0;
		}

	switch(nChipType) {

		case  CHIP_TYPE_MSG28XX:
		case  CHIP_TYPE_MSG28XXA:
			ReadFlash(nChipType,0x1fff4,EMEM_TYPE_MAIN_BLOCK,4,main_fw_ver);

		    //DEBUG("*** %s() main_fw_ver[0]:0x%x main_fw_ver[1]:0x%x main_fw_ver[2]:0x%x main_fw_ver[3]:0x%x***\n",
		    //		__func__,main_fw_ver[0],main_fw_ver[1],main_fw_ver[2],main_fw_ver[3]);

			ReadFlash(nChipType,0x07ec,EMEM_TYPE_INFO_BLOCK,4,info_fw_ver);
		    //DEBUG("*** %s() info_fw_ver[0]:0x%x info_fw_ver[1]:0x%x info_fw_ver[2]:0x%x info_fw_ver[3]:0x%x***\n",
		    //		__func__,info_fw_ver[0],info_fw_ver[1],info_fw_ver[2],info_fw_ver[3]);


			//ret[0]='5';
			//ret[1]='6';
			break;
			
		default: 
			ret[0]='5';
			ret[1]='A';
			ret[2]='A';
			ret[3]='5';
		break;
	}

    StartMCU();
    ExitDBBus();

	main_major=(main_fw_ver[0]|main_fw_ver[1]<<8);
	main_minor=(main_fw_ver[2]|main_fw_ver[3]<<8);

	info_major=(info_fw_ver[0]|info_fw_ver[1]<<8);
	info_minor=(info_fw_ver[2]|info_fw_ver[3]<<8);

	printk("%d.%03d/%d.%03d\n", main_major, main_minor, info_major,
			info_minor);
	sprintf(ret, "%d.%03d/%d.%03d", main_major, main_minor, info_major,
			info_minor);
	printk("%d.%03d/%d.%03d\n", main_major, main_minor, info_major,
			info_minor);

#ifdef BUILD_FUNCTION_APK 
    result=(*env)->NewByteArray(env, 21);
    (*env)->SetByteArrayRegion(env, result, 0, 21, ret);
#else
	tpinfo.ChipType = nChipType;
	sprintf(tpinfo.MainBlockFWVersion, "%d.%03d", main_major, main_minor);
	sprintf(tpinfo.InfoBlockFWVersion, "%d.%03d", info_major, info_minor);
	sprintf(tpinfo.PlatformVersion, "%s", _gPlatformFwVersion);
	sprintf(tpinfo.FwVersion, "%s", _gFwVersion);
	
#endif

	kfree(main_fw_ver);
	kfree(info_fw_ver);
	kfree(ret);

	return result;
}

int startMPTest(int nChipType, char *pFilePath)
{
	int res = 0;
	//char *pFile;
	//size_t str_length = strlen(pFilePath);

	//pFile = kmalloc(str_length, GFP_KERNEL);	
	//memcpy((char*)pFile, (char *)pFilePath, str_length);

	DBG(&g_I2cClient->dev, "*** %s : nChipType = 0x%x *** \n", __func__,nChipType);
	DBG(&g_I2cClient->dev, "*** %s : Start running MP Test *** \n", __func__);
    DBG(&g_I2cClient->dev, "*** %s : iniPath = %s *** \n", __func__, pFilePath);

	mutex_lock(&g_Mutex);
    g_IsInMpTest = 1;
    mutex_unlock(&g_Mutex);

	if(nChipType == CHIP_TYPE_MSG28XX)
	{
		if(Msg28xxLoadIni(pFilePath) < 0)
		{
			DBG(&g_I2cClient->dev, "*** %s : MSG28xx failed to load ini *** \n", __func__);
			res = -1;
			goto out;
		}
		Msg28xxStartMPTest();
	}
	else if(nChipType == CHIP_TYPE_ILI2117A || nChipType == CHIP_TYPE_ILI2118A)
	{
		if(Msg30xxLoadIni(pFilePath) < 0)
		{
			DBG(&g_I2cClient->dev, "*** %s : 2117A/2117B failed to load ini *** \n", __func__);
			res = -1;
			goto out;
		}
		Msg30xxStartMPTest();
	}
	else
	{
		DBG(&g_I2cClient->dev, "*** %s : It doesn't support the MP test for this chip *** \n", __func__);
		res = -1;
		goto out;
	}

out:
	mutex_lock(&g_Mutex);
    g_IsInMpTest = 0;
    mutex_unlock(&g_Mutex);
	return res;

}

void EndMpTest(int nChipType)
{
	if(nChipType == CHIP_TYPE_MSG28XX)
	{
		Msg28xxEndMPTest();		
	}
	else if(nChipType == CHIP_TYPE_ILI2117A || nChipType == CHIP_TYPE_ILI2118A)
	{
		Msg30xxEndMPTest();		
	}

	return;
}
#endif